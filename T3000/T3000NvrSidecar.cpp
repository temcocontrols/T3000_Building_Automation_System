#include "stdafx.h"
#include "T3000NvrSidecar.h"

#include <string>
#include <ShlObj.h>
#include <Shlwapi.h>
#include <json/json.h>

#pragma comment(lib, "Shlwapi.lib")

namespace
{
	HANDLE g_job = NULL;
	HANDLE g_go2rtcProcess = NULL;
	HANDLE g_mediamtxProcess = NULL;
	bool g_ownGo2rtc = false;
	bool g_ownMediamtx = false;

	CString ExeDir()
	{
		TCHAR path[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, path, MAX_PATH);
		PathRemoveFileSpec(path);
		return CString(path);
	}

	bool FileExists(const CString& path)
	{
		const DWORD attr = GetFileAttributes(path);
		return attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY);
	}

	bool DirExists(const CString& path)
	{
		const DWORD attr = GetFileAttributes(path);
		return attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY);
	}

	void EnsureDir(const CString& path)
	{
		if (!DirExists(path))
			CreateDirectory(path, NULL);
	}

	CString Utf8ToCString(const std::string& s)
	{
		if (s.empty())
			return CString();
		const int n = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), NULL, 0);
		CString out;
		LPWSTR buf = out.GetBuffer(n);
		MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), buf, n);
		out.ReleaseBuffer(n);
		return out;
	}

	std::string CStringToUtf8(const CString& s)
	{
		if (s.IsEmpty())
			return std::string();
		const int n = WideCharToMultiByte(CP_UTF8, 0, s, s.GetLength(), NULL, 0, NULL, NULL);
		std::string out(n, 0);
		WideCharToMultiByte(CP_UTF8, 0, s, s.GetLength(), &out[0], n, NULL, NULL);
		return out;
	}

	CString YamlQuote(const CString& s)
	{
		CString t = s;
		t.Replace(_T("\\"), _T("\\\\"));
		t.Replace(_T("\""), _T("\\\""));
		return _T("\"") + t + _T("\"");
	}

	CString UrlEncode(const CString& s)
	{
		CStringA utf8 = CStringA(s);
		CString out;
		for (int i = 0; i < utf8.GetLength(); ++i)
		{
			unsigned char c = (unsigned char)utf8[i];
			if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
				(c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.' || c == '~')
			{
				out.AppendChar((TCHAR)c);
			}
			else
			{
				CString hex;
				hex.Format(_T("%%%02X"), c);
				out += hex;
			}
		}
		return out;
	}

	CString FileUri(const CString& winPath)
	{
		CString p = winPath;
		p.Replace(_T("\\"), _T("/"));
		if (p.GetLength() >= 2 && p[1] == _T(':'))
			return _T("file:///") + p;
		return _T("file://") + p;
	}

	bool TcpPortOpen(int port, int timeoutMs)
	{
		SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (s == INVALID_SOCKET)
			return false;

		u_long nonblock = 1;
		ioctlsocket(s, FIONBIO, &nonblock);

		sockaddr_in addr = {};
		addr.sin_family = AF_INET;
		addr.sin_port = htons((u_short)port);
		addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

		connect(s, (sockaddr*)&addr, sizeof(addr));

		fd_set writeSet;
		FD_ZERO(&writeSet);
		FD_SET(s, &writeSet);
		timeval tv;
		tv.tv_sec = timeoutMs / 1000;
		tv.tv_usec = (timeoutMs % 1000) * 1000;
		const int sel = select(0, NULL, &writeSet, NULL, &tv);
		bool ok = false;
		if (sel > 0 && FD_ISSET(s, &writeSet))
		{
			int err = 0;
			int len = sizeof(err);
			getsockopt(s, SOL_SOCKET, SO_ERROR, (char*)&err, &len);
			ok = (err == 0);
		}
		closesocket(s);
		return ok;
	}

	bool HttpGetLocal(int port, const char* path, CStringA& body, int timeoutMs)
	{
		body.Empty();
		SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (s == INVALID_SOCKET)
			return false;

		DWORD tv = (DWORD)timeoutMs;
		setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
		setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof(tv));

		sockaddr_in addr = {};
		addr.sin_family = AF_INET;
		addr.sin_port = htons((u_short)port);
		addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		if (connect(s, (sockaddr*)&addr, sizeof(addr)) != 0)
		{
			closesocket(s);
			return false;
		}

		CStringA req;
		req.Format("GET %s HTTP/1.0\r\nHost: 127.0.0.1:%d\r\nConnection: close\r\n\r\n", path, port);
		if (send(s, req.GetString(), req.GetLength(), 0) <= 0)
		{
			closesocket(s);
			return false;
		}

		char buf[2048];
		int n;
		while ((n = recv(s, buf, sizeof(buf), 0)) > 0)
			body.Append(buf, n);
		closesocket(s);

		const int headerEnd = body.Find("\r\n\r\n");
		const CStringA head = (headerEnd >= 0) ? body.Left(headerEnd) : body;
		return head.Find(" 200") >= 0;
	}

	bool LooksLikeGo2rtc(const CStringA& body)
	{
		return body.Find("\"rtsp\"") >= 0 ||
			body.Find("go2rtc") >= 0 ||
			body.Find("\"streams\"") >= 0 ||
			body.Find("\"host\"") >= 0;
	}

	CString CamerasJsonPath()
	{
		return T3000Nvr_GetConfigDir() + _T("\\cameras.json");
	}

	CString ReadTextFile(const CString& path)
	{
		CFile f;
		if (!f.Open(path, CFile::modeRead | CFile::shareDenyWrite))
			return CString();
		const ULONGLONG len = f.GetLength();
		if (len == 0 || len > 2 * 1024 * 1024)
		{
			f.Close();
			return CString();
		}
		std::string raw((size_t)len, 0);
		f.Read(&raw[0], (UINT)len);
		f.Close();
		return Utf8ToCString(raw);
	}

	bool WriteTextFile(const CString& path, const CString& text)
	{
		CFile f;
		if (!f.Open(path, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite))
			return false;
		const std::string raw = CStringToUtf8(text);
		if (!raw.empty())
			f.Write(raw.data(), (UINT)raw.size());
		f.Close();
		return true;
	}

	CString StreamsYamlFromCamerasJson()
	{
		const CString jsonText = ReadTextFile(CamerasJsonPath());
		if (jsonText.IsEmpty())
			return CString();

		Json::Value root;
		Json::Reader reader;
		if (!reader.parse(CStringToUtf8(jsonText), root, false))
			return CString();

		const Json::Value cams = root.isMember("cameras") ? root["cameras"] : root;
		if (!cams.isArray())
			return CString();

		CString yaml;
		for (Json::ArrayIndex i = 0; i < cams.size(); ++i)
		{
			const Json::Value& c = cams[i];
			CString name = Utf8ToCString(c.get("name", "").asString());
			CString src = Utf8ToCString(c.get("src", "").asString());
			if (name.IsEmpty() || src.IsEmpty())
				continue;
			for (int k = 0; k < name.GetLength(); ++k)
			{
				const TCHAR ch = name[k];
				const bool ok = (ch >= _T('A') && ch <= _T('Z')) ||
					(ch >= _T('a') && ch <= _T('z')) ||
					(ch >= _T('0') && ch <= _T('9')) ||
					ch == _T('_') || ch == _T('-');
				if (!ok)
					name.SetAt(k, _T('_'));
			}
			yaml += _T("  ") + name + _T(": ") + YamlQuote(src) + _T("\r\n");
		}
		return yaml;
	}

	void WriteGo2rtcYaml(const CString& configDir, const CString& staticDir)
	{
		CString streams = StreamsYamlFromCamerasJson();
		CString yaml;
		yaml += _T("# Generated by T3000. Bind localhost only.\r\n");
		yaml += _T("# Live/discover: go2rtc. Record/playback hook: MediaMTX (separate process).\r\n");
		yaml += _T("api:\r\n");
		yaml.AppendFormat(_T("  listen: \"127.0.0.1:%d\"\r\n"), T3000_NVR_GO2RTC_API_PORT);
		yaml += _T("  origin: \"*\"\r\n");
		yaml += _T("  static_dir: ") + YamlQuote(staticDir) + _T("\r\n");
		yaml += _T("rtsp:\r\n");
		yaml.AppendFormat(_T("  listen: \"127.0.0.1:%d\"\r\n"), T3000_NVR_GO2RTC_RTSP_PORT);
		yaml += _T("webrtc:\r\n");
		yaml.AppendFormat(_T("  listen: \"127.0.0.1:%d\"\r\n"), T3000_NVR_GO2RTC_WEBRTC_PORT);
		yaml += _T("log:\r\n");
		yaml += _T("  level: info\r\n");
		if (streams.IsEmpty())
			yaml += _T("streams: {}\r\n");
		else
		{
			yaml += _T("streams:\r\n");
			yaml += streams;
		}
		WriteTextFile(configDir + _T("\\go2rtc.yaml"), yaml);
	}

	void WriteMediamtxYml(const CString& configDir)
	{
		const CString recDir = configDir + _T("\\recordings");
		EnsureDir(recDir);
		CString yml;
		yml += _T("# Generated by T3000. v1 live view uses go2rtc; this file is the record/playback home.\r\n");
		yml += _T("logLevel: info\r\n");
		yml += _T("api: yes\r\n");
		yml.AppendFormat(_T("apiAddress: 127.0.0.1:%d\r\n"), T3000_NVR_MEDIAMTX_API_PORT);
		yml += _T("rtsp: yes\r\n");
		yml.AppendFormat(_T("rtspAddress: 127.0.0.1:%d\r\n"), T3000_NVR_MEDIAMTX_RTSP_PORT);
		yml += _T("playback: yes\r\n");
		yml.AppendFormat(_T("playbackAddress: 127.0.0.1:%d\r\n"), T3000_NVR_MEDIAMTX_PLAY_PORT);
		yml += _T("rtmp: no\r\n");
		yml += _T("hls: no\r\n");
		yml += _T("webrtc: no\r\n");
		yml += _T("srt: no\r\n");
		yml += _T("pathDefaults:\r\n");
		yml += _T("  source: publisher\r\n");
		yml += _T("  # record: yes\r\n");
		yml += _T("  # recordPath: ") + YamlQuote(recDir + _T("\\%path\\%Y-%m-%d_%H-%M-%S-%f")) + _T("\r\n");
		WriteTextFile(configDir + _T("\\mediamtx.yml"), yml);
	}

	void CopyUiIntoConfigWww(const CString& configDir)
	{
		const CString www = configDir + _T("\\www");
		EnsureDir(www);
		const CString src = ExeDir() + _T("\\ResourceFile\\nvr\\index.html");
		const CString dst = www + _T("\\index.html");
		if (FileExists(src))
			CopyFile(src, dst, FALSE);
	}

	void EnsureJobObject()
	{
		if (g_job)
			return;
		g_job = CreateJobObject(NULL, _T("T3000NVRSidecars"));
		if (!g_job)
			return;
		JOBOBJECT_EXTENDED_LIMIT_INFORMATION info = {};
		info.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
		SetInformationJobObject(g_job, JobObjectExtendedLimitInformation, &info, sizeof(info));
	}

	bool StartHiddenProcess(const CString& exe, const CString& cmdLine, const CString& workDir, HANDLE& outHandle)
	{
		STARTUPINFO si = {};
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
		PROCESS_INFORMATION pi = {};

		CString writable = cmdLine;
		LPTSTR buf = writable.GetBuffer(writable.GetLength() + 1);
		DWORD flags = CREATE_NO_WINDOW | CREATE_BREAKAWAY_FROM_JOB;
		BOOL ok = CreateProcess(
			exe,
			buf,
			NULL,
			NULL,
			FALSE,
			flags,
			NULL,
			workDir.IsEmpty() ? NULL : workDir.GetString(),
			&si,
			&pi);
		if (!ok)
		{
			ok = CreateProcess(
				exe,
				buf,
				NULL,
				NULL,
				FALSE,
				CREATE_NO_WINDOW,
				NULL,
				workDir.IsEmpty() ? NULL : workDir.GetString(),
				&si,
				&pi);
		}
		writable.ReleaseBuffer();
		if (!ok)
			return false;

		EnsureJobObject();
		if (g_job)
			AssignProcessToJobObject(g_job, pi.hProcess);

		outHandle = pi.hProcess;
		CloseHandle(pi.hThread);
		return true;
	}

	bool WaitForHttp(int port, const char* path, int timeoutMs)
	{
		const DWORD start = GetTickCount();
		CStringA body;
		while ((GetTickCount() - start) < (DWORD)timeoutMs)
		{
			if (HttpGetLocal(port, path, body, 400))
				return true;
			Sleep(200);
		}
		return false;
	}

	void CloseOwned(HANDLE& h, bool& owned)
	{
		if (owned && h)
		{
			TerminateProcess(h, 0);
			WaitForSingleObject(h, 2000);
		}
		if (h)
		{
			CloseHandle(h);
			h = NULL;
		}
		owned = false;
	}

	CString OfflineViewUrl(const CString& reason)
	{
		CString html = ExeDir() + _T("\\ResourceFile\\nvr\\index.html");
		if (!FileExists(html))
			html = T3000Nvr_GetConfigDir() + _T("\\www\\index.html");
		CString url = FileUri(html);
		url += _T("?offline=1&reason=");
		url += UrlEncode(reason);
		url.AppendFormat(_T("&go2rtcPort=%d&mediamtxApi=%d"),
			T3000_NVR_GO2RTC_API_PORT, T3000_NVR_MEDIAMTX_API_PORT);
		return url;
	}

	CString OnlineViewUrl(bool mediamtxOk)
	{
		CString url;
		url.Format(
			_T("http://127.0.0.1:%d/index.html?go2rtc=1&mediamtx=%d&api=http://127.0.0.1:%d&mtx=http://127.0.0.1:%d"),
			T3000_NVR_GO2RTC_API_PORT,
			mediamtxOk ? 1 : 0,
			T3000_NVR_GO2RTC_API_PORT,
			T3000_NVR_MEDIAMTX_API_PORT);
		return url;
	}
}

