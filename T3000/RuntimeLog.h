#pragma once

#include <deque>
#include <mutex>

// Ring buffer of the last things the app did, so a bug report can carry the
// "what did you click just before this" that people never type.
//
// Header-only on purpose: a new .cpp would have to be added to every vcxproj
// variant (VS2015 / VS2019 / Platform Version) for no gain.
class CRuntimeLog
{
public:
	enum
	{
		MAX_ENTRIES = 200,
		MAX_LINE = 400,
		MAX_FILE_BYTES = 1024 * 1024
	};

	static CRuntimeLog& Instance()
	{
		static CRuntimeLog theLog;
		return theLog;
	}

	// area is a short tag shown in the report: nav, cmd, status, err, dlg, report, ui
	void Add(LPCTSTR area, LPCTSTR text)
	{
		if (area == NULL || text == NULL) { return; }

		SYSTEMTIME st;
		::GetLocalTime(&st);

		CString line;
		line.Format(_T("[%02u:%02u:%02u.%03u] %-6s %s"),
			(unsigned)st.wHour, (unsigned)st.wMinute, (unsigned)st.wSecond,
			(unsigned)st.wMilliseconds, area, text);
		if (line.GetLength() > MAX_LINE)
		{
			line = line.Left(MAX_LINE);
		}
		line += _T("\r\n");

		std::lock_guard<std::mutex> guard(m_lock);
		m_entries.push_back(line);
		while ((int)m_entries.size() > MAX_ENTRIES)
		{
			m_entries.pop_front();
		}
	}

	// The last count lines as one string, oldest first. Capped for the wire.
	CString Tail(int count) const
	{
		std::lock_guard<std::mutex> guard(m_lock);
		CString out;
		const int n = (int)m_entries.size();
		const int from = (count <= 0 || count > n) ? 0 : n - count;
		for (int i = from; i < n; i++)
		{
			out += m_entries[i];
		}
		return out;
	}

	int Count() const
	{
		std::lock_guard<std::mutex> guard(m_lock);
		return (int)m_entries.size();
	}

	// Mirror the buffer to <exe folder>\T3000-runtime.log, rotated at 1 MB.
	CString Save() const
	{
		TCHAR exePath[MAX_PATH] = { 0 };
		if (::GetModuleFileName(NULL, exePath, MAX_PATH) == 0) { return CString(); }

		CString path(exePath);
		const int slash = path.ReverseFind(_T('\\'));
		if (slash < 0) { return CString(); }
		path = path.Left(slash + 1) + _T("T3000-runtime.log");

		const CString rotated = path + _T(".1");
		WIN32_FILE_ATTRIBUTE_DATA fad;
		if (::GetFileAttributesEx(path, GetFileExInfoStandard, &fad))
		{
			const ULONGLONG bytes = ((ULONGLONG)fad.nFileSizeHigh << 32) | fad.nFileSizeLow;
			if (bytes > (ULONGLONG)MAX_FILE_BYTES)
			{
				::DeleteFile(rotated);
				::MoveFile(path, rotated);
			}
		}

		const CString body = Tail(0);
		HANDLE hFile = ::CreateFile(path, GENERIC_WRITE, FILE_SHARE_READ, NULL,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { return CString(); }

		DWORD written = 0;
#ifdef UNICODE
		const CStringA utf8(body);
		::WriteFile(hFile, (LPCSTR)utf8, (DWORD)utf8.GetLength(), &written, NULL);
#else
		::WriteFile(hFile, (LPCSTR)body, (DWORD)body.GetLength(), &written, NULL);
#endif
		::CloseHandle(hFile);
		return path;
	}

private:
	CRuntimeLog() {}
	CRuntimeLog(const CRuntimeLog&);
	CRuntimeLog& operator=(const CRuntimeLog&);

	mutable std::mutex m_lock;
	std::deque<CString> m_entries;
};

#define RUNTIME_LOG(area, text) CRuntimeLog::Instance().Add(_T(area), text)