CString T3000Nvr_GetConfigDir()
{
	PWSTR localApp = NULL;
	CString dir;
	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &localApp)) && localApp)
	{
		dir = CString(localApp) + _T("\\T3000");
		EnsureDir(dir);
		dir += _T("\\nvr");
		EnsureDir(dir);
		CoTaskMemFree(localApp);
	}
	else
	{
		dir = ExeDir() + _T("\\nvr-config");
		EnsureDir(dir);
	}
	EnsureDir(dir + _T("\\www"));
	EnsureDir(dir + _T("\\recordings"));
	return dir;
}

CString T3000Nvr_FindSidecarExe(LPCTSTR fileName)
{
	const CString exeDir = ExeDir();
	// Runtime next to T3000.exe first, then the source-tree sidecar folder so a
	// teammate can drop go2rtc.exe in T3000\sidecar\ without a rebuild.
	const CString candidates[] = {
		exeDir + _T("\\sidecar\\") + fileName,
		exeDir + _T("\\") + fileName,
		exeDir + _T("\\nvr\\") + fileName,
		exeDir + _T("\\..\\..\\T3000\\sidecar\\") + fileName,
		exeDir + _T("\\..\\T3000\\sidecar\\") + fileName,
	};
	for (int i = 0; i < 5; ++i)
	{
		TCHAR full[MAX_PATH] = { 0 };
		if (GetFullPathName(candidates[i], MAX_PATH, full, NULL) == 0)
			continue;
		if (FileExists(full))
			return CString(full);
	}
	return CString();
}

T3000NvrLaunchResult T3000Nvr_StartSidecars()
{
	T3000NvrLaunchResult r = {};
	const CString configDir = T3000Nvr_GetConfigDir();
	CopyUiIntoConfigWww(configDir);
	WriteGo2rtcYaml(configDir, configDir + _T("\\www"));
	WriteMediamtxYml(configDir);

	CStringA probe;
	const bool port9191 = TcpPortOpen(T3000_NVR_GO2RTC_API_PORT, 200);
	if (port9191)
	{
		if (HttpGetLocal(T3000_NVR_GO2RTC_API_PORT, "/api", probe, 800) && LooksLikeGo2rtc(probe))
		{
			r.go2rtcOk = true;
			r.go2rtcAlreadyRunning = true;
		}
		else
		{
			r.go2rtcPortBusy = true;
			r.message = _T("Port 127.0.0.1:9191 is in use by another program. Close it or change the T3000 NVR ports.");
			r.viewUrl = OfflineViewUrl(r.message);
			return r;
		}
	}

	if (!r.go2rtcOk)
	{
		const CString go2rtc = T3000Nvr_FindSidecarExe(_T("go2rtc.exe"));
		if (go2rtc.IsEmpty())
		{
			r.go2rtcMissing = true;
			r.message.Format(
				_T("go2rtc.exe was not found. T3000 is running normally.\r\n\r\n")
				_T("Drop go2rtc.exe here and reopen Tools > Cameras (NVR):\r\n")
				_T("  %s\\sidecar\\go2rtc.exe\r\n")
				_T("  %s\\go2rtc.exe\r\n")
				_T("or in the source tree: T3000\\sidecar\\go2rtc.exe\r\n\r\n")
				_T("Windows zip: https://github.com/AlexxIT/go2rtc/releases/download/v1.9.14/go2rtc_win64.zip\r\n")
				_T("Step-by-step: DEMO.md"),
				ExeDir().GetString(), ExeDir().GetString());
			r.viewUrl = OfflineViewUrl(r.message);
		}
		else
		{
			CString cmd;
			cmd.Format(_T("\"%s\" -c \"%s\\go2rtc.yaml\""), go2rtc.GetString(), configDir.GetString());
			if (!StartHiddenProcess(go2rtc, cmd, configDir, g_go2rtcProcess))
			{
				r.message.Format(_T("Failed to start go2rtc.exe (Win32 error %u)."), GetLastError());
				r.viewUrl = OfflineViewUrl(r.message);
			}
			else
			{
				g_ownGo2rtc = true;
				if (WaitForHttp(T3000_NVR_GO2RTC_API_PORT, "/api", 10000))
				{
					r.go2rtcOk = true;
				}
				else
				{
					r.message = _T("go2rtc.exe started but its API on 127.0.0.1:9191 did not become ready.");
					r.viewUrl = OfflineViewUrl(r.message);
				}
			}
		}
	}

	const bool port9197 = TcpPortOpen(T3000_NVR_MEDIAMTX_API_PORT, 200);
	if (port9197)
	{
		r.mediamtxOk = true;
		r.mediamtxAlreadyRunning = true;
	}
	else
	{
		const CString mtx = T3000Nvr_FindSidecarExe(_T("mediamtx.exe"));
		if (mtx.IsEmpty())
		{
			r.mediamtxMissing = true;
		}
		else
		{
			CString cmd;
			cmd.Format(_T("\"%s\" \"%s\\mediamtx.yml\""), mtx.GetString(), configDir.GetString());
			if (StartHiddenProcess(mtx, cmd, configDir, g_mediamtxProcess))
			{
				g_ownMediamtx = true;
				r.mediamtxOk = WaitForHttp(T3000_NVR_MEDIAMTX_API_PORT, "/v3/config/global/get", 8000)
					|| TcpPortOpen(T3000_NVR_MEDIAMTX_API_PORT, 200);
			}
		}
	}

	if (r.go2rtcOk)
	{
		r.viewUrl = OnlineViewUrl(r.mediamtxOk);
		if (r.message.IsEmpty())
		{
			if (r.go2rtcAlreadyRunning)
				r.message = _T("Using go2rtc already listening on 127.0.0.1:9191.");
			else
				r.message = _T("Cameras sidecar started (go2rtc on 127.0.0.1:9191).");
			if (r.mediamtxMissing)
				r.message += _T(" MediaMTX is not installed yet (optional for live view).");
		}
	}

	return r;
}

void T3000Nvr_StopSidecars()
{
	CloseOwned(g_go2rtcProcess, g_ownGo2rtc);
	CloseOwned(g_mediamtxProcess, g_ownMediamtx);
}

void T3000Nvr_HandleWebMessage(const CString& msg, CString& outmsg)
{
	Json::Value json;
	Json::Reader reader;
	if (!reader.parse(CStringToUtf8(msg), json, false))
	{
		outmsg = _T("{\"type\":\"nvr\",\"ok\":false,\"error\":\"invalid json\"}");
		return;
	}

	const std::string cmd = json.get("cmd", "").asString();
	Json::Value reply;
	reply["type"] = "nvr";
	reply["ok"] = true;
	if (json.isMember("msgId"))
		reply["msgId"] = json["msgId"];

	if (cmd == "saveCameras")
	{
		Json::Value store;
		store["cameras"] = json["cameras"];
		Json::StreamWriterBuilder wb;
		wb["indentation"] = "  ";
		const std::string raw = Json::writeString(wb, store);
		if (!WriteTextFile(CamerasJsonPath(), Utf8ToCString(raw)))
		{
			reply["ok"] = false;
			reply["error"] = "failed to write cameras.json";
		}
		else
		{
			WriteGo2rtcYaml(T3000Nvr_GetConfigDir(), T3000Nvr_GetConfigDir() + _T("\\www"));
			reply["path"] = CStringToUtf8(CamerasJsonPath());
		}
	}
	else if (cmd == "loadCameras")
	{
		const CString text = ReadTextFile(CamerasJsonPath());
		Json::Value stored;
		if (!text.IsEmpty())
			reader.parse(CStringToUtf8(text), stored, false);
		reply["cameras"] = stored.isMember("cameras") ? stored["cameras"] : Json::Value(Json::arrayValue);
	}
	else if (cmd == "status")
	{
		CStringA body;
		reply["go2rtc"] = HttpGetLocal(T3000_NVR_GO2RTC_API_PORT, "/api", body, 500);
		reply["mediamtx"] = TcpPortOpen(T3000_NVR_MEDIAMTX_API_PORT, 200);
		reply["go2rtcPort"] = T3000_NVR_GO2RTC_API_PORT;
		reply["mediamtxPort"] = T3000_NVR_MEDIAMTX_API_PORT;
		reply["configDir"] = CStringToUtf8(T3000Nvr_GetConfigDir());
		reply["go2rtcExe"] = CStringToUtf8(T3000Nvr_FindSidecarExe(_T("go2rtc.exe")));
		reply["mediamtxExe"] = CStringToUtf8(T3000Nvr_FindSidecarExe(_T("mediamtx.exe")));
	}
	else
	{
		reply["ok"] = false;
		reply["error"] = "unknown nvr cmd";
	}

	Json::StreamWriterBuilder wb;
	wb["indentation"] = "";
	outmsg = Utf8ToCString(Json::writeString(wb, reply));
}
