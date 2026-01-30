#include <afxwin.h>
#include "resource.h"
#include <Shellapi.h>
#include <ShlObj_core.h>
#include <functional>
#include <stdexcept>
#include <regex>
#include <gdiplus.h>

#include <wil/com.h>
#include <wrl.h>
#include <json/json.h>

#include "stdafx.h"
#include "T3000.h"
#include "global_function.h"

#include "BacnetInput.h"
#include "BacnetOutput.h"
#include "BacnetVariable.h"
#include "dirent.h"
#include "BacnetProgram.h"
#include "BacnetWeeklyRoutine.h"
#include "BacnetAnnualRoutine.h"
#include "BacnetWebView.h"
#include "MainFrm.h"
#include "JsonHead.h"
extern "C" {
	enum RustError {
		Ok = 0,
		Error = 1,
	};

	RustError run_server();
}
using namespace Microsoft::WRL;
size_t thread_local BacnetWebViewAppWindow::s_appInstances = 0;
char* base64_decode(char const* base64Str, char* debase64Str, int encodeStrLen);
extern int Read_Webview_Data_Special(int panelid, UINT nserialnumber, int nscreenindex);
extern int Write_Webview_Data_Special(int panelid, UINT nserialnumber, int nscreenindex, int element_count);

// ❌ Set to false to disable all T3WebLog logging
static bool enable_t3_web_logging = true;

//enum WEBVIEW_MESSAGE_TYPE
//{
//    READ_VARIABLES = 0,
//    READ_OUTPUT_VARIABLES = 1,
//    READ_DEVICE_VARIABLES = 2,
//    READ_DATA = 3 ,
//    UPDATE_DATA = 4,
//    SAVE_GRAPHIC = 5 ,
//    LIST_GRAPHICS = 6,
//    LOAD_GRAPHICS = 7,
//    TRIGGER_T3000_DLG = 8,
//    ADD_VAR = 9,
//    LOAD_BG_IMG = 10,
//
//};

enum WEBVIEW_MESSAGE_TYPE
{
	GET_PANEL_DATA = 0,
	GET_INITIAL_DATA = 1,
	SAVE_GRAPHIC_DATA = 2,
	UPDATE_ENTRY = 3,
	GET_PANELS_LIST = 4,
	GET_PANEL_RANGE_INFO = 5,
	GET_ENTRIES = 6,
	LOAD_GRAPHIC_ENTRY = 7,
	OPEN_ENTRY_EDIT_WINDOW = 8,
	SAVE_IMAGE = 9,
	SAVE_LIBRAY_DATA = 10,
	DELETE_IMAGE = 11,
	GET_SELECTED_DEVICE_INFO = 12,
	BIND_DEVICE = 13,
	SAVE_NEW_LIBRARY_DATA = 14,
	LOGGING_DATA = 15,
	UPDATE_WEBVIEW_LIST = 16,
	GET_WEBVIEW_LIST = 17
};

#define READ_INPUT_VARIABLE  0
#define READ_OUTPUT_VARIABLE 1
#define READ_VARIABLE 2
#define READ_SCHEDULE 3
#define READ_CALENDER 4
#define READ_PROGRAM 5

int save_button_click = 0;
extern char* ispoint_ex(char* token, int* num_point, byte* var_type, byte* point_type, int* num_panel, int* num_net, int network, unsigned char& sub_panel, byte panel, int* netpresent);

static inline bool IsNullOrEmptyOrWhitespace(const char* s) noexcept
{
	if (s == nullptr) return true;
	const unsigned char* us = reinterpret_cast<const unsigned char*>(s);
	while (*us != '\0') {
		if (!isspace(*us)) return false;
		++us;
	}
	return true;
}

void DeleteDirectoryRecursive(const std::wstring& dir_path) {
	std::wstring file_search_path = dir_path + L"\\*";
	WIN32_FIND_DATAW find_data;
	HANDLE hFind = FindFirstFileW(file_search_path.c_str(), &find_data);

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (wcscmp(find_data.cFileName, L".") != 0 && wcscmp(find_data.cFileName, L"..") != 0) {
					// This is a directory, so we need to recurse into it
					DeleteDirectoryRecursive(dir_path + L"\\" + find_data.cFileName);
				}
			}
			else {
				// This is a file, so delete it
				DeleteFileW((dir_path + L"\\" + find_data.cFileName).c_str());
			}
		} while (FindNextFileW(hFind, &find_data) != 0);

		FindClose(hFind);

		// Now that all the files in the directory are deleted, delete the directory itself
		RemoveDirectoryW(dir_path.c_str());
	}
}

extern CBacnetProgram* Program_Window;
extern BacnetWeeklyRoutine* WeeklyRoutine_Window;
extern BacnetAnnualRoutine* AnnualRoutine_Window;
BacnetWebViewAppWindow::BacnetWebViewAppWindow(
	UINT creationModeId,
	std::wstring initialUri,
	std::wstring title,
	bool isMainWindow,
	std::function<void()> webviewCreatedCallback,
	bool customWindowRect,
	RECT windowRect,
	bool shouldHaveToolbar)
	: m_creationModeId(creationModeId),
	m_initialUri(initialUri),
	m_onWebViewFirstInitialized(webviewCreatedCallback)
{
	if (FAILED(OleInitialize(NULL)))
		throw std::runtime_error("Could not initialize COM!");
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	++s_appInstances;
	m_mainWindow = CreateWindowExW(0, GetWindowClass(), title.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, BacNet_hwd, nullptr,
		GetModuleHandle(nullptr), nullptr);

	/*
	if (initialUri != L"")
	{
		Gdiplus::Bitmap bitmap(initialUri.c_str(), false);
		bitmap.GetHBITMAP(0, &m_appBackgroundImageHandle);
		GetObject(m_appBackgroundImageHandle, sizeof(m_appBackgroundImage), &m_appBackgroundImage);
		m_memHdc = CreateCompatibleDC(GetDC(m_mainWindow));
		SelectObject(m_memHdc, m_appBackgroundImageHandle);
	}
	*/

	if (shouldHaveToolbar)
		m_toolbar.Initialize(this);



	SetWindowLongPtr(m_mainWindow, GWLP_USERDATA, (LONG_PTR)this);
	ShowWindow(m_mainWindow, SW_SHOWDEFAULT);
	UpdateWindow(m_mainWindow);

	wil::unique_cotaskmem_string version_info;
	HRESULT hr = GetAvailableCoreWebView2BrowserVersionString(nullptr, &version_info);
	if (hr == S_OK && version_info != nullptr)
	{
		InitializeWebView();
	}
	save_button_click = 0;
}



PCWSTR BacnetWebViewAppWindow::GetWindowClass()
{
	// Only do this once
	static PCWSTR windowClass = []() {
		static std::wstring windowClass = L"BacnetWebViewWindowClass";

		WNDCLASSEX wcx;
		wcx.cbSize = sizeof(wcx);
		wcx.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wcx.lpfnWndProc = WndProcStatic;
		wcx.cbClsExtra = 0;
		wcx.cbWndExtra = 0;
		wcx.hInstance = GetModuleHandle(nullptr);
		wcx.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
		wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wcx.lpszMenuName = L"MainMenu";
		wcx.lpszClassName = windowClass.c_str();
		wcx.hIconSm = (HICON)LoadImage(GetModuleHandle(nullptr),
			MAKEINTRESOURCE(5),
			IMAGE_ICON,
			GetSystemMetrics(SM_CXSMICON),
			GetSystemMetrics(SM_CYSMICON),
			LR_DEFAULTCOLOR);

		// Register the window class. 
		RegisterClassEx(&wcx);
		return windowClass.c_str();
		}();

	return windowClass;
}

LRESULT CALLBACK BacnetWebViewAppWindow::WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (auto app = (BacnetWebViewAppWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA))
	{
		LRESULT result = 0;
		if (app->HandleWindowMessage(hWnd, message, wParam, lParam, &result))
		{
			return result;
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

// This is purely for proof of concept. I am not going to tie MFC into
// a webview2 application.
#include <afxdlgs.h>
bool BacnetWebViewAppWindow::HandleWindowMessage(
	HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* result)
{
	switch (message)
	{
	case WM_LBUTTONDBLCLK:
	{
		const TCHAR szFilter[] = _T("HTML File (*.html)|*.html");

		CFileDialog dlg(1, _T("html"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			szFilter, CWnd::FromHandle(hWnd));
		if (dlg.DoModal() == IDOK)
		{
			CString sFilePath = dlg.GetPathName();
			m_initialUri = sFilePath;
			wil::com_ptr<IUri> uri;
			CreateUri(m_initialUri.c_str(), Uri_CREATE_ALLOW_IMPLICIT_FILE_SCHEME, 0, &uri);

			wil::unique_bstr uriBstr;
			uri->GetAbsoluteUri(&uriBstr);
			auto hr = m_webView->Navigate(uriBstr.get());
			if (SUCCEEDED(hr))
				UpdateWindow(m_mainWindow);
		}
		break;
	}
	case WM_SIZE:
	{
		// Don't resize the app or webview when the app is minimized
		// let WM_SYSCOMMAND to handle it
		if (lParam != 0)
		{
			if (m_controller)
			{
				RECT bounds;
				GetClientRect(m_mainWindow, &bounds);
				m_controller->put_Bounds(bounds);
			}

			ResizeEverything();
			return true;
		}
	}
	break;
	case WM_PAINT:
	{
		//if (m_webView)
		// m_webView->PostWebMessageAsJson(L"{\"SetInput\":{\"id\":\"IN2\",\"value\":\"On\"}}");
		PAINTSTRUCT ps;
		HDC hdc;
		RECT mainWindowBounds;
		RECT webViewBounds = { 0 };
		BeginPaint(hWnd, &ps);

		hdc = GetDC(hWnd);
		GetClientRect(hWnd, &mainWindowBounds);
		StretchBlt(hdc, mainWindowBounds.left, mainWindowBounds.top, mainWindowBounds.right, mainWindowBounds.bottom,
			m_memHdc, 0, 0, m_appBackgroundImage.bmWidth, m_appBackgroundImage.bmHeight, SRCCOPY);

		EndPaint(hWnd, &ps);
		return true;
	}
	break;
	case WM_NCDESTROY:
	{
		if (Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub >= WEBVIEW_JSON_FEATURE) //643 版本会有这个功能
		{
			::PostMessage(m_screen_dlg_hwnd, HANDLE_JSON_DATA, NULL, NULL);
		}

		int retValue = 0;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, NULL);
		NotifyClosed();
		if (--s_appInstances == 0)
		{
			PostQuitMessage(retValue);
		}
		//DeleteObject(m_appBackgroundImageHandle);
		DeleteDC(m_memHdc);
		//FreeConsole(); //关闭控制台
	}
	break;
	//! [RestartManager]
	case WM_QUERYENDSESSION:
	{
		// yes, we can shut down
		// Register how we might be restarted
		RegisterApplicationRestart(L"--restore", RESTART_NO_CRASH | RESTART_NO_HANG);
		*result = TRUE;
		return true;
	}
	break;
	case WM_ENDSESSION:
	{
		if (wParam == TRUE)
		{
			// save app state and exit.
			PostQuitMessage(0);
			return true;
		}
	}
	break;
	case WM_COMMAND:
	{
		return ExecuteWebViewCommands(wParam, lParam) || ExecuteAppCommands(wParam, lParam);
	}
	break;
	}
	return false;
}

bool BacnetWebViewAppWindow::ExecuteWebViewCommands(WPARAM wParam, LPARAM lParam)
{
	if (!m_webView)
		return false;
	switch (LOWORD(wParam))
	{
	case IDM_GET_BROWSER_VERSION_AFTER_CREATION:
	{
		//! [GetBrowserVersionString]
		wil::unique_cotaskmem_string version_info;
		m_webViewEnvironment->get_BrowserVersionString(&version_info);
		MessageBox(
			m_mainWindow, version_info.get(), L"Browser Version Info After WebView Creation",
			MB_OK);
		//! [GetBrowserVersionString]
		return true;
	}
	case IDM_CLOSE_WEBVIEW:
	{
		CloseWebView();
		return true;
	}
	case IDM_CLOSE_WEBVIEW_CLEANUP:
	{
		CloseWebView(true);
		return true;
	}
	case IDM_SCENARIO_POST_WEB_MESSAGE:
	{
		//NewComponent<ScenarioWebMessage>(this);
		return true;
	}
	case IDM_SCENARIO_ADD_HOST_OBJECT:
	{
		//NewComponent<ScenarioAddHostObject>(this);
		return true;
	}
	case IDM_SCENARIO_WEB_VIEW_EVENT_MONITOR:
	{
		//NewComponent<ScenarioWebViewEventMonitor>(this);
		return true;
	}
	case IDM_SCENARIO_JAVA_SCRIPT:
	{
		WCHAR c_scriptPath[] = L"ScenarioJavaScriptDebugIndex.html";
		std::wstring m_scriptUri = GetLocalUri(c_scriptPath);
		m_webView->Navigate(m_scriptUri.c_str());
		return true;
	}
	case IDM_SCENARIO_TYPE_SCRIPT:
	{
		WCHAR c_scriptPath[] = L"ScenarioTypeScriptDebugIndex.html";
		std::wstring m_scriptUri = GetLocalUri(c_scriptPath);
		m_webView->Navigate(m_scriptUri.c_str());
	}
	case IDM_SCENARIO_AUTHENTICATION:
	{
		//NewComponent<ScenarioAuthentication>(this);

		return true;
	}
	case IDM_SCENARIO_COOKIE_MANAGEMENT:
	{
		//NewComponent<ScenarioCookieManagement>(this);
		return true;
	}
	case IDM_SCENARIO_DOM_CONTENT_LOADED:
	{
		//NewComponent<ScenarioDOMContentLoaded>(this);
		return true;
	}
	case IDM_SCENARIO_NAVIGATEWITHWEBRESOURCEREQUEST:
	{
		//NewComponent<ScenarioNavigateWithWebResourceRequest>(this);
		return true;
	}
	}
	return false;
}

bool BacnetWebViewAppWindow::ExecuteAppCommands(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_GET_BROWSER_VERSION_BEFORE_CREATION:
	{
		wil::unique_cotaskmem_string version_info;
		GetAvailableCoreWebView2BrowserVersionString(
			nullptr,
			&version_info);
		MessageBox(
			m_mainWindow, version_info.get(), L"Browser Version Info Before WebView Creation",
			MB_OK);
		return true;
	}
	case IDM_EXIT:
		CloseAppWindow();
		return true;
	}
	return false;
}

void BacnetWebViewAppWindow::AddRef()
{
	InterlockedIncrement((LONG*)&m_refCount);
}

void BacnetWebViewAppWindow::Release()
{
	uint32_t refCount = InterlockedDecrement((LONG*)&m_refCount);
	if (refCount == 0)
	{
		delete this;
	}
}

void BacnetWebViewAppWindow::NotifyClosed()
{
	//对话框窗体大小及其屏幕坐标
	//CRect rectDlg;
	////GetClientRect(rectDlg);//获得窗体的大小 //法1：
	//::GetWindowRect(this,;//获得窗体在屏幕上的位置 //法2：
	//ScreenToClient(rectDlg);

	//printf(“窗口位置大小:底: % d, 右 : % d, 宽 : % d, 高 : % d\r\n”, rectDlg.bottom, rectDlg.right, rectDlg.Width(), rectDlg.Height());
	CloseWebView(true);
	m_isClosed = true;
}

void BacnetWebViewAppWindow::SetTitleText(PCWSTR titleText)
{
	if (m_mainWindow != nullptr)
	{
		SetWindowTextW(m_mainWindow, titleText);
	}
}

std::wstring BacnetWebViewAppWindow::GetLocalUri(std::wstring relativePath)
{
	if (m_webView3)
	{
		//! [LocalUrlUsage]
		const std::wstring localFileRootUrl = L"https://appassets.example/";
		return localFileRootUrl + regex_replace(relativePath, std::wregex(L"\\\\"), L"/");
		//! [LocalUrlUsage]
	}
	else
	{
		std::wstring path = GetLocalPath(L"assets\\" + relativePath, false);

		wil::com_ptr<IUri> uri;
		CreateUri(path.c_str(), Uri_CREATE_ALLOW_IMPLICIT_FILE_SCHEME, 0, &uri);

		wil::unique_bstr uriBstr;
		uri->GetAbsoluteUri(&uriBstr);
		return std::wstring(uriBstr.get());
	}
}

std::wstring BacnetWebViewAppWindow::GetLocalPath(std::wstring relativePath, bool keep_exe_path)
{
	WCHAR rawPath[MAX_PATH];
	GetModuleFileNameW(GetModuleHandle(nullptr), rawPath, MAX_PATH);
	std::wstring path(rawPath);
	if (keep_exe_path)
	{
		path.append(relativePath);
	}
	else
	{
		std::size_t index = path.find_last_of(L"\\") + 1;
		path.replace(index, path.length(), relativePath);
	}
	return path;
}

void BacnetWebViewAppWindow::ResizeEverything()
{
	/*   RECT availableBounds = { 0 };
	   GetClientRect(m_mainWindow, &availableBounds);

	   if (!m_containsFullscreenElement)
	   {
		   availableBounds = m_toolbar.Resize(availableBounds);
	   }

	   if (auto view = GetComponent<ViewComponent>())
	   {
		   view->SetBounds(availableBounds);
	   }*/
}

//! [Close]
// Close the WebView and deinitialize related state. This doesn't close the app window.
void BacnetWebViewAppWindow::CloseWebView(bool cleanupUserDataFolder)
{
	//DeleteAllComponents();
	if (m_controller)
	{
		m_controller->Close();
		m_controller = nullptr;
		m_webView = nullptr;
	}

	Gdiplus::GdiplusShutdown(m_gdiplusToken);
	m_webViewEnvironment = nullptr;
	if (cleanupUserDataFolder)
	{
		// For non-UWP apps, the default user data folder {Executable File Name}.WebView2
		// is in the same directory next to the app executable. If end
		// developers specify userDataFolder during WebView environment
		// creation, they would need to pass in that explicit value here.
		// For more information about userDataFolder:
		// https://docs.microsoft.com/microsoft-edge/webview2/reference/win32/webview2-idl#createcorewebview2environmentwithoptions
		/*
		PWSTR userDataPath;
		HRESULT hrfolder = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &userDataPath);
		std::wstring userDataFolder(userDataPath);
		userDataFolder += L"\\T3000";
		Sleep(2000);
		DeleteDirectoryRecursive(userDataFolder);
		*/
	}
}

void BacnetWebViewAppWindow::CloseAppWindow()
{
	CloseWebView(true);
	DestroyWindow(m_mainWindow);
}

int BacnetWebViewAppWindow::RunMessagePump()
{
	// HACCEL hAccelTable = LoadAccelerators(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDC_WEBVIEW2APISAMPLE));
	MSG msg;

	// Main message loop:
	//! [MoveFocus0]
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	//! [MoveFocus0]

	return (int)msg.wParam;
}

#pragma region WebviewRelatedMethods
#include <WebView2EnvironmentOptions.h>
#include <wrl/event.h>
using namespace Microsoft::WRL;
void BacnetWebViewAppWindow::InitializeWebView()
{
	LPCWSTR subFolder = nullptr;
	PWSTR userDataPath;
	HRESULT hrfolder = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &userDataPath);
	std::wstring userDataFolder(userDataPath);
	userDataFolder += L"\\T3000";
	auto options = Microsoft::WRL::Make<CoreWebView2EnvironmentOptions>();
	HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(
		subFolder, userDataFolder.c_str(), options.Get(),
		Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
			this, &BacnetWebViewAppWindow::OnCreateEnvironmentCompleted)
		.Get());
}

HRESULT BacnetWebViewAppWindow::OnCreateEnvironmentCompleted(HRESULT result,
	ICoreWebView2Environment* environment)
{
	if (FAILED(result))
		return result;

	m_webViewEnvironment = environment;
	return m_webViewEnvironment->CreateCoreWebView2Controller(
		m_mainWindow, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
			this, &BacnetWebViewAppWindow::OnCreateCoreWebView2ControllerCompleted)
		.Get());
}

HRESULT BacnetWebViewAppWindow::OnCreateCoreWebView2ControllerCompleted(
	HRESULT result, ICoreWebView2Controller* controller)
{
	if (FAILED(result))
		return result;

	m_controller = controller;
	RECT bounds;
	GetClientRect(m_mainWindow, &bounds);
	m_controller->put_Bounds(bounds);
	wil::com_ptr<ICoreWebView2> coreWebView2;
	auto hr = m_controller->get_CoreWebView2(&coreWebView2);
	if (FAILED(hr))
		return hr;

	coreWebView2.query_to(&m_webView);

	SetTimer(m_mainWindow, 1, 1000, onTimer);

	/* START Resgister the listner for message handling */
	EventRegistrationToken token;
	m_webView->add_WebMessageReceived(Callback<ICoreWebView2WebMessageReceivedEventHandler>(this, &BacnetWebViewAppWindow::WebMessageReceived).Get(), &token);
	/* END Resgister the listner for message handling */

	// Add navigation completion handler for hard refresh (Ctrl+F5 equivalent)
	EventRegistrationToken navigationCompletedToken;
	m_webView->add_NavigationCompleted(
		Callback<ICoreWebView2NavigationCompletedEventHandler>(
			[this](ICoreWebView2* sender, ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT
			{
				// Perform hard refresh equivalent to Ctrl+F5
				static bool hasPerformedHardRefresh = false;
				if (!hasPerformedHardRefresh) {
					hasPerformedHardRefresh = true;

					// Execute JavaScript to perform hard refresh
					sender->ExecuteScript(
						L"try {"
						L"  // Add cache-busting meta tags"
						L"  var metaNoCache = document.createElement('meta');"
						L"  metaNoCache.httpEquiv = 'Cache-Control';"
						L"  metaNoCache.content = 'no-cache, no-store, must-revalidate';"
						L"  document.head.appendChild(metaNoCache);"
						L"  var metaPragma = document.createElement('meta');"
						L"  metaPragma.httpEquiv = 'Pragma';"
						L"  metaPragma.content = 'no-cache';"
						L"  document.head.appendChild(metaPragma);"
						L"  // Force hard reload like Ctrl+F5"
						L"  setTimeout(function() {"
						L"    window.location.reload(true);"
						L"  }, 100);"
						L"} catch(e) { console.log('Hard refresh error:', e); }", nullptr);
				}
				return S_OK;
			}).Get(), &navigationCompletedToken);

	/*wil::com_ptr<ICoreWebView2CompositionController> compositionController =
		m_controller.query<ICoreWebView2CompositionController>();*/
	m_dropTarget = Make<DropTarget>();
	m_dropTarget->Init(m_mainWindow, coreWebView2.get(), m_controller.get());

	if (m_webView != nullptr && m_initialUri != L"") {
		// Add timestamp parameter to URL for cache busting
		std::wstring cacheBustUri = m_initialUri;

		// Handle Vue.js hash routing - insert timestamp before hash fragment
		size_t hashPos = cacheBustUri.find(L'#');
		std::wstring baseUrl, hashFragment;

		if (hashPos != std::wstring::npos) {
			baseUrl = cacheBustUri.substr(0, hashPos);
			hashFragment = cacheBustUri.substr(hashPos);
		}
		else {
			baseUrl = cacheBustUri;
			hashFragment = L"";
		}

		// Add timestamp parameter to base URL
		if (baseUrl.find(L'?') != std::wstring::npos) {
			baseUrl += L"&ts=" + std::to_wstring(GetTickCount64());
		}
		else {
			baseUrl += L"?ts=" + std::to_wstring(GetTickCount64());
		}

		// Reconstruct URL with timestamp before hash
		cacheBustUri = baseUrl + hashFragment;

		m_webView->Navigate(cacheBustUri.c_str());
	}
}
bool ParseString2Json(CString strIn, Json::Value& jsonOut)
{
	bool bRet = false;
	std::string szIn = CT2A(strIn.GetString());
	//string szIn = wstring_to_string(strIn.GetBuffer(0));
	strIn.ReleaseBuffer();
	Json::Reader reader;
	if (reader.parse(szIn, jsonOut, false))
	{
		bRet = true;
	}

	return bRet;
}
HRESULT BacnetWebViewAppWindow::WebMessageReceived(ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args)
{
	LPWSTR pwStr;
	args->get_WebMessageAsJson(&pwStr);// TryGetWebMessageAsString(&pwStr);
	CString receivedMessage = pwStr;
	if (!receivedMessage.IsEmpty())
	{
		//TRACE(receivedMessage);
		ProcessWebviewMsg(receivedMessage);

		//AfxMessageBox(L"Message  from Javascript : " + receivedMessage);
		 //m_webView->PostWebMessageAsJson(L"test");
		// m_webView->ExecuteScript(L"MessageReceived('Sent From MFC-APP "+receivedMessage+ "')", Callback<ICoreWebView2ExecuteScriptCompletedHandler>(this, &BacnetWebViewAppWindow::ExecuteScriptResponse).Get());
	}
	return S_OK;
}
HRESULT BacnetWebViewAppWindow::ExecuteScriptResponse(HRESULT errorCode, LPCWSTR result)
{
	//AfxMessageBox(L"MFC Application Popup : Message Sent Successfully");
	//console
	return S_OK;
}

/*void read_panel_entries(int panel_id, int command) {
	str_point_info readinfo = { 0 };
	for (int i = 0; i < 64; i++) {
		readinfo.npanel_id = panel_id;
		readinfo.npanel_commad = command;
		readinfo.npoint_number = i;

		str_point_info* pmy_refresh_info = new str_point_info;
		memcpy(pmy_refresh_info, &readinfo, sizeof(str_point_info));
		if (!PostThreadMessage(nThreadID, MY_BAC_READ_GRP, (WPARAM)pmy_refresh_info, NULL))//post thread msg
		{

		}
		else
		{


		}
	}
}*/

CString empty_grp_file = _T("{\"activeItemIndex\":null,\"customObjectsCount\":0,\"elementGuidelines\":[],\"groupCount\":0,\"items\":[],\"itemsCount\":0,\"selectedTargets\":[],\"version\":\"0.4.4\",\"viewportTransform\":{\"scale\":1,\"x\":0,\"y\":0}} ");



void WrapErrorMessage(Json::StreamWriterBuilder& builder, const Json::Value& tempjson, CString& outmsg, const CString& error_message) {
	if (!error_message.IsEmpty()) {
		Json::Value tempjsonCopy = tempjson;
		tempjsonCopy["error"] = std::string(CT2A(error_message));
		const std::string output = Json::writeString(builder, tempjsonCopy);
		outmsg = CString(output.c_str());
	}
}




// Helper function to write HandleWebViewMsg logs to T3WebLog directory
// Creates organized logs in pattern: T3WebLog/YYYY-MM/MMDD/T3_CppMsg_HandWebViewMsg_MMDD_HHMM.txt
// Logs are grouped into 4-hour buckets (00-03, 04-07, 08-11, 12-15, 16-19, 20-23)
void WriteHandleWebViewMsgLog(const CString& messageType, const CString& outmsg, int itemCount)
{
	if (!enable_t3_web_logging) return;

	try {
		SYSTEMTIME st;
		GetSystemTime(&st);

		// Create directory path: T3WebLog\YYYY-MM\MMDD
		CString yearMonth;
		yearMonth.Format(_T("T3WebLog\\%04d-%02d"), st.wYear, st.wMonth);

		CString monthDay;
		monthDay.Format(_T("%02d%02d"), st.wMonth, st.wDay);

		CString logDir;
		logDir.Format(_T("%s\\%s"), yearMonth, monthDay);

		// Create directories if they don't exist
		CreateDirectory(_T("T3WebLog"), NULL);
		CreateDirectory(yearMonth, NULL);
		CreateDirectory(logDir, NULL);

		// Calculate 4-hour bucket (00-03, 04-07, 08-11, 12-15, 16-19, 20-23)
		int start_hour = (st.wHour / 4) * 4;
		int end_hour = start_hour + 3;

		// Create log file with pattern: YYYY-MM/MMDD/T3_CppMsg_HandWebViewMsg_MMDD_HHMM.txt
		CString logFile;
		logFile.Format(_T("%s\\T3_CppMsg_HandWebViewMsg_%02d%02d_%02d%02d.txt"),
			logDir, st.wMonth, st.wDay, start_hour, end_hour);

		CStdioFile file;
		// Use append mode to add multiple calls to same 4-hour period file
		if (file.Open(logFile, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate | CFile::typeText)) {
			// Move to end of file for appending
			file.SeekToEnd();

			// Log entry separator and timestamp
			CString logContent;
			logContent.Format(_T("=== %s C++ FFI Call [%04d-%02d-%02d %02d:%02d:%02d] ===\n"),
				messageType, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
			file.WriteString(logContent);

			logContent.Format(_T("JSON response size: %d characters\n"), outmsg.GetLength());
			file.WriteString(logContent);

			logContent.Format(_T("Data items processed: %d\n"), itemCount);
			file.WriteString(logContent);

			// Log full JSON response for complete inspection
			file.WriteString(_T("=== Complete JSON Response ===\n"));
			file.WriteString(outmsg);
			file.WriteString(_T("\n"));

			file.WriteString(_T("=== End of Entry ===\r\n"));
			file.Close();
		}
	}
	catch (...) {
		// Silently ignore logging errors to prevent disrupting main functionality
		// Logging failure should not affect the FFI response
	}
}


#include <mutex>
std::mutex handleWebViewMsgMutex;
void HandleWebViewMsg(CString msg, CString& outmsg, int msg_source = 0)
{
	// 使用 std::lock_guard 来锁定互斥锁
	std::lock_guard<std::mutex> lock(handleWebViewMsgMutex);

	Json::Value json;
	std::string message = CT2A(msg);
	Json::Reader reader;
	reader.parse(message, json, false);
	int action = json.get("action", Json::nullValue).asInt();


	Json::StreamWriterBuilder builder;
	builder["indentation"] = ""; // If you want whitespace-less output
	Json::Value tempjson;
	int grp_index = 0;  //选中的第几幅图;
	int grp_serial_number = 0; //选中设备的序列号 ，靠这个序列号来找到对应的设备 zip文件;
	int n_ret = -1;
	CString des_file;
	CString des_file_zip;
	CString des_lib_file;

	TRACE(msg + _T("\r\n"));
#if 1
	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	CString image_fordor = g_strExePth + CString("Database\\Buildings\\") + pFrame->m_strCurMainBuildingName + _T("\\image");
	CString temp_item;
	CString temp_item_zip;

#endif

	// append msgId
	if (json.isMember("msgId")) {
		std::string msgId = json["msgId"].asString();
		tempjson["msgId"] = msgId;
	}

	CString temp_action;
	temp_action.Format(_T("action value = %d\r\n"), action);
	DFTrace(temp_action);

	switch (action)
	{
	case WEBVIEW_MESSAGE_TYPE::GET_PANEL_DATA:
	{
		tempjson["action"] = "GET_PANEL_DATA_RES";
		int npanel_id = json.get("panelId", Json::nullValue).asInt();
		//temp_action.Format(_T("npanel_idididididididididididid = %d\r\n"), npanel_id);
		//DFTrace(temp_action);
		if (npanel_id == 0)
			npanel_id = bac_gloab_panel;
		int nret = LoadOnlinePanelData(npanel_id);
		if (nret < 0)
		{
			CString temp_message;
			temp_message.Format(_T("No cached data was found for this panel %d"), npanel_id);
			SetPaneString(BAC_SHOW_MISSION_RESULTS, temp_message);
			WrapErrorMessage(builder, tempjson, outmsg, temp_message);
			break;
#if 0
			temp_message.Format(_T("No cached data was found for this panel %d, do you want to read the device's data immediately"), npanel_id);
			if (MessageBox(m_mainWindow, temp_message, L"Warning", MB_YESNO) == IDYES)
			{
				for (int j = 0; j < g_bacnet_panel_info.size(); j++)
				{
					int nseiral = 0;
					if (g_bacnet_panel_info.at(j).panel_number != npanel_id)
						continue;

					nseiral = g_bacnet_panel_info.at(j).nseiral_number;
					CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
					pFrame->LoadDeviceData(nseiral);
					PostMessage(m_mainWindow, WM_CLOSE, NULL, NULL);
				}
			}
#endif
			break;
		}
		else if (nret == 0)
		{
			CString temp_message;
			temp_message.Format(_T("Panel %d is offline!\r\n"), npanel_id);
			TRACE(temp_message);
			WrapErrorMessage(builder, tempjson, outmsg, temp_message);
			break;
		}
		else if (npanel_id == 0)
		{
			CString temp_message;
			temp_message.Format(_T("Panel id can't be  0 !"), npanel_id);
			WrapErrorMessage(builder, tempjson, outmsg, temp_message);
			break;
		}

		g_Device_Basic_Setting[npanel_id].reg.ip_addr;
		char ipStr[16]; // 用于存储转换后的IP字符串
		unsigned char* ipAddr = g_Device_Basic_Setting[npanel_id].reg.ip_addr;
		// 使用 sprintf 将 IP 地址转换为字符串
		sprintf(ipStr, "%d.%d.%d.%d", ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3]);

		tempjson["panel_id"] = npanel_id;
		tempjson["panel_name"] = (char*)g_Device_Basic_Setting[npanel_id].reg.panel_name;
		tempjson["panel_serial_number"] = g_Device_Basic_Setting[npanel_id].reg.n_serial_number;
		tempjson["panel_ipaddress"] = ipStr;

		int p_i = 0;
		for (int i = 0; i < BAC_INPUT_ITEM_COUNT; i++) {
			tempjson["data"][p_i]["pid"] = npanel_id;
			tempjson["data"][p_i]["type"] = "INPUT";
			tempjson["data"][p_i]["index"] = i;
			tempjson["data"][p_i]["id"] = "IN" + to_string(i + 1);
			tempjson["data"][p_i]["command"] = to_string(npanel_id) + "IN" + to_string(i + 1);
			tempjson["data"][p_i]["description"] = (char*)g_Input_data[npanel_id].at(i).description;
			tempjson["data"][p_i]["label"] = (char*)g_Input_data[npanel_id].at(i).label;
			tempjson["data"][p_i]["unit"] = g_Input_data[npanel_id].at(i).range;
			tempjson["data"][p_i]["auto_manual"] = g_Input_data[npanel_id].at(i).auto_manual;
			tempjson["data"][p_i]["value"] = g_Input_data[npanel_id].at(i).value;
			tempjson["data"][p_i]["filter"] = g_Input_data[npanel_id].at(i).filter;
			tempjson["data"][p_i]["control"] = g_Input_data[npanel_id].at(i).control;
			tempjson["data"][p_i]["digital_analog"] = g_Input_data[npanel_id].at(i).digital_analog;
			tempjson["data"][p_i]["range"] = g_Input_data[npanel_id].at(i).range;
			tempjson["data"][p_i]["calibration_sign"] = g_Input_data[npanel_id].at(i).calibration_sign;
			tempjson["data"][p_i]["calibration_h"] = g_Input_data[npanel_id].at(i).calibration_h;
			tempjson["data"][p_i]["calibration_l"] = g_Input_data[npanel_id].at(i).calibration_l;
			tempjson["data"][p_i]["decom"] = g_Input_data[npanel_id].at(i).decom; //for input   0 "Normal"    1"Open"   2  "Shorted"
			p_i++;
		}

		for (int i = 0; i < BAC_OUTPUT_ITEM_COUNT; i++) {
			tempjson["data"][p_i]["pid"] = npanel_id;
			tempjson["data"][p_i]["type"] = "OUTPUT";
			tempjson["data"][p_i]["index"] = i;
			tempjson["data"][p_i]["id"] = "OUT" + to_string(i + 1);
			tempjson["data"][p_i]["command"] = to_string(npanel_id) + "OUT" + to_string(i + 1);
			tempjson["data"][p_i]["description"] = (char*)g_Output_data[npanel_id].at(i).description;
			tempjson["data"][p_i]["label"] = (char*)g_Output_data[npanel_id].at(i).label;
			tempjson["data"][p_i]["auto_manual"] = g_Output_data[npanel_id].at(i).auto_manual;
			tempjson["data"][p_i]["value"] = g_Output_data[npanel_id].at(i).value;
			tempjson["data"][p_i]["low_voltage"] = g_Output_data[npanel_id].at(i).low_voltage;
			tempjson["data"][p_i]["high_voltage"] = g_Output_data[npanel_id].at(i).high_voltage;
			tempjson["data"][p_i]["range"] = g_Output_data[npanel_id].at(i).range;
			tempjson["data"][p_i]["control"] = g_Output_data[npanel_id].at(i).control;
			tempjson["data"][p_i]["digital_analog"] = g_Output_data[npanel_id].at(i).digital_analog;
			tempjson["data"][p_i]["hw_switch_status"] = g_Output_data[npanel_id].at(i).hw_switch_status;
			tempjson["data"][p_i]["decom"] = g_Output_data[npanel_id].at(i).decom; //for output   0 "Normal"    1"Alarm"  
			p_i++;
		}


		for (int i = 0; i < BAC_VARIABLE_ITEM_COUNT; i++) {
			tempjson["data"][p_i]["pid"] = npanel_id;
			tempjson["data"][p_i]["type"] = "VARIABLE";
			tempjson["data"][p_i]["index"] = i;
			tempjson["data"][p_i]["id"] = "VAR" + to_string(i + 1);
			tempjson["data"][p_i]["command"] = to_string(npanel_id) + "VAR" + to_string(i + 1);
			tempjson["data"][p_i]["description"] = (char*)g_Variable_data[npanel_id].at(i).description;
			tempjson["data"][p_i]["label"] = (char*)g_Variable_data[npanel_id].at(i).label;
			tempjson["data"][p_i]["auto_manual"] = g_Variable_data[npanel_id].at(i).auto_manual;
			tempjson["data"][p_i]["value"] = g_Variable_data[npanel_id].at(i).value;
			tempjson["data"][p_i]["range"] = g_Variable_data[npanel_id].at(i).range;
			tempjson["data"][p_i]["control"] = g_Variable_data[npanel_id].at(i).control;
			tempjson["data"][p_i]["digital_analog"] = g_Variable_data[npanel_id].at(i).digital_analog;
			tempjson["data"][p_i]["unused"] = g_Variable_data[npanel_id].at(i).unused;
			p_i++;
		}


		for (int i = 0; i < BAC_PROGRAM_ITEM_COUNT; i++) {
			tempjson["data"][p_i]["pid"] = npanel_id;
			tempjson["data"][p_i]["type"] = "PROGRAM";
			tempjson["data"][p_i]["index"] = i;
			tempjson["data"][p_i]["id"] = "PRG" + to_string(i + 1);
			tempjson["data"][p_i]["command"] = to_string(npanel_id) + "PRG" + to_string(i + 1);
			tempjson["data"][p_i]["description"] = (char*)g_Program_data[npanel_id].at(i).description;
			tempjson["data"][p_i]["label"] = (char*)g_Program_data[npanel_id].at(i).label;
			tempjson["data"][p_i]["auto_manual"] = g_Program_data[npanel_id].at(i).auto_manual;
			tempjson["data"][p_i]["status"] = g_Program_data[npanel_id].at(i).on_off;
			tempjson["data"][p_i]["unused"] = g_Program_data[npanel_id].at(i).unused;
			p_i++;
		}


		for (int i = 0; i < BAC_SCHEDULE_COUNT; i++) {
			tempjson["data"][p_i]["pid"] = npanel_id;
			tempjson["data"][p_i]["type"] = "SCHEDULE";
			tempjson["data"][p_i]["index"] = i;
			tempjson["data"][p_i]["id"] = "SCH" + to_string(i + 1);
			tempjson["data"][p_i]["command"] = to_string(npanel_id) + "SCH" + to_string(i + 1);
			tempjson["data"][p_i]["description"] = (char*)g_Weekly_data[npanel_id].at(i).description;
			tempjson["data"][p_i]["label"] = (char*)g_Weekly_data[npanel_id].at(i).label;
			tempjson["data"][p_i]["auto_manual"] = g_Weekly_data[npanel_id].at(i).auto_manual;
			tempjson["data"][p_i]["output"] = g_Weekly_data[npanel_id].at(i).value;
			tempjson["data"][p_i]["state1"] = g_Weekly_data[npanel_id].at(i).override_1_value;
			tempjson["data"][p_i]["state2"] = g_Weekly_data[npanel_id].at(i).override_2_value;
			tempjson["data"][p_i]["unused"] = g_Weekly_data[npanel_id].at(i).unused;
			p_i++;
		}


		for (int i = 0; i < BAC_HOLIDAY_COUNT; i++) {
			tempjson["data"][p_i]["pid"] = npanel_id;
			tempjson["data"][p_i]["type"] = "HOLIDAY";
			tempjson["data"][p_i]["index"] = i;
			tempjson["data"][p_i]["id"] = "CAL" + to_string(i + 1);
			tempjson["data"][p_i]["command"] = to_string(npanel_id) + "CAL" + to_string(i + 1);
			tempjson["data"][p_i]["description"] = (char*)g_Annual_data[npanel_id].at(i).description;
			tempjson["data"][p_i]["label"] = (char*)g_Annual_data[npanel_id].at(i).label;
			tempjson["data"][p_i]["auto_manual"] = g_Annual_data[npanel_id].at(i).auto_manual;
			tempjson["data"][p_i]["value"] = g_Annual_data[npanel_id].at(i).value;
			tempjson["data"][p_i]["unused"] = g_Annual_data[npanel_id].at(i).unused;
			p_i++;
		}

		for (int i = 0; i < BAC_PID_COUNT; i++)
		{
			tempjson["data"][p_i]["pid"] = npanel_id;
			tempjson["data"][p_i]["type"] = "PID";
			tempjson["data"][p_i]["index"] = i;
			tempjson["data"][p_i]["id"] = "PID" + to_string(i + 1);
			tempjson["data"][p_i]["command"] = to_string(npanel_id) + "PID" + to_string(i + 1);
			tempjson["data"][p_i]["input_panel"] = g_controller_data[npanel_id].at(i).input.panel;
			tempjson["data"][p_i]["input_type"] = g_controller_data[npanel_id].at(i).input.point_type;
			tempjson["data"][p_i]["input_number"] = g_controller_data[npanel_id].at(i).input.number;
			tempjson["data"][p_i]["input_value"] = g_controller_data[npanel_id].at(i).input_value;
			tempjson["data"][p_i]["units"] = g_controller_data[npanel_id].at(i).units;
			tempjson["data"][p_i]["auto_manual"] = g_controller_data[npanel_id].at(i).auto_manual;
			tempjson["data"][p_i]["output_value"] = g_controller_data[npanel_id].at(i).value;
			tempjson["data"][p_i]["setpoint_panel"] = g_controller_data[npanel_id].at(i).setpoint.panel;
			tempjson["data"][p_i]["setpoint_type"] = g_controller_data[npanel_id].at(i).setpoint.point_type;
			tempjson["data"][p_i]["setpoint_number"] = g_controller_data[npanel_id].at(i).setpoint.number;
			tempjson["data"][p_i]["time_type"] = g_controller_data[npanel_id].at(i).repeats_per_min;
			tempjson["data"][p_i]["action"] = g_controller_data[npanel_id].at(i).action;
			tempjson["data"][p_i]["proportional"] = g_controller_data[npanel_id].at(i).proportional;
			tempjson["data"][p_i]["integral"] = g_controller_data[npanel_id].at(i).reset;
			tempjson["data"][p_i]["differential"] = g_controller_data[npanel_id].at(i).rate;
			tempjson["data"][p_i]["bias"] = g_controller_data[npanel_id].at(i).bias;
			p_i++;
		}

		for (int i = 0; i < BAC_SCREEN_COUNT; i++)
		{
			tempjson["data"][p_i]["pid"] = npanel_id;
			tempjson["data"][p_i]["type"] = "GRP";
			tempjson["data"][p_i]["index"] = i;
			tempjson["data"][p_i]["id"] = "GRP" + to_string(i + 1);
			tempjson["data"][p_i]["command"] = to_string(npanel_id) + "GRP" + to_string(i + 1);
			tempjson["data"][p_i]["description"] = (char*)g_screen_data[npanel_id].at(i).description;
			tempjson["data"][p_i]["label"] = (char*)g_screen_data[npanel_id].at(i).label;
			tempjson["data"][p_i]["count"] = g_screen_data[npanel_id].at(i).webview_element_count;
			//There is also additional data that does not need to be passed to the webview interface
			p_i++;
		}

		for (int i = 0; i < BAC_MONITOR_COUNT; i++)
		{
			tempjson["data"][p_i]["pid"] = npanel_id;
			tempjson["data"][p_i]["type"] = "MON";
			tempjson["data"][p_i]["index"] = i;
			tempjson["data"][p_i]["id"] = "MON" + to_string(i + 1);
			tempjson["data"][p_i]["command"] = to_string(npanel_id) + "MON" + to_string(i + 1);
			tempjson["data"][p_i]["label"] = (char*)g_monitor_data[npanel_id].at(i).label;
			tempjson["data"][p_i]["hour_interval_time"] = g_monitor_data[npanel_id].at(i).hour_interval_time;
			tempjson["data"][p_i]["minute_interval_time"] = g_monitor_data[npanel_id].at(i).minute_interval_time;
			tempjson["data"][p_i]["second_interval_time"] = g_monitor_data[npanel_id].at(i).second_interval_time;
			tempjson["data"][p_i]["status"] = g_monitor_data[npanel_id].at(i).status;
			tempjson["data"][p_i]["num_inputs"] = g_monitor_data[npanel_id].at(i).num_inputs; ///* total number of points */
			tempjson["data"][p_i]["an_inputs"] = g_monitor_data[npanel_id].at(i).an_inputs;  ///* number of analog points */
			for (int m = 0; m < MAX_POINTS_IN_MONITOR; m++)
			{
				tempjson["data"][p_i]["range"][m] = g_monitor_data[npanel_id].at(i).range[m]; //14个input对应的range
				//例如 例子1  111OUT45          panel = 111 , sub_panel = 0 . point_type = 0 ，number = 45 , network 默认为0
				//例如 例子2  123.45.MB_REG67   panel = 123 , sub_panel = 45, point_type = 2 , number = 67 , network 默认为0
				tempjson["data"][p_i]["input"][m]["panel"] = g_monitor_data[npanel_id].at(i).inputs[m].panel;
				tempjson["data"][p_i]["input"][m]["sub_panel"] = g_monitor_data[npanel_id].at(i).inputs[m].sub_panel;
				tempjson["data"][p_i]["input"][m]["point_type"] = g_monitor_data[npanel_id].at(i).inputs[m].point_type;
				tempjson["data"][p_i]["input"][m]["point_number"] = g_monitor_data[npanel_id].at(i).inputs[m].number;
				tempjson["data"][p_i]["input"][m]["network"] = g_monitor_data[npanel_id].at(i).inputs[m].network;
			}
			p_i++;
		}

		int r_i = 0;
#pragma region digital_custom_range
		for (int i = 0; i < BAC_CUSTOMER_UNITS_COUNT; i++)
		{
			tempjson["ranges"][r_i]["pid"] = npanel_id;
			tempjson["ranges"][r_i]["type"] = "digital";
			tempjson["ranges"][r_i]["index"] = i + 23;    // 8 custom digital ranges, corresponding to ranges 23 to 30 (Condition .digital_analog = 0)
			tempjson["ranges"][r_i]["id"] = "DRange" + to_string(i + 1);
			tempjson["ranges"][r_i]["direct"] = g_customer_unit_data[npanel_id].at(i).direct;
			tempjson["ranges"][r_i]["off"] = (char*)g_customer_unit_data[npanel_id].at(i).digital_units_off;
			tempjson["ranges"][r_i]["on"] = (char*)g_customer_unit_data[npanel_id].at(i).digital_units_on;
			r_i++;
		}

#pragma endregion

#pragma region analog_custom_range
		for (int i = 0; i < BAC_ALALOG_CUSTMER_RANGE_TABLE_COUNT; i++)
		{
			tempjson["ranges"][r_i]["pid"] = npanel_id;
			tempjson["ranges"][r_i]["type"] = "analog";
			tempjson["ranges"][r_i]["index"] = i + 20;  // 5 analog custom range, corresponding to ranges 23 to 30  (Condition .digital_analog = 1)
			tempjson["ranges"][r_i]["id"] = "ARange" + to_string(i + 1);
			tempjson["ranges"][r_i]["unit"] = (char*)g_analog_custmer_range[npanel_id].at(i).table_name;
			r_i++;
			//The other values of this structure don't need to be passed to json for now
		}

#pragma endregion

		for (int i = 0; i < BAC_MSV_COUNT; i++)
		{
			tempjson["ranges"][r_i]["pid"] = npanel_id;
			tempjson["ranges"][r_i]["type"] = "MSV";
			tempjson["ranges"][r_i]["index"] = i + 101;  //8 custom digital MSV ranges, corresponding to ranges 101 to 104 (MSV is special, do not care about  digital_analog value, has nothing to do with  digital_analog)
			tempjson["ranges"][r_i]["id"] = "MSV" + to_string(i + 1);
			for (int j = 0; j < STR_MSV_MULTIPLE_COUNT; j++)
			{
				char temp_status[20];	memset(temp_status, 0, 20); sprintf(temp_status, "Status%d", j);
				char temp_name[20];	memset(temp_name, 0, 20); sprintf(temp_name, "Name%d", j);
				char temp_value[20];	memset(temp_value, 0, 20); sprintf(temp_value, "Value%d", j);
				tempjson["ranges"][r_i]["options"][j]["status"] = g_msv_data[npanel_id].at(i).msv_data[j].status;
				tempjson["ranges"][r_i]["options"][j]["name"] = (char*)g_msv_data[npanel_id].at(i).msv_data[j].msv_name;
				tempjson["ranges"][r_i]["options"][j]["value"] = g_msv_data[npanel_id].at(i).msv_data[j].msv_value;
			}
			r_i++;
		}


#if 1
		for (int k = 0; k < BAC_WEEKLYCODE_ROUTINES_COUNT; k++)
		{
			tempjson["data"][r_i]["pid"] = npanel_id;
			tempjson["data"][r_i]["type"] = "SCH";
			tempjson["data"][r_i]["index"] = k; //第几个 schedule
			tempjson["data"][r_i]["id"] = "SCH" + to_string(k + 1);
			tempjson["data"][r_i]["command"] = to_string(npanel_id) + "SCH" + to_string(k + 1);

			for (int j = 0; j < 9; j++)
			{
				for (int i = 0; i < 8; i++)
				{
					tempjson["data"][r_i]["time"][j][i]["minutes"] = g_Schedual_Time_data[npanel_id].at(k).Schedual_Day_Time[i][j].time_minutes;
					tempjson["data"][r_i]["time"][j][i]["hours"] = g_Schedual_Time_data[npanel_id].at(k).Schedual_Day_Time[i][j].time_hours;
					tempjson["data"][r_i]["time"][j][i]["flag"] = g_Schedual_time_flag[npanel_id].at(weekly_list_line).Time_flag[i][j];
				}
			}
			r_i++;
		}



		for (int k = 0; k < BAC_HOLIDAY_COUNT; k++)
		{
			tempjson["data"][r_i]["pid"] = npanel_id;
			tempjson["data"][r_i]["type"] = "HOL";
			tempjson["data"][r_i]["index"] = k; //第几个 holiday
			tempjson["data"][r_i]["id"] = "HOL" + to_string(k + 1);
			tempjson["data"][r_i]["command"] = to_string(npanel_id) + "CAL" + to_string(k + 1);
			for (int m = 0; m < ANNUAL_CODE_SIZE; m++)
			{
				tempjson["data"][r_i]["data"][m] = gp_DayState[npanel_id][k][m];
			}
			r_i++;
		}
#endif




		const std::string output = Json::writeString(builder, tempjson);
		CString temp_cs(output.c_str());
		outmsg = temp_cs;

		break;
	}
	case WEBVIEW_MESSAGE_TYPE::LOAD_GRAPHIC_ENTRY:
	case WEBVIEW_MESSAGE_TYPE::GET_INITIAL_DATA:
	{
		Json::Value tempjson;
		int panel_id;
		if (msg_source == 0)//来自T3000按键点击
		{
			grp_serial_number = g_selected_serialnumber;
			panel_id = bac_gloab_panel;
			// 判断是否存在 "viewitem" 字段
			if (json.isMember("viewitem") && !json["viewitem"].isNull())
			{
				// 存在 "viewitem" 字段且不为 null
				grp_index = json.get("viewitem", Json::nullValue).asInt(); //这里如果是按键点进来的，要用T3000的index ，如果是 浏览器的 要浏览器的index
				// 使用 grp_index 进行后续操作
			}
			else {
				// 不存在 "viewitem" 字段或其值为 null
				grp_index = screen_list_line;
				//std::cout << "JSON 中不存在 viewitem 字段或其值为 null" << std::endl;
			}

			temp_item_zip.Format(_T("%u_%d.zip"), grp_serial_number, grp_index);
			des_file_zip = image_fordor + _T("\\") + temp_item_zip;
			//判断文件 des_file_zip是否存在 没有缓存就只能从设备中读取
			if (!PathFileExists(des_file_zip))
			{
				int n_read_result = Read_Webview_Data_Special(panel_id, grp_serial_number, grp_index);
				if (n_read_result < 0)
				{
					if (action == GET_INITIAL_DATA)
					{
						tempjson["action"] = "GET_INITIAL_DATA_RES";
					}
					else if (action == LOAD_GRAPHIC_ENTRY)
					{
						tempjson["action"] = "LOAD_GRAPHIC_ENTRY_RES";
					}
					WrapErrorMessage(builder, tempjson, outmsg, _T("Read panel data timeout."));
					DFTrace(_T("Read panel data timeout."));
					break;
				}
			}
		}
		else if (msg_source == 1)//来自浏览器
		{
			panel_id = json.get("panelId", Json::nullValue).asInt(); //这里要根据panelId来判断是那个序列号的设备，进而确定保存的文件名
			grp_index = json.get("viewitem", Json::nullValue).asInt(); //这里如果是按键点进来的，要用T3000的index ，如果是 浏览器的 要浏览器的index
			grp_serial_number = json.get("serialNumber", Json::nullValue).asInt();
			//浏览器这里要调用一个函数，根据panel_id 和 grp_index 以及序列号 读取对应的panel的 绘图数据的zip文件并解压;

			//返回传入的PanelId,ViewItem,SerialNumber 消息群发后根据状态回显数据
			tempjson["panelId"] = panel_id;
			tempjson["viewitem"] = grp_index;
			tempjson["serialNumber"] = grp_serial_number;

			if ((panel_id <= 0) || (panel_id > 254))
			{
				if (action == GET_INITIAL_DATA)
				{
					tempjson["action"] = "GET_INITIAL_DATA_RES";
				}
				else if (action == LOAD_GRAPHIC_ENTRY)
				{
					tempjson["action"] = "LOAD_GRAPHIC_ENTRY_RES";
				}
				WrapErrorMessage(builder, tempjson, outmsg, _T("Message Source Error."));
				break;
			}

			if (grp_index < 0)
			{

				if (action == GET_INITIAL_DATA)
				{
					tempjson["action"] = "GET_INITIAL_DATA_RES";
				}
				else if (action == LOAD_GRAPHIC_ENTRY)
				{
					tempjson["action"] = "LOAD_GRAPHIC_ENTRY_RES";
				}
				WrapErrorMessage(builder, tempjson, outmsg, _T("Message Source Error."));
				break;
			}
			int n_read_result = Read_Webview_Data_Special(panel_id, grp_serial_number, grp_index);
			if (n_read_result < 0)
			{
				if (action == GET_INITIAL_DATA)
				{
					tempjson["action"] = "GET_INITIAL_DATA_RES";
				}
				else if (action == LOAD_GRAPHIC_ENTRY)
				{
					tempjson["action"] = "LOAD_GRAPHIC_ENTRY_RES";
				}
				WrapErrorMessage(builder, tempjson, outmsg, _T("Read panel data timeout."));
				DFTrace(_T("Read panel data timeout."));
				break;
			}
		}
		else
		{

			if (action == GET_INITIAL_DATA)
			{
				tempjson["action"] = "GET_INITIAL_DATA_RES";
			}
			else if (action == LOAD_GRAPHIC_ENTRY)
			{
				tempjson["action"] = "LOAD_GRAPHIC_ENTRY_RES";
			}
			SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Message Source Error."));
			WrapErrorMessage(builder, tempjson, outmsg, _T("Message Source Error."));
			break;
		}

		temp_item.Format(_T("%u_%d.txt"), grp_serial_number, grp_index);
		temp_item_zip.Format(_T("%u_%d.zip"), grp_serial_number, grp_index);

		des_file = image_fordor + _T("\\") + temp_item;
		des_file_zip = image_fordor + _T("\\") + temp_item_zip;
		CString temp_lib_file;
		temp_lib_file = _T("hvac_library.json");
		des_lib_file = image_fordor + _T("\\") + temp_lib_file;

		if (action == LOAD_GRAPHIC_ENTRY)
		{
			tempjson["action"] = "LOAD_GRAPHIC_ENTRY_RES";
			panel_id = json.get("panelId", Json::nullValue).asInt();
			int entry_index = json.get("entryIndex", Json::nullValue).asInt();
			if ((panel_id == 0) || entry_index >= BAC_SCREEN_COUNT)
			{
				SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Panel is invalid or Index is invalid."));
				WrapErrorMessage(builder, tempjson, outmsg, _T("Panel is invalid or Index is invalid."));
				break;
			}
			grp_index = entry_index;
			if (panel_id == bac_gloab_panel)
				grp_serial_number = g_selected_serialnumber;
			else
			{
				for (int z = 0; z < g_bacnet_panel_info.size(); z++)
				{
					if (panel_id == g_bacnet_panel_info.at(z).panel_number)
					{
						grp_serial_number = g_bacnet_panel_info.at(z).nseiral_number;
						break;
					}
				}
				if (grp_serial_number == 0)
				{
					SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Panel is offline."));
					WrapErrorMessage(builder, tempjson, outmsg, _T("Panel is offline."));
					break;
				}
			}

		}
		else if (action == GET_INITIAL_DATA)
		{
			tempjson["action"] = "GET_INITIAL_DATA_RES";
			//panel_id = bac_gloab_panel;
			//grp_serial_number = g_selected_serialnumber;
			//grp_index = screen_list_line;
		}
		tempjson["entry"]["pid"] = bac_gloab_panel;
		tempjson["entry"]["index"] = grp_index;
		tempjson["entry"]["id"] = "GRP" + to_string(grp_index + 1);
		tempjson["entry"]["command"] = to_string(panel_id) + "GRP" + to_string(grp_index + 1);
		tempjson["entry"]["description"] = (char*)g_screen_data[panel_id].at(grp_index).description;
		tempjson["entry"]["label"] = (char*)g_screen_data[panel_id].at(grp_index).label;

#if 0
		CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
		CString image_fordor = g_strExePth + CString("Database\\Buildings\\") + pFrame->m_strCurMainBuildingName + _T("\\image");
		CString temp_item;
		CString temp_item_zip;
		if (0)
		{
			temp_item.Format(_T("%u_%d_json.txt"), grp_serial_number, grp_index);
			temp_item_zip.Format(_T("%u_%d_json.zip"), grp_serial_number, grp_index);
		}
		else
		{
			temp_item.Format(_T("%u_%d.txt"), grp_serial_number, grp_index);
			temp_item_zip.Format(_T("%u_%d.zip"), grp_serial_number, grp_index);

		}

		des_file = image_fordor + _T("\\") + temp_item;
		des_file_zip = image_fordor + _T("\\") + temp_item_zip;
		CString temp_lib_file;
		temp_lib_file = _T("hvac_library.json");
		des_lib_file = image_fordor + _T("\\") + temp_lib_file;
#endif
		CFile file;


		CFileFind temp_find;
		if (temp_find.FindFile(des_file) == 0) //当 没有发现缓存的 grp文件时，返回空的 grp给 webview 显示;
		{
			wstring nbuff_wstring(empty_grp_file.GetBuffer());
			string nbuff_str(nbuff_wstring.begin(), nbuff_wstring.end());
			tempjson["data"] = nbuff_str;
			const std::string output = Json::writeString(builder, tempjson);
			CString tempjson_str(output.c_str());
			outmsg = tempjson_str;
			//m_webView->PostWebMessageAsJson(tempjson_str);
			break;
		}
		if (temp_find.FindFile(des_lib_file) == 0) //当 没有发现缓存的 lib文件时，返回空的 grp给 webview 显示;
		{

		}
		else
		{
			CFile filelib;
			filelib.Open(des_lib_file, CFile::modeRead, NULL);
			DWORD len = filelib.GetLength();
			if (len == 0)
				break;
			WCHAR* nlibbuff = new WCHAR[len + 1];
			memset(nlibbuff, 0, 2 * (len + 1));
			filelib.Read(nlibbuff, len * 2 + 1);   //Read( void* lpBuf, UINT nCount ) lpBuf是用于接收读取到的数据的Buf指针nCount是从文件读取的字节数
			wstring nbuff_wstring(nlibbuff);
			string file_content(nbuff_wstring.begin(), nbuff_wstring.end());
			tempjson["library"] = file_content;
			filelib.Close();
			delete nlibbuff;
		}

		file.Open(des_file, CFile::modeRead, NULL);
		DWORD len = file.GetLength();
		if (len == 0)
			break;
		WCHAR* nbuff = new WCHAR[len + 1];
		memset(nbuff, 0, 2 * (len + 1));
		file.Read(nbuff, len * 2 + 1);   //Read( void* lpBuf, UINT nCount ) lpBuf是用于接收读取到的数据的Buf指针nCount是从文件读取的字节数
		wstring nbuff_wstring(nbuff);
		string nbuff_str(nbuff_wstring.begin(), nbuff_wstring.end());
		tempjson["data"] = nbuff_str;
		const std::string output = Json::writeString(builder, tempjson);
		CString tempjson_str(output.c_str());
		//TRACE(nbuff);
		//m_webView->PostWebMessageAsJson(tempjson_str);
		outmsg = tempjson_str;
		delete nbuff;

		break;
	}


	case WEBVIEW_MESSAGE_TYPE::SAVE_GRAPHIC_DATA:
	{
		int panelId = json.get("panelId", Json::nullValue).asInt(); //这里要根据panelId来判断是那个序列号的设备，进而确定保存的文件名
		int save_grp_index = -1;


		if (msg_source == 0)//来自T3000按键点击
		{
			grp_serial_number = g_selected_serialnumber; //暂时用这个代替
			// 判断是否存在 "viewitem" 字段
			if (json.isMember("viewitem") && !json["viewitem"].isNull())
			{
				// 存在 "viewitem" 字段且不为 null
				save_grp_index = json.get("viewitem", Json::nullValue).asInt(); //这里如果是按键点进来的，要用T3000的index ，如果是 浏览器的 要浏览器的index
				// 使用 grp_index 进行后续操作
			}
			else {
				// 不存在 "viewitem" 字段或其值为 null
				save_grp_index = screen_list_line;
				//std::cout << "JSON 中不存在 viewitem 字段或其值为 null" << std::endl;
			}

			//save_grp_index = screen_list_line;
			panelId = bac_gloab_panel; //有时候浏览器这里传过来的panelID是0 ，不正常，这里强行赋值;
			save_button_click = 1;
		}
		else if (msg_source == 1)//来自浏览器
		{
			grp_serial_number = json.get("serialNumber", Json::nullValue).asInt();
			save_grp_index = json.get("viewitem", Json::nullValue).asInt();
		}
		else
		{
			if (msg_source == 0)
				SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Message Source Error."));
			WrapErrorMessage(builder, tempjson, outmsg, _T("Message Source Error."));
			break;
		}

		if ((save_grp_index < 0) || (save_grp_index > 7))
		{
			if (msg_source == 0)
				SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Viewitem out of range."));
			WrapErrorMessage(builder, tempjson, outmsg, _T("Viewitem out of range."));
			break;
		}


		temp_item.Format(_T("%u_%d.txt"), grp_serial_number, save_grp_index);
		temp_item_zip.Format(_T("%u_%d.zip"), grp_serial_number, save_grp_index);

		des_file = image_fordor + _T("\\") + temp_item;
		des_file_zip = image_fordor + _T("\\") + temp_item_zip;
		CString temp_lib_file;
		temp_lib_file = _T("hvac_library.json");
		des_lib_file = image_fordor + _T("\\") + temp_lib_file;

		int temp_elementcount = json["data"].get("itemsCount", Json::nullValue).asInt();
		if (temp_elementcount == 0)
		{
			if (msg_source == 0)
				SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("No data to save."));
			WrapErrorMessage(builder, tempjson, outmsg, _T("No data to save."));
			break;
		}



		//m_screen_data.at(screen_list_line).webview_element_count = temp_elementcount;
		//n_ret = Write_Private_Data_Blocking(WRITESCREEN_T3000, screen_list_line, screen_list_line, g_bac_instance);
		//if (n_ret > 0)
		//{
		//	SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write element count success."));
		//}
		//else
		//{
		//	SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write element count timeout."));
		//	WrapErrorMessage(builder, tempjson, outmsg, _T("Write element count timeout."));
		//	break;

		//}


		CFile file;
		const std::string file_output = Json::writeString(builder, json["data"]);
		CString file_temp_cs(file_output.c_str());
		file.Open(des_file, CFile::modeCreate | CFile::modeWrite | CFile::modeCreate, NULL);
		file.Write(file_temp_cs, file_temp_cs.GetLength() * 2);
		file.Close();
		Json::Value tempjson;
		tempjson["action"] = "SAVE_GRAPHIC_DATA_RES";
		tempjson["data"]["status"] = true;
		const std::string output = Json::writeString(builder, tempjson);
		CString temp_cs(output.c_str());

		//m_webView->PostWebMessageAsJson(temp_cs);
		outmsg = temp_cs;

		//JsonDataToStruct(file_output);
		bool zip_ret = ZipSingleItem(des_file_zip, des_file);
		if (!zip_ret)
		{
			if (msg_source == 0)
				SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Failed to store data!"));
			WrapErrorMessage(builder, tempjson, outmsg, _T("Failed to store data!"));
			break;
		}
		else
		{
			if (msg_source == 0)
				SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data into file success!"));
		}

		//这里涉及到写设备的动作 发消息放到线程中去做。避免界面卡死，以及影响其他消息收发;
		//这里要根据不同的panel 对不同设备进行保存 .
		int n_write_result = Write_Webview_Data_Special(panelId, grp_serial_number, save_grp_index, temp_elementcount);
		if (n_write_result == -3)
		{
			if (msg_source == 0)
				SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write too often. Ignore this request!"));
			WrapErrorMessage(builder, tempjson, outmsg, _T("Write too often. Ignore this request!"));
			break;
		}
		else if (n_write_result < 0)
		{
			if (msg_source == 0)
				SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Failed to store data!"));
			WrapErrorMessage(builder, tempjson, outmsg, _T("Failed to store data!"));
			break;
		}



		break;
	}
	case WEBVIEW_MESSAGE_TYPE::GET_WEBVIEW_LIST:
	{
		//接受 6个参数 panelId,serialNumber,entryType,entryIndexStart,entryIndexEnd,objectinstance
		//并返回对应的数据列表  阻塞的方式读取数据
		int temp_panel_id = json.get("panelId", Json::nullValue).asInt();
		int temp_serial_number = json.get("serialNumber", Json::nullValue).asInt();
		int entry_type = json.get("entryType", Json::nullValue).asInt();
		int entry_index_start = json.get("entryIndexStart", Json::nullValue).asInt();
		int entry_index_end = json.get("entryIndexEnd", Json::nullValue).asInt();
		int entry_objectinstance = json.get("objectinstance", Json::nullValue).asInt();
		// Log incoming request
		CString logMsg;
		logMsg.Format(_T("GET_WEBVIEW_LIST: PanelId=%d, SerialNumber=%d, EntryType=%d, StartIndex=%d, EndIndex=%d"),
			temp_panel_id, temp_serial_number, entry_type, entry_index_start, entry_index_end);
		WriteHandleWebViewMsgLog(_T("GET_WEBVIEW_LIST"), logMsg, 1);
		if ((temp_panel_id == 0) || (temp_panel_id >= 255))
		{
			if (msg_source == 0)
				SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Panel is invalid ."));
			WrapErrorMessage(builder, tempjson, outmsg, _T("Panel is invalid ."));
			CString errorLog;
			errorLog.Format(_T("ERROR: Invalid panel_id=%d"), temp_panel_id);
			WriteHandleWebViewMsgLog(_T("GET_WEBVIEW_LIST"), errorLog, 0);
			break;
		}

		if (g_Device_Basic_Setting[temp_panel_id].reg.n_serial_number != temp_serial_number)
		{
			if (msg_source == 0)
				SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("The serial number does not match the panel."));
			WrapErrorMessage(builder, tempjson, outmsg, _T("The serial number does not match the panel."));

			CString errorLog;
			errorLog.Format(_T("ERROR: Serial mismatch - Expected=%d, Got=%d"),
				g_Device_Basic_Setting[temp_panel_id].reg.n_serial_number, temp_serial_number);
			WriteHandleWebViewMsgLog(_T("GET_WEBVIEW_LIST"), errorLog, 0);
			break;
		}
		unsigned int temp_objectinstance = g_Device_Basic_Setting[temp_panel_id].reg.object_instance;
		if ((temp_objectinstance == 0) ||
			(temp_objectinstance >= 0X3FFFFF) ||
			(temp_objectinstance != entry_objectinstance))
		{
			if (msg_source == 0)
				SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("The objectinstance does not match the panel."));
			WrapErrorMessage(builder, tempjson, outmsg, _T("The objectinstance does not match the panel."));

			CString errorLog;
			errorLog.Format(_T("ERROR: object_instance mismatch - Expected=%d, Got=%d"),
				g_Device_Basic_Setting[temp_panel_id].reg.object_instance, entry_objectinstance);
			WriteHandleWebViewMsgLog(_T("GET_WEBVIEW_LIST"), errorLog, 0);
			break;
		}

		// Add device main info to data array
		tempjson["data"]["panelId"] = temp_panel_id;
		tempjson["data"]["serialNumber"] = temp_serial_number;
		tempjson["data"]["entryType"] = entry_type;
		tempjson["data"]["entryIndexStart"] = entry_index_start;
		tempjson["data"]["entryIndexEnd"] = entry_index_end;
		tempjson["data"]["objectinstance"] = entry_objectinstance;
		int point_idx = 0;
		switch (entry_type)
		{
			case BAC_IN:
			{
				if (entry_index_end >= entry_index_start)
				{
					// 计算需要几个块（向上取整）
					int totalCount = entry_index_end - entry_index_start + 1;
					int groupSize = BAC_READ_INPUT_GROUP_NUMBER;
					int read_input_group = (totalCount + groupSize - 1) / groupSize;

					for (int i = 0; i < read_input_group; ++i)
					{
						int temp_start = entry_index_start + i * groupSize;
						if (temp_start > entry_index_end) // 防止越界
							break;

						int temp_end = temp_start + groupSize - 1;
						if (temp_end > entry_index_end)
							temp_end = entry_index_end;

						// 再次确保不超过全局最大点数
						if (temp_start >= BAC_INPUT_ITEM_COUNT)
							break;
						if (temp_end >= BAC_INPUT_ITEM_COUNT)
							temp_end = BAC_INPUT_ITEM_COUNT - 1;

						// 示例：对每个块进行阻塞读取
						if (GetPrivateDataSaveSPBlocking(entry_objectinstance, READINPUT_T3000,
							(uint8_t)temp_start, (uint8_t)temp_end, sizeof(Str_in_point), 4) > 0)
						{
							// 将读取到的临时数据复制到全局缓存
							for (int idx = temp_start; idx <= temp_end; ++idx)
							{
								if (idx < BAC_INPUT_ITEM_COUNT) {
									g_Input_data[temp_panel_id].at(idx) = s_Input_data[idx];
								}
							}
							if ((debug_item_show == DEBUG_SHOW_MESSAGE_THREAD) || (debug_item_show == DEBUG_SHOW_ALL))
							{
								int elementCount = temp_end - temp_start + 1; // 本块实际元素个数
								// 调试输出（可选）
								CString dbg;
								dbg.Format(_T("obj=%d,input ,Chunk %d: temp_start=%d, temp_end=%d, count=%d\r\n"), entry_objectinstance, i, temp_start, temp_end, elementCount);
								DFTrace(dbg);
							}
							Sleep(SEND_COMMAND_DELAY_TIME);
						}
						else
						{
							CString errorLog;
							errorLog.Format(_T("ERROR: Read input failed start=%d, end=%d"),
								temp_start, temp_end);
							WriteHandleWebViewMsgLog(_T("GET_WEBVIEW_LIST"), errorLog, 0);

							if (msg_source == 0)
								SetPaneString(BAC_SHOW_MISSION_RESULTS, errorLog);
							WrapErrorMessage(builder, tempjson, outmsg, errorLog);
							if ((debug_item_show == DEBUG_SHOW_MESSAGE_THREAD) || (debug_item_show == DEBUG_SHOW_ALL))
							{
								DFTrace(errorLog);
							}
							continue;
						}

						int npanel_id = temp_panel_id;
						for (int idx = temp_start; idx < temp_end; idx++)
						{
							int input_idx = idx;
							tempjson["data"]["device_data"][point_idx]["pid"] = npanel_id;
							tempjson["data"]["device_data"][point_idx]["type"] = "INPUT";
							tempjson["data"]["device_data"][point_idx]["index"] = input_idx;
							tempjson["data"]["device_data"][point_idx]["id"] = "IN" + to_string(input_idx + 1);
							tempjson["data"]["device_data"][point_idx]["command"] = to_string(npanel_id) + "IN" + to_string(input_idx + 1);
							tempjson["data"]["device_data"][point_idx]["description"] = (char*)g_Input_data[npanel_id].at(input_idx).description;
							tempjson["data"]["device_data"][point_idx]["label"] = (char*)g_Input_data[npanel_id].at(input_idx).label;
							tempjson["data"]["device_data"][point_idx]["unit"] = g_Input_data[npanel_id].at(input_idx).range;
							tempjson["data"]["device_data"][point_idx]["auto_manual"] = g_Input_data[npanel_id].at(input_idx).auto_manual;
							tempjson["data"]["device_data"][point_idx]["value"] = g_Input_data[npanel_id].at(input_idx).value;
							tempjson["data"]["device_data"][point_idx]["filter"] = g_Input_data[npanel_id].at(input_idx).filter;
							tempjson["data"]["device_data"][point_idx]["control"] = g_Input_data[npanel_id].at(input_idx).control;
							tempjson["data"]["device_data"][point_idx]["digital_analog"] = g_Input_data[npanel_id].at(input_idx).digital_analog;
							tempjson["data"]["device_data"][point_idx]["range"] = g_Input_data[npanel_id].at(input_idx).range;
							tempjson["data"]["device_data"][point_idx]["calibration_sign"] = g_Input_data[npanel_id].at(input_idx).calibration_sign;
							tempjson["data"]["device_data"][point_idx]["calibration_h"] = g_Input_data[npanel_id].at(input_idx).calibration_h;
							tempjson["data"]["device_data"][point_idx]["calibration_l"] = g_Input_data[npanel_id].at(input_idx).calibration_l;
							tempjson["data"]["device_data"][point_idx]["decom"] = g_Input_data[npanel_id].at(input_idx).decom; //for input   0 "Normal"    1"Open"   2  "Shorted"
							point_idx++;
						}

					} // for groups
				}
				break;
			}
			case BAC_OUT:
			{
				if (entry_index_end >= entry_index_start)
				{
					// 计算需要几个块（向上取整）
					int totalCount = entry_index_end - entry_index_start + 1;
					int groupSize = BAC_READ_OUTPUT_GROUP_NUMBER;
					int read_output_group = (totalCount + groupSize - 1) / groupSize;

					for (int i = 0; i < read_output_group; ++i)
					{
						int temp_start = entry_index_start + i * groupSize;
						if (temp_start > entry_index_end) // 防止越界
							break;

						int temp_end = temp_start + groupSize - 1;
						if (temp_end > entry_index_end)
							temp_end = entry_index_end;

						// 再次确保不超过全局最大点数
						if (temp_start >= BAC_OUTPUT_ITEM_COUNT)
							break;
						if (temp_end >= BAC_OUTPUT_ITEM_COUNT)
							temp_end = BAC_OUTPUT_ITEM_COUNT - 1;

						// 示例：对每个块进行阻塞读取
						if (GetPrivateDataSaveSPBlocking(entry_objectinstance, READOUTPUT_T3000,
							(uint8_t)temp_start, (uint8_t)temp_end, sizeof(Str_out_point), 4) > 0)
						{
							// 将读取到的临时数据复制到全局缓存
							for (int idx = temp_start; idx <= temp_end; ++idx)
							{
								if (idx < BAC_OUTPUT_ITEM_COUNT) {
									g_Output_data[temp_panel_id].at(idx) = s_Output_data[idx];
								}
							}
							if ((debug_item_show == DEBUG_SHOW_MESSAGE_THREAD) || (debug_item_show == DEBUG_SHOW_ALL))
							{
								int elementCount = temp_end - temp_start + 1; // 本块实际元素个数
								// 调试输出（可选）
								CString dbg;
								dbg.Format(_T("obj=%d,output ,Chunk %d: temp_start=%d, temp_end=%d, count=%d\r\n"), entry_objectinstance, i, temp_start, temp_end, elementCount);
								DFTrace(dbg);
							}
							Sleep(SEND_COMMAND_DELAY_TIME);
						}
						else
						{
							CString errorLog;
							errorLog.Format(_T("ERROR: Read output failed start=%d, end=%d"),
								temp_start, temp_end);
							WriteHandleWebViewMsgLog(_T("GET_WEBVIEW_LIST"), errorLog, 0);

							if (msg_source == 0)
								SetPaneString(BAC_SHOW_MISSION_RESULTS, errorLog);
							WrapErrorMessage(builder, tempjson, outmsg, errorLog);
							if ((debug_item_show == DEBUG_SHOW_MESSAGE_THREAD) || (debug_item_show == DEBUG_SHOW_ALL))
							{
								DFTrace(errorLog);
							}
							continue;
						}

						int npanel_id = temp_panel_id;
						for (int idx = temp_start; idx < temp_end; idx++)
						{
							int output_idx = idx;
							tempjson["data"]["device_data"][point_idx]["pid"] = npanel_id;
							tempjson["data"]["device_data"][point_idx]["type"] = "OUTPUT";
							tempjson["data"]["device_data"][point_idx]["index"] = output_idx;
							tempjson["data"]["device_data"][point_idx]["id"] = "OUT" + to_string(output_idx + 1);
							tempjson["data"]["device_data"][point_idx]["command"] = to_string(npanel_id) + "OUT" + to_string(output_idx + 1);
							tempjson["data"]["device_data"][point_idx]["description"] = (char*)g_Output_data[npanel_id].at(output_idx).description;
							tempjson["data"]["device_data"][point_idx]["label"] = (char*)g_Output_data[npanel_id].at(output_idx).label;
							tempjson["data"]["device_data"][point_idx]["auto_manual"] = g_Output_data[npanel_id].at(output_idx).auto_manual;
							tempjson["data"]["device_data"][point_idx]["value"] = g_Output_data[npanel_id].at(output_idx).value;
							tempjson["data"]["device_data"][point_idx]["low_voltage"] = g_Output_data[npanel_id].at(output_idx).low_voltage;
							tempjson["data"]["device_data"][point_idx]["high_voltage"] = g_Output_data[npanel_id].at(output_idx).high_voltage;
							tempjson["data"]["device_data"][point_idx]["range"] = g_Output_data[npanel_id].at(output_idx).range;
							tempjson["data"]["device_data"][point_idx]["control"] = g_Output_data[npanel_id].at(output_idx).control;
							tempjson["data"]["device_data"][point_idx]["digital_analog"] = g_Output_data[npanel_id].at(output_idx).digital_analog;
							tempjson["data"]["device_data"][point_idx]["hw_switch_status"] = g_Output_data[npanel_id].at(output_idx).hw_switch_status;
							tempjson["data"]["device_data"][point_idx]["decom"] = g_Output_data[npanel_id].at(output_idx).decom; //for output   0 "Normal"    1"Alarm"
							point_idx++;
						}
					} // for groups
				}
				break;
			}
			case BAC_VAR:
			{
				if (entry_index_end >= entry_index_start)
				{
					// 计算需要几个块（向上取整）
					int totalCount = entry_index_end - entry_index_start + 1;
					int groupSize = BAC_READ_VARIABLE_GROUP_NUMBER;
					int read_variable_group = (totalCount + groupSize - 1) / groupSize;

					for (int i = 0; i < read_variable_group; ++i)
					{
						int temp_start = entry_index_start + i * groupSize;
						if (temp_start > entry_index_end) // 防止越界
							break;

						int temp_end = temp_start + groupSize - 1;
						if (temp_end > entry_index_end)
							temp_end = entry_index_end;

						// 再次确保不超过全局最大点数
						if (temp_start >= BAC_VARIABLE_ITEM_COUNT)
							break;
						if (temp_end >= BAC_VARIABLE_ITEM_COUNT)
							temp_end = BAC_VARIABLE_ITEM_COUNT - 1;

						// 示例：对每个块进行阻塞读取
						if (GetPrivateDataSaveSPBlocking(entry_objectinstance, READVARIABLE_T3000,
							(uint8_t)temp_start, (uint8_t)temp_end, sizeof(Str_variable_point), 4) > 0)
						{
							// 将读取到的临时数据复制到全局缓存
							for (int idx = temp_start; idx <= temp_end; ++idx)
							{
								if (idx < BAC_VARIABLE_ITEM_COUNT) {
									g_Variable_data[temp_panel_id].at(idx) = s_Variable_data[idx];
								}
							}
							if ((debug_item_show == DEBUG_SHOW_MESSAGE_THREAD) || (debug_item_show == DEBUG_SHOW_ALL))
							{
								int elementCount = temp_end - temp_start + 1; // 本块实际元素个数
								// 调试输出（可选）
								CString dbg;
								dbg.Format(_T("obj=%d,variable ,Chunk %d: temp_start=%d, temp_end=%d, count=%d\r\n"), entry_objectinstance, i, temp_start, temp_end, elementCount);
								DFTrace(dbg);
							}
							Sleep(SEND_COMMAND_DELAY_TIME);
						}
						else
						{
							CString errorLog;
							errorLog.Format(_T("ERROR: Read variable failed start=%d, end=%d"),
								temp_start, temp_end);
							WriteHandleWebViewMsgLog(_T("GET_WEBVIEW_LIST"), errorLog, 0);

							if (msg_source == 0)
								SetPaneString(BAC_SHOW_MISSION_RESULTS, errorLog);
							WrapErrorMessage(builder, tempjson, outmsg, errorLog);
							if ((debug_item_show == DEBUG_SHOW_MESSAGE_THREAD) || (debug_item_show == DEBUG_SHOW_ALL))
							{
								DFTrace(errorLog);
							}
							continue;
						}

						int npanel_id = temp_panel_id;
						for (int idx = temp_start; idx < temp_end; idx++)
						{
							int var_idx = idx;
							tempjson["data"]["device_data"][point_idx]["pid"] = npanel_id;
							tempjson["data"]["device_data"][point_idx]["type"] = "VARIABLE";
							tempjson["data"]["device_data"][point_idx]["index"] = var_idx;
							tempjson["data"]["device_data"][point_idx]["id"] = "VAR" + to_string(var_idx + 1);
							tempjson["data"]["device_data"][point_idx]["command"] = to_string(npanel_id) + "VAR" + to_string(var_idx + 1);
							tempjson["data"]["device_data"][point_idx]["description"] = (char*)g_Variable_data[npanel_id].at(var_idx).description;
							tempjson["data"]["device_data"][point_idx]["label"] = (char*)g_Variable_data[npanel_id].at(var_idx).label;
							tempjson["data"]["device_data"][point_idx]["auto_manual"] = g_Variable_data[npanel_id].at(var_idx).auto_manual;
							tempjson["data"]["device_data"][point_idx]["value"] = g_Variable_data[npanel_id].at(var_idx).value;
							tempjson["data"]["device_data"][point_idx]["range"] = g_Variable_data[npanel_id].at(var_idx).range;
							tempjson["data"]["device_data"][point_idx]["control"] = g_Variable_data[npanel_id].at(var_idx).control;
							tempjson["data"]["device_data"][point_idx]["digital_analog"] = g_Variable_data[npanel_id].at(var_idx).digital_analog;
							tempjson["data"]["device_data"][point_idx]["unused"] = g_Variable_data[npanel_id].at(var_idx).unused;
							point_idx++;
						}
					} // for groups
				}
				break;
			}
			case BAC_PRG:
			{
				if (entry_index_end >= entry_index_start)
				{
					// Calculate number of groups needed (round up)
					int totalCount = entry_index_end - entry_index_start + 1;
					int groupSize = BAC_READ_PROGRAM_GROUP_NUMBER;
					int read_program_group = (totalCount + groupSize - 1) / groupSize;

					for (int i = 0; i < read_program_group; ++i)
					{
						int temp_start = entry_index_start + i * groupSize;
						if (temp_start > entry_index_end) // Prevent overflow
							break;

						int temp_end = temp_start + groupSize - 1;
						if (temp_end > entry_index_end)
							temp_end = entry_index_end;

						// Ensure not exceeding global max count
						if (temp_start >= BAC_PROGRAM_ITEM_COUNT)
							break;
						if (temp_end >= BAC_PROGRAM_ITEM_COUNT)
							temp_end = BAC_PROGRAM_ITEM_COUNT - 1;

						// Blocking read for each group
						if (GetPrivateDataSaveSPBlocking(entry_objectinstance, READPROGRAM_T3000,
							(uint8_t)temp_start, (uint8_t)temp_end, sizeof(Str_program_point), 4) > 0)
						{
							// Copy read data to global cache
							for (int idx = temp_start; idx <= temp_end; ++idx)
							{
								if (idx < BAC_PROGRAM_ITEM_COUNT) 
								{
									memcpy(&g_Program_data[temp_panel_id].at(idx), &s_Program_data[idx], sizeof(Str_program_point));
								}
							}
							if ((debug_item_show == DEBUG_SHOW_MESSAGE_THREAD) || (debug_item_show == DEBUG_SHOW_ALL))
							{
								int elementCount = temp_end - temp_start + 1;
								CString dbg;
								dbg.Format(_T("obj=%d,program ,Chunk %d: temp_start=%d, temp_end=%d, count=%d\r\n"),
									entry_objectinstance, i, temp_start, temp_end, elementCount);
								DFTrace(dbg);
							}
							Sleep(SEND_COMMAND_DELAY_TIME);
						}
						else
						{
							CString errorLog;
							errorLog.Format(_T("ERROR: Read program failed start=%d, end=%d"),
								temp_start, temp_end);
							WriteHandleWebViewMsgLog(_T("GET_WEBVIEW_LIST"), errorLog, 0);

							if (msg_source == 0)
								SetPaneString(BAC_SHOW_MISSION_RESULTS, errorLog);
							WrapErrorMessage(builder, tempjson, outmsg, errorLog);
							if ((debug_item_show == DEBUG_SHOW_MESSAGE_THREAD) || (debug_item_show == DEBUG_SHOW_ALL))
							{
								DFTrace(errorLog);
							}
							continue;
						}

						int npanel_id = temp_panel_id;
						for (int idx = temp_start; idx <= temp_end; idx++)
						{
							int prg_idx = idx;
							tempjson["data"]["device_data"][point_idx]["pid"] = npanel_id;
							tempjson["data"]["device_data"][point_idx]["type"] = "PROGRAM";
							tempjson["data"]["device_data"][point_idx]["index"] = prg_idx;
							tempjson["data"]["device_data"][point_idx]["id"] = "PRG" + to_string(prg_idx + 1);
							tempjson["data"]["device_data"][point_idx]["command"] = to_string(npanel_id) + "PRG" + to_string(prg_idx + 1);
							tempjson["data"]["device_data"][point_idx]["description"] = (char*)g_Program_data[npanel_id].at(prg_idx).description;
							tempjson["data"]["device_data"][point_idx]["label"] = (char*)g_Program_data[npanel_id].at(prg_idx).label;
							tempjson["data"]["device_data"][point_idx]["auto_manual"] = g_Program_data[npanel_id].at(prg_idx).auto_manual;
							tempjson["data"]["device_data"][point_idx]["status"] = g_Program_data[npanel_id].at(prg_idx).on_off;
							tempjson["data"]["device_data"][point_idx]["unused"] = g_Program_data[npanel_id].at(prg_idx).unused;
							point_idx++;
						}

					} // for groups
				}
				break;
			}

			case BAC_GRP:  // Graphics/Screens
			{
				if (entry_index_end >= entry_index_start)
				{
					// Calculate number of groups needed (round up)
					int totalCount = entry_index_end - entry_index_start + 1;
					int groupSize = BAC_READ_SCREEN_GROUP_NUMBER;
					int read_screen_group = (totalCount + groupSize - 1) / groupSize;

					for (int i = 0; i < read_screen_group; ++i)
					{
						int temp_start = entry_index_start + i * groupSize;
						if (temp_start > entry_index_end) // Prevent overflow
							break;

						int temp_end = temp_start + groupSize - 1;
						if (temp_end > entry_index_end)
							temp_end = entry_index_end;

						// Ensure not exceeding global max count
						if (temp_start >= BAC_SCREEN_COUNT)
							break;
						if (temp_end >= BAC_SCREEN_COUNT)
							temp_end = BAC_SCREEN_COUNT - 1;

						// Blocking read for each group
						if (GetPrivateDataSaveSPBlocking(entry_objectinstance, READSCREEN_T3000,
							(uint8_t)temp_start, (uint8_t)temp_end, sizeof(Control_group_point), 4) > 0)
						{
							// Copy read data to global cache
							for (int idx = temp_start; idx <= temp_end; ++idx)
							{
								if (idx < BAC_SCREEN_COUNT) 
								{
									memcpy(&g_screen_data[temp_panel_id].at(idx), &s_screen_data[idx], sizeof(Control_group_point));
									if ((debug_item_show == DEBUG_SHOW_MESSAGE_THREAD) || (debug_item_show == DEBUG_SHOW_ALL))
									{
										CString dbg;
										dbg.Format(_T("obj=%d,screen %d: temp_start=%d, temp_end=%d,element count=%d\r\n"),
											entry_objectinstance, i, idx, idx, s_screen_data[idx].webview_element_count);
										DFTrace(dbg);
									}
								}
							}

							Sleep(SEND_COMMAND_DELAY_TIME);
						}
						else
						{
							CString errorLog;
							errorLog.Format(_T("ERROR: Read screen failed start=%d, end=%d"),
								temp_start, temp_end);
							WriteHandleWebViewMsgLog(_T("GET_WEBVIEW_LIST"), errorLog, 0);

							if (msg_source == 0)
								SetPaneString(BAC_SHOW_MISSION_RESULTS, errorLog);
							WrapErrorMessage(builder, tempjson, outmsg, errorLog);
							if ((debug_item_show == DEBUG_SHOW_MESSAGE_THREAD) || (debug_item_show == DEBUG_SHOW_ALL))
							{
								DFTrace(errorLog);
							}
							continue;
						}

						int npanel_id = temp_panel_id;
						for (int idx = temp_start; idx <= temp_end; idx++)
						{
							int grp_idx = idx;
							tempjson["data"]["device_data"][point_idx]["pid"] = npanel_id;
							tempjson["data"]["device_data"][point_idx]["type"] = "GRP";
							tempjson["data"]["device_data"][point_idx]["index"] = grp_idx;
							tempjson["data"]["device_data"][point_idx]["id"] = "GRP" + to_string(grp_idx + 1);
							tempjson["data"]["device_data"][point_idx]["command"] = to_string(npanel_id) + "GRP" + to_string(grp_idx + 1);
							tempjson["data"]["device_data"][point_idx]["description"] = (char*)g_screen_data[npanel_id].at(grp_idx).description;
							tempjson["data"]["device_data"][point_idx]["label"] = (char*)g_screen_data[npanel_id].at(grp_idx).label;
							tempjson["data"]["device_data"][point_idx]["count"] = g_screen_data[npanel_id].at(grp_idx).webview_element_count;
							point_idx++;
						}

					} // for groups
				}
				break;
			}

			case BAC_PID:  // PID Controllers / Loops
			{
				if (entry_index_end >= entry_index_start)
				{
					// Calculate number of groups needed (round up)
					int totalCount = entry_index_end - entry_index_start + 1;
					int groupSize = BAC_READ_PID_GROUP_NUMBER;  // 10
					int read_pid_group = (totalCount + groupSize - 1) / groupSize;

					for (int i = 0; i < read_pid_group; ++i)
					{
						int temp_start = entry_index_start + i * groupSize;
						if (temp_start > entry_index_end) // Prevent overflow
							break;

						int temp_end = temp_start + groupSize - 1;
						if (temp_end > entry_index_end)
							temp_end = entry_index_end;

						// Ensure not exceeding global max count
						if (temp_start >= BAC_PID_COUNT)
							break;
						if (temp_end >= BAC_PID_COUNT)
							temp_end = BAC_PID_COUNT - 1;

						// Blocking read for each group
						if (GetPrivateDataSaveSPBlocking(entry_objectinstance, READCONTROLLER_T3000,
							(uint8_t)temp_start, (uint8_t)temp_end, sizeof(Str_controller_point), 4) > 0)
						{
							// Copy read data to global cache
							for (int idx = temp_start; idx <= temp_end; ++idx)
							{
								if (idx < BAC_PID_COUNT) 
								{
									memcpy(&g_controller_data[temp_panel_id].at(idx), &s_controller_data[idx], sizeof(Str_controller_point));
									if ((debug_item_show == DEBUG_SHOW_MESSAGE_THREAD) || (debug_item_show == DEBUG_SHOW_ALL))
									{
										int elementCount = temp_end - temp_start + 1;
										CString dbg;
										dbg.Format(_T("obj=%d,controller ,Chunk %d: temp_start=%d, temp_end=%d\r\n"),
											entry_objectinstance, idx, idx, idx);
										DFTrace(dbg);
									}
								}
							}

							Sleep(SEND_COMMAND_DELAY_TIME);
						}
						else
						{
							CString errorLog;
							errorLog.Format(_T("ERROR: Read controller failed start=%d, end=%d"),
								temp_start, temp_end);
							WriteHandleWebViewMsgLog(_T("GET_WEBVIEW_LIST"), errorLog, 0);

							if (msg_source == 0)
								SetPaneString(BAC_SHOW_MISSION_RESULTS, errorLog);
							WrapErrorMessage(builder, tempjson, outmsg, errorLog);
							if ((debug_item_show == DEBUG_SHOW_MESSAGE_THREAD) || (debug_item_show == DEBUG_SHOW_ALL))
							{
								DFTrace(errorLog);
							}
							continue;
						}

						int npanel_id = temp_panel_id;
						for (int idx = temp_start; idx <= temp_end; idx++)
						{
							int pid_idx = idx;
							tempjson["data"]["device_data"][point_idx]["pid"] = npanel_id;
							tempjson["data"]["device_data"][point_idx]["type"] = "PID";
							tempjson["data"]["device_data"][point_idx]["index"] = pid_idx;
							tempjson["data"]["device_data"][point_idx]["id"] = "PID" + to_string(pid_idx + 1);
							tempjson["data"]["device_data"][point_idx]["command"] = to_string(npanel_id) + "PID" + to_string(pid_idx + 1);

							// PID Controller specific fields
							tempjson["data"]["device_data"][point_idx]["input_panel"] = g_controller_data[npanel_id].at(pid_idx).input.panel;
							tempjson["data"]["device_data"][point_idx]["input_type"] = g_controller_data[npanel_id].at(pid_idx).input.point_type;
							tempjson["data"]["device_data"][point_idx]["input_number"] = g_controller_data[npanel_id].at(pid_idx).input.number;
							tempjson["data"]["device_data"][point_idx]["input_value"] = g_controller_data[npanel_id].at(pid_idx).input_value;
							tempjson["data"]["device_data"][point_idx]["units"] = g_controller_data[npanel_id].at(pid_idx).units;
							tempjson["data"]["device_data"][point_idx]["auto_manual"] = g_controller_data[npanel_id].at(pid_idx).auto_manual;
							tempjson["data"]["device_data"][point_idx]["output_value"] = g_controller_data[npanel_id].at(pid_idx).value;
							tempjson["data"]["device_data"][point_idx]["setpoint_panel"] = g_controller_data[npanel_id].at(pid_idx).setpoint.panel;
							tempjson["data"]["device_data"][point_idx]["setpoint_type"] = g_controller_data[npanel_id].at(pid_idx).setpoint.point_type;
							tempjson["data"]["device_data"][point_idx]["setpoint_number"] = g_controller_data[npanel_id].at(pid_idx).setpoint.number;
							tempjson["data"]["device_data"][point_idx]["time_type"] = g_controller_data[npanel_id].at(pid_idx).repeats_per_min;
							tempjson["data"]["device_data"][point_idx]["action"] = g_controller_data[npanel_id].at(pid_idx).action;
							tempjson["data"]["device_data"][point_idx]["proportional"] = g_controller_data[npanel_id].at(pid_idx).proportional;
							tempjson["data"]["device_data"][point_idx]["integral"] = g_controller_data[npanel_id].at(pid_idx).reset;
							tempjson["data"]["device_data"][point_idx]["differential"] = g_controller_data[npanel_id].at(pid_idx).rate;
							tempjson["data"]["device_data"][point_idx]["bias"] = g_controller_data[npanel_id].at(pid_idx).bias;

							point_idx++;
						}

					} // for groups
				}
				break;
			}

			case BAC_SCH:  // Schedules / Weekly Routines
			{
				if (entry_index_end >= entry_index_start)
				{
					// Calculate number of groups needed (round up)
					int totalCount = entry_index_end - entry_index_start + 1;
					int groupSize = BAC_READ_SCHEDULE_GROUP_NUMBER;  // 10
					int read_schedule_group = (totalCount + groupSize - 1) / groupSize;

					for (int i = 0; i < read_schedule_group; ++i)
					{
						int temp_start = entry_index_start + i * groupSize;
						if (temp_start > entry_index_end) // Prevent overflow
							break;

						int temp_end = temp_start + groupSize - 1;
						if (temp_end > entry_index_end)
							temp_end = entry_index_end;

						// Ensure not exceeding global max count
						if (temp_start >= BAC_SCHEDULE_COUNT)
							break;
						if (temp_end >= BAC_SCHEDULE_COUNT)
							temp_end = BAC_SCHEDULE_COUNT - 1;

						// Blocking read for each group
						if (GetPrivateDataSaveSPBlocking(entry_objectinstance, READWEEKLYROUTINE_T3000,
							(uint8_t)temp_start, (uint8_t)temp_end, sizeof(Str_weekly_routine_point), 4) > 0)
						{
							// Copy read data to global cache
							for (int idx = temp_start; idx <= temp_end; ++idx)
							{
								if (idx < BAC_SCHEDULE_COUNT) 
								{
									memcpy(&g_Weekly_data[temp_panel_id].at(idx), &s_Weekly_data[idx], sizeof(Str_weekly_routine_point));
									if ((debug_item_show == DEBUG_SHOW_MESSAGE_THREAD) || (debug_item_show == DEBUG_SHOW_ALL))
									{
										CString dbg;
										dbg.Format(_T("obj=%d, temp_start=%d, temp_end=%d\r\n"),
											entry_objectinstance, idx, idx);
										DFTrace(dbg);
									}
								}
							}

							Sleep(SEND_COMMAND_DELAY_TIME);
						}
						else
						{
							CString errorLog;
							errorLog.Format(_T("ERROR: Read schedule failed start=%d, end=%d"),
								temp_start, temp_end);
							WriteHandleWebViewMsgLog(_T("GET_WEBVIEW_LIST"), errorLog, 0);

							if (msg_source == 0)
								SetPaneString(BAC_SHOW_MISSION_RESULTS, errorLog);
							WrapErrorMessage(builder, tempjson, outmsg, errorLog);
							if ((debug_item_show == DEBUG_SHOW_MESSAGE_THREAD) || (debug_item_show == DEBUG_SHOW_ALL))
							{
								DFTrace(errorLog);
							}
							continue;
						}

						int npanel_id = temp_panel_id;
						for (int idx = temp_start; idx <= temp_end; idx++)
						{
							int sch_idx = idx;
							tempjson["data"]["device_data"][point_idx]["pid"] = npanel_id;
							tempjson["data"]["device_data"][point_idx]["type"] = "SCHEDULE";
							tempjson["data"]["device_data"][point_idx]["index"] = sch_idx;
							tempjson["data"]["device_data"][point_idx]["id"] = "SCH" + to_string(sch_idx + 1);
							tempjson["data"]["device_data"][point_idx]["command"] = to_string(npanel_id) + "SCH" + to_string(sch_idx + 1);

							// Schedule specific fields (matching GET_PANEL_DATA structure)
							tempjson["data"]["device_data"][point_idx]["description"] = (char*)g_Weekly_data[npanel_id].at(sch_idx).description;
							tempjson["data"]["device_data"][point_idx]["label"] = (char*)g_Weekly_data[npanel_id].at(sch_idx).label;
							tempjson["data"]["device_data"][point_idx]["auto_manual"] = g_Weekly_data[npanel_id].at(sch_idx).auto_manual;
							tempjson["data"]["device_data"][point_idx]["output"] = g_Weekly_data[npanel_id].at(sch_idx).value;
							tempjson["data"]["device_data"][point_idx]["state1"] = g_Weekly_data[npanel_id].at(sch_idx).override_1_value;
							tempjson["data"]["device_data"][point_idx]["state2"] = g_Weekly_data[npanel_id].at(sch_idx).override_2_value;
							tempjson["data"]["device_data"][point_idx]["unused"] = g_Weekly_data[npanel_id].at(sch_idx).unused;

							point_idx++;
						}

					} // for groups
				}
				break;
			}

			case BAC_HOL:  // Holidays / Annual Routines
			{
				if (entry_index_end >= entry_index_start)
				{
					// Calculate number of groups needed (round up)
					int totalCount = entry_index_end - entry_index_start + 1;
					int groupSize = BAC_READ_HOLIDAY_GROUP_NUMBER;  // 8
					int read_holiday_group = (totalCount + groupSize - 1) / groupSize;

					for (int i = 0; i < read_holiday_group; ++i)
					{
						int temp_start = entry_index_start + i * groupSize;
						if (temp_start > entry_index_end) // Prevent overflow
							break;

						int temp_end = temp_start + groupSize - 1;
						if (temp_end > entry_index_end)
							temp_end = entry_index_end;

						// Ensure not exceeding global max count
						if (temp_start >= BAC_HOLIDAY_COUNT)
							break;
						if (temp_end >= BAC_HOLIDAY_COUNT)
							temp_end = BAC_HOLIDAY_COUNT - 1;

						// Blocking read for each group
						if (GetPrivateDataSaveSPBlocking(entry_objectinstance, READANNUALROUTINE_T3000,
							(uint8_t)temp_start, (uint8_t)temp_end, sizeof(Str_annual_routine_point), 4) > 0)
						{
							// Copy read data to global cache
							for (int idx = temp_start; idx <= temp_end; ++idx)
							{
								if (idx < BAC_HOLIDAY_COUNT) 
								{
									memcpy(&g_Annual_data[temp_panel_id].at(idx), &s_Annual_data[idx], sizeof(Str_annual_routine_point));
									if ((debug_item_show == DEBUG_SHOW_MESSAGE_THREAD) || (debug_item_show == DEBUG_SHOW_ALL))
									{
										int elementCount = temp_end - temp_start + 1;
										CString dbg;
										dbg.Format(_T("obj=%d,holiday , temp_start=%d, temp_end=%d\r\n"),
											entry_objectinstance, idx, idx);
										DFTrace(dbg);
									}
								}
							}

							Sleep(SEND_COMMAND_DELAY_TIME);
						}
						else
						{
							CString errorLog;
							errorLog.Format(_T("ERROR: Read holiday failed start=%d, end=%d"),
								temp_start, temp_end);
							WriteHandleWebViewMsgLog(_T("GET_WEBVIEW_LIST"), errorLog, 0);

							if (msg_source == 0)
								SetPaneString(BAC_SHOW_MISSION_RESULTS, errorLog);
							WrapErrorMessage(builder, tempjson, outmsg, errorLog);
							if ((debug_item_show == DEBUG_SHOW_MESSAGE_THREAD) || (debug_item_show == DEBUG_SHOW_ALL))
							{
								DFTrace(errorLog);
							}
							continue;
						}

						int npanel_id = temp_panel_id;
						for (int idx = temp_start; idx <= temp_end; idx++)
						{
							int hol_idx = idx;
							tempjson["data"]["device_data"][point_idx]["pid"] = npanel_id;
							tempjson["data"]["device_data"][point_idx]["type"] = "HOLIDAY";
							tempjson["data"]["device_data"][point_idx]["index"] = hol_idx;
							tempjson["data"]["device_data"][point_idx]["id"] = "CAL" + to_string(hol_idx + 1);
							tempjson["data"]["device_data"][point_idx]["command"] = to_string(npanel_id) + "CAL" + to_string(hol_idx + 1);

							// Holiday specific fields (matching GET_PANEL_DATA structure)
							tempjson["data"]["device_data"][point_idx]["description"] = (char*)g_Annual_data[npanel_id].at(hol_idx).description;
							tempjson["data"]["device_data"][point_idx]["label"] = (char*)g_Annual_data[npanel_id].at(hol_idx).label;
							tempjson["data"]["device_data"][point_idx]["auto_manual"] = g_Annual_data[npanel_id].at(hol_idx).auto_manual;
							tempjson["data"]["device_data"][point_idx]["value"] = g_Annual_data[npanel_id].at(hol_idx).value;
							tempjson["data"]["device_data"][point_idx]["unused"] = g_Annual_data[npanel_id].at(hol_idx).unused;

							point_idx++;
						}

					} // for groups
				}
				break;
			}

			case READ_SETTING_COMMAND:
			{
				if ((entry_index_end == 0) && (entry_index_start == 0))
				{
						// 示例：对每个块进行阻塞读取
						if (GetPrivateDataSaveSPBlocking(entry_objectinstance, READ_SETTING_COMMAND,
							entry_index_start, entry_index_end, sizeof(Str_Setting_Info), 4) > 0)
						{

							// 将读取到的临时数据复制到全局缓存
							memcpy(&g_Device_Basic_Setting[temp_panel_id], &s_Basic_Setting, sizeof(Str_Setting_Info));

							if ((debug_item_show == DEBUG_SHOW_MESSAGE_THREAD) || (debug_item_show == DEBUG_SHOW_ALL))
							{
								CString dbg;
								dbg.Format(_T("obj=%d, Setting config info \r\n"), entry_objectinstance);
								DFTrace(dbg);
							}
							Sleep(SEND_COMMAND_DELAY_TIME);
						}
						else
						{
							CString errorLog;
							errorLog.Format(_T("ERROR: Read Setting config info failed start=%d, end=%d"),
								entry_index_start, entry_index_end);
							WriteHandleWebViewMsgLog(_T("GET_WEBVIEW_LIST"), errorLog, 0);

							if (msg_source == 0)
								SetPaneString(BAC_SHOW_MISSION_RESULTS, errorLog);
							WrapErrorMessage(builder, tempjson, outmsg, errorLog);
							if ((debug_item_show == DEBUG_SHOW_MESSAGE_THREAD) || (debug_item_show == DEBUG_SHOW_ALL))
							{
								DFTrace(errorLog);
							}
							break;
						}

						int npanel_id = temp_panel_id;
						int input_idx = 0;
						tempjson["data"]["device_data"][point_idx]["pid"] = npanel_id;
						tempjson["data"]["device_data"][point_idx]["type"] = "SETTING";
						tempjson["data"]["device_data"][point_idx]["index"] = input_idx;
						tempjson["data"]["device_data"][point_idx]["id"] = "SETTING" + to_string(input_idx + 1);

						for (int m = 0; m < 400; m++)
						{
							tempjson["data"]["device_data"][point_idx]["All"][m] = Device_Basic_Setting.all[m];
						}point_idx++;


				} // for groups
				
				break;
			}
			default:
				break;
		}


		// Generate final JSON response once for all devices
		const std::string output = Json::writeString(builder, tempjson);
		CString temp_cs(output.c_str());
		outmsg = temp_cs;

		break;
	}
	case WEBVIEW_MESSAGE_TYPE::UPDATE_WEBVIEW_LIST:
	{
		int temp_panel_id = json.get("panelId", Json::nullValue).asInt();
		int temp_serial_number = json.get("serialNumber", Json::nullValue).asInt();
		int entry_type = json.get("entryType", Json::nullValue).asInt();
		int entry_index = json.get("entryIndex", Json::nullValue).asInt();

		// Log incoming request
		CString logMsg;
		logMsg.Format(_T("UPDATE_WEBVIEW_LIST: PanelId=%d, SerialNumber=%d, EntryType=%d, Index=%d"),
			temp_panel_id, temp_serial_number, entry_type, entry_index);
		WriteHandleWebViewMsgLog(_T("UPDATE_WEBVIEW_LIST"), logMsg, 1);


		if ((temp_panel_id == 0) || (temp_panel_id >= 255))
		{
			if (msg_source == 0)
				SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Panel is invalid ."));
			WrapErrorMessage(builder, tempjson, outmsg, _T("Panel is invalid ."));
			CString errorLog;
			errorLog.Format(_T("ERROR: Invalid panel_id=%d"), temp_panel_id);
			WriteHandleWebViewMsgLog(_T("UPDATE_WEBVIEW_LIST"), errorLog, 0);
			break;

		}

		switch (entry_type)
		{
		case BAC_IN:
		{

			if ((entry_index >= 0) && entry_index + 1 > BAC_INPUT_ITEM_COUNT)
			{
				if (msg_source == 0)
					SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Index is invalid."));
				WrapErrorMessage(builder, tempjson, outmsg, _T("Index is invalid."));
				CString errorLog;
				errorLog.Format(_T("ERROR: Invalid entry_index=%d (max=%d)"), entry_index, BAC_INPUT_ITEM_COUNT);
				WriteHandleWebViewMsgLog(_T("UPDATE_WEBVIEW_LIST"), errorLog, 0);

				break;
			}
			// Log field values before update
			CString beforeLog;
			beforeLog.Format(_T("INPUT[%d] BEFORE: description='%s', label='%s', value=%d, range=%d"),
				entry_index,
				(LPCTSTR)CString((char*)g_Input_data[temp_panel_id].at(entry_index).description),
				(LPCTSTR)CString((char*)g_Input_data[temp_panel_id].at(entry_index).label),
				g_Input_data[temp_panel_id].at(entry_index).value,
				g_Input_data[temp_panel_id].at(entry_index).range);
			WriteHandleWebViewMsgLog(_T("UPDATE_WEBVIEW_LIST"), beforeLog, 0);


			g_Input_data[temp_panel_id].at(entry_index).control = json["control"].asInt();
			g_Input_data[temp_panel_id].at(entry_index).value = json["value"].asFloat() * 1000;
			strncpy((char*)g_Input_data[temp_panel_id].at(entry_index).description, json["description"].asCString(), STR_IN_DESCRIPTION_LENGTH);
			strncpy((char*)g_Input_data[temp_panel_id].at(entry_index).label, json["label"].asCString(), STR_IN_LABEL);
			g_Input_data[temp_panel_id].at(entry_index).range = json["range"].asInt();
			g_Input_data[temp_panel_id].at(entry_index).auto_manual = json["auto_manual"].asInt();
			g_Input_data[temp_panel_id].at(entry_index).filter = json["filter"].asInt();
			g_Input_data[temp_panel_id].at(entry_index).control = json["control"].asInt();
			g_Input_data[temp_panel_id].at(entry_index).digital_analog = json["digital_analog"].asInt();
			g_Input_data[temp_panel_id].at(entry_index).calibration_sign = json["calibration_sign"].asInt();
			g_Input_data[temp_panel_id].at(entry_index).calibration_h = json["calibration_h"].asInt();
			g_Input_data[temp_panel_id].at(entry_index).calibration_l = json["calibration_l"].asInt();
			g_Input_data[temp_panel_id].at(entry_index).decom = json["decom"].asInt();

			// Log field values after update
			CString afterLog;
			afterLog.Format(_T("INPUT[%d] AFTER: description='%s', label='%s', value=%d, range=%d"),
				entry_index,
				(LPCTSTR)CString((char*)g_Input_data[temp_panel_id].at(entry_index).description),
				(LPCTSTR)CString((char*)g_Input_data[temp_panel_id].at(entry_index).label),
				g_Input_data[temp_panel_id].at(entry_index).value,
				g_Input_data[temp_panel_id].at(entry_index).range);
			WriteHandleWebViewMsgLog(_T("UPDATE_WEBVIEW_LIST"), afterLog, 0);


			if (g_Device_Basic_Setting[temp_panel_id].reg.n_serial_number != temp_serial_number)
			{
				if (msg_source == 0)
					SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("The serial number does not match the panel."));
				WrapErrorMessage(builder, tempjson, outmsg, _T("The serial number does not match the panel."));

				CString errorLog;
				errorLog.Format(_T("ERROR: Serial mismatch - Expected=%d, Got=%d"),
					g_Device_Basic_Setting[temp_panel_id].reg.n_serial_number, temp_serial_number);
				WriteHandleWebViewMsgLog(_T("UPDATE_WEBVIEW_LIST"), errorLog, 0);
				break;
			}
			unsigned int temp_objectinstance = g_Device_Basic_Setting[temp_panel_id].reg.object_instance;

			// Log write attempt
			CString writeLog;
			writeLog.Format(_T("Writing to device: ObjectInstance=%d, Type=INPUT, Index=%d"),
				temp_objectinstance, entry_index);
			WriteHandleWebViewMsgLog(_T("UPDATE_WEBVIEW_LIST"), writeLog, 0);


			int ret_results = WritePrivateData_Blocking(temp_objectinstance, WRITEINPUT_T3000, entry_index, entry_index, 4, (char*)&g_Input_data[temp_panel_id].at(entry_index));
			if (ret_results > 0)
			{

			}
			else
			{
				WrapErrorMessage(builder, tempjson, outmsg, _T("Write data timeout."));
				break;
			}
			break;
		}
		case BAC_OUT:
		{
			if ((entry_index >= 0) && entry_index + 1 > BAC_OUTPUT_ITEM_COUNT)
			{
				if (msg_source == 0)
					SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Index is invalid."));
				WrapErrorMessage(builder, tempjson, outmsg, _T("Index is invalid."));
				break;
			}

			g_Output_data[temp_panel_id].at(entry_index).control = json["control"].asInt();
			g_Output_data[temp_panel_id].at(entry_index).value = json["value"].asFloat() * 1000;
			strncpy((char*)g_Output_data[temp_panel_id].at(entry_index).description, json["description"].asCString(), STR_OUT_DESCRIPTION_LENGTH);
			strncpy((char*)g_Output_data[temp_panel_id].at(entry_index).label, json["label"].asCString(), STR_OUT_LABEL);
			g_Output_data[temp_panel_id].at(entry_index).auto_manual = json["auto_manual"].asInt();
			g_Output_data[temp_panel_id].at(entry_index).low_voltage = json["low_voltage"].asInt();
			g_Output_data[temp_panel_id].at(entry_index).high_voltage = json["high_voltage"].asInt();
			g_Output_data[temp_panel_id].at(entry_index).range = json["range"].asInt();
			g_Output_data[temp_panel_id].at(entry_index).digital_analog = json["digital_analog"].asInt();
			g_Output_data[temp_panel_id].at(entry_index).hw_switch_status = json["hw_switch_status"].asInt();
			g_Output_data[temp_panel_id].at(entry_index).decom = json["decom"].asInt();  ////for output   0 "Normal"    1"Alarm"  
			if (g_Device_Basic_Setting[temp_panel_id].reg.n_serial_number != temp_serial_number)
			{
				if (msg_source == 0)
					SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("The serial number does not match the panel."));
				WrapErrorMessage(builder, tempjson, outmsg, _T("The serial number does not match the panel."));
				break;
			}
			unsigned int temp_objectinstance = g_Device_Basic_Setting[temp_panel_id].reg.object_instance;
			int ret_results = WritePrivateData_Blocking(temp_objectinstance, WRITEOUTPUT_T3000, entry_index, entry_index, 4, (char*)&g_Output_data[temp_panel_id].at(entry_index));
			if (ret_results > 0)
			{

			}
			else
			{
				WrapErrorMessage(builder, tempjson, outmsg, _T("Write data timeout."));
				break;
			}

			break;
		}
		case BAC_VAR:
		{
			if ((entry_index >= 0) && entry_index + 1 > BAC_VARIABLE_ITEM_COUNT)
			{
				if (msg_source == 0)
					SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Index is invalid."));
				WrapErrorMessage(builder, tempjson, outmsg, _T("Index is invalid."));
				break;
			}


			strncpy((char*)g_Variable_data[temp_panel_id].at(entry_index).description, json["description"].asCString(), STR_VARIABLE_DESCRIPTION_LENGTH);
			strncpy((char*)g_Variable_data[temp_panel_id].at(entry_index).label, json["label"].asCString(), STR_VARIABLE_LABEL);
			g_Variable_data[temp_panel_id].at(entry_index).auto_manual = json["auto_manual"].asInt();
			g_Variable_data[temp_panel_id].at(entry_index).value = json["value"].asFloat() * 1000;
			g_Variable_data[temp_panel_id].at(entry_index).range = json["range"].asInt();
			g_Variable_data[temp_panel_id].at(entry_index).control = json["control"].asInt();
			g_Variable_data[temp_panel_id].at(entry_index).digital_analog = json["digital_analog"].asInt();

			if (g_Device_Basic_Setting[temp_panel_id].reg.n_serial_number != temp_serial_number)
			{
				if (msg_source == 0)
					SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("The serial number does not match the panel."));
				WrapErrorMessage(builder, tempjson, outmsg, _T("The serial number does not match the panel."));
				break;
			}
			unsigned int temp_objectinstance = g_Device_Basic_Setting[temp_panel_id].reg.object_instance;

			int ret_results = WritePrivateData_Blocking(temp_objectinstance, WRITEVARIABLE_T3000, entry_index, entry_index, 4, (char*)&g_Variable_data[temp_panel_id].at(entry_index));
			if (ret_results > 0)
			{

			}
			else
			{
				WrapErrorMessage(builder, tempjson, outmsg, _T("Write data timeout."));
				break;
			}

			break;

		}
		case WRITE_SETTING_COMMAND:
		{
			if (entry_index != 0)
			{
				if (msg_source == 0)
					SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Index is invalid."));
				WrapErrorMessage(builder, tempjson, outmsg, _T("Index is invalid."));
				CString errorLog;
				errorLog.Format(_T("ERROR: Invalid entry_index=%d (fix=%d)"), entry_index, 0);
				WriteHandleWebViewMsgLog(_T("UPDATE_WEBVIEW_LIST"), errorLog, 0);
				break;
			}
			Str_Setting_Info temp_Setting;
			for (int index = 0; index < 400; index++)
			{
				temp_Setting.all[index] = json["all"][index].asInt();
			}

			//校验是不是同一个设备
			if (g_Device_Basic_Setting[temp_panel_id].reg.n_serial_number != temp_serial_number)
			{
				if (msg_source == 0)
					SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("The serial number does not match the panel."));
				WrapErrorMessage(builder, tempjson, outmsg, _T("The serial number does not match the panel."));

				CString errorLog;
				errorLog.Format(_T("ERROR: Serial mismatch - Expected=%d, Got=%d"),
					g_Device_Basic_Setting[temp_panel_id].reg.n_serial_number, temp_serial_number);
				WriteHandleWebViewMsgLog(_T("UPDATE_WEBVIEW_LIST"), errorLog, 0);
				break;
			}
			unsigned int temp_objectinstance = g_Device_Basic_Setting[temp_panel_id].reg.object_instance;

			// Log write attempt
			CString writeLog;
			writeLog.Format(_T("Writing to device: ObjectInstance=%d, Type=SETTING, Index=%d"),
				temp_objectinstance, entry_index);
			WriteHandleWebViewMsgLog(_T("UPDATE_WEBVIEW_LIST"), writeLog, 0);
			int ret_results = WritePrivateData_Blocking(temp_objectinstance, WRITE_SETTING_COMMAND, entry_index, entry_index, 4, (char*)&temp_Setting);
			if (ret_results > 0)
			{
				memcpy(g_Device_Basic_Setting[temp_panel_id].all, &temp_Setting, sizeof(Str_Setting_Info));
			}
			else
			{
				WrapErrorMessage(builder, tempjson, outmsg, _T("Write data timeout."));
				break;
			}

		}
			break;
		default:
			break;
		}

		tempjson["action"] = "UPDATE_WEBVIEW_LIST";
		tempjson["data"]["status"] = true;
		const std::string output = Json::writeString(builder, tempjson);
		CString temp_cs(output.c_str());
		outmsg = temp_cs;




	}
	break;
	case WEBVIEW_MESSAGE_TYPE::UPDATE_ENTRY:
	{

		Json::Value tempjson;
		int panel_id = json.get("panelId", Json::nullValue).asInt();
		int entry_index = json.get("entryIndex", Json::nullValue).asInt();
		int entry_type = json.get("entryType", Json::nullValue).asInt();
		const std::string field = json.get("field", Json::nullValue).asString();

		if ((panel_id == 0) || (panel_id >= 255))
		{
			if (msg_source == 0)
				SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Panel is invalid ."));
			WrapErrorMessage(builder, tempjson, outmsg, _T("Panel is invalid ."));
			break;
		}

		switch (entry_type)
		{
		case BAC_IN:
		{

			if ((entry_index >= 0) && entry_index + 1 > BAC_INPUT_ITEM_COUNT)
			{
				if (msg_source == 0)
					SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Index is invalid."));
				WrapErrorMessage(builder, tempjson, outmsg, _T("Index is invalid."));
				break;
			}
			if (field.compare("control") == 0) {
				m_Input_data.at(entry_index).control = json["value"].asInt();
				TRACE(_T("Change input Value to %d\r\n"), m_Input_data.at(entry_index).control);
			}
			else if (field.compare("value") == 0) {
				m_Input_data.at(entry_index).value = json["value"].asFloat() * 1000;
			}
			else if (field.compare("auto_manual") == 0) {
				m_Input_data.at(entry_index).auto_manual = json["value"].asInt();
			}

			n_ret = Write_Private_Data_Blocking(WRITEINPUT_T3000, entry_index, entry_index, g_bac_instance);
			if (n_ret > 0)
			{
				memcpy(&g_Input_data.at(panel_id).at(entry_index), &m_Input_data.at(entry_index), sizeof(Str_in_point));
				int n_index = SearchDataIndexByPanel(panel_id, READINPUT_T3000, entry_index);
				if ((n_index >= 0) && (n_index <= 255))
				{
					//第一时间更新全局数据，如果不这样，数据会来回变动
					memcpy(&m_backbround_data.at(n_index).ret_data.m_group_input_data, &g_Input_data[panel_id].at(entry_index), sizeof(Str_in_point));
				}
			}
			else
			{
				if (msg_source == 0)
					SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data timeout."));
				WrapErrorMessage(builder, tempjson, outmsg, _T("Write data timeout."));
				break;
			}
			::PostMessage(m_input_dlg_hwnd, WM_REFRESH_BAC_INPUT_LIST, entry_index, REFRESH_ON_ITEM);
			break;
		}
		case BAC_OUT:
		{

			if ((entry_index >= 0) && entry_index + 1 > BAC_OUTPUT_ITEM_COUNT)
			{
				if (msg_source == 0)
					SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Index is invalid."));
				WrapErrorMessage(builder, tempjson, outmsg, _T("Index is invalid."));
				break;
			}
			if (field.compare("control") == 0) {
				m_Output_data.at(entry_index).control = json["value"].asInt();
				TRACE(_T("Change output Value to %d\r\n"), m_Output_data.at(entry_index).control);
			}
			else if (field.compare("value") == 0) {
				m_Output_data.at(entry_index).value = json["value"].asFloat() * 1000;

			}
			else if (field.compare("auto_manual") == 0) {
				m_Output_data.at(entry_index).auto_manual = json["value"].asInt();
			}

			n_ret = Write_Private_Data_Blocking(WRITEOUTPUT_T3000, entry_index, entry_index, g_bac_instance);
			if (n_ret > 0)
			{
				memcpy(&g_Output_data.at(panel_id).at(entry_index), &m_Output_data.at(entry_index), sizeof(Str_out_point));
				int n_index = SearchDataIndexByPanel(panel_id, READOUTPUT_T3000, entry_index);
				if ((n_index >= 0) && (n_index <= 255))
				{
					//第一时间更新全局数据，如果不这样，数据会来回变动
					memcpy(&m_backbround_data.at(n_index).ret_data.m_group_output_data, &g_Output_data[panel_id].at(entry_index), sizeof(Str_out_point));
				}
			}
			else
			{
				if (msg_source == 0)
					SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data timeout."));
				WrapErrorMessage(builder, tempjson, outmsg, _T("Write data timeout."));
				break;
			}
			::PostMessage(m_output_dlg_hwnd, WM_REFRESH_BAC_OUTPUT_LIST, entry_index, REFRESH_ON_ITEM);
			break;
		}
		case BAC_VAR:
		{

			if ((entry_index >= 0) && entry_index + 1 > BAC_VARIABLE_ITEM_COUNT)
			{
				if (msg_source == 0)
					SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Index is invalid."));
				WrapErrorMessage(builder, tempjson, outmsg, _T("Index is invalid."));
				break;
			}
			if (field.compare("control") == 0) {
				m_Variable_data.at(entry_index).control = json["value"].asInt();
			}
			else if (field.compare("value") == 0) {
				m_Variable_data.at(entry_index).value = json["value"].asFloat() * 1000;
			}
			else if (field.compare("auto_manual") == 0) {
				m_Variable_data.at(entry_index).auto_manual = json["value"].asInt();
			}

			n_ret = Write_Private_Data_Blocking(WRITEVARIABLE_T3000, entry_index, entry_index, g_bac_instance);
			if (n_ret > 0)
			{
				memcpy(&g_Variable_data.at(panel_id).at(entry_index), &m_Variable_data.at(entry_index), sizeof(Str_variable_point));
				int n_index = SearchDataIndexByPanel(panel_id, READVARIABLE_T3000, entry_index);
				if ((n_index >= 0) && (n_index <= 255))
				{
					//第一时间更新全局数据，如果不这样，数据会来回变动
					memcpy(&m_backbround_data.at(n_index).ret_data.m_group_variable_data, &g_Variable_data[panel_id].at(entry_index), sizeof(Str_variable_point));
				}
			}
			else
			{
				if (msg_source == 0)
					SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data timeout."));
				WrapErrorMessage(builder, tempjson, outmsg, _T("Write data timeout."));
				break;
			}
			::PostMessage(m_variable_dlg_hwnd, WM_REFRESH_BAC_VARIABLE_LIST, entry_index, REFRESH_ON_ITEM);
			break;
		}
		case BAC_PRG:
		{

			if ((entry_index >= 0) && entry_index + 1 > BAC_PROGRAM_ITEM_COUNT)
			{
				if (msg_source == 0)
					SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Index is invalid."));
				WrapErrorMessage(builder, tempjson, outmsg, _T("Index is invalid."));
				break;
			}
			if (field.compare("status") == 0) {
				m_Program_data.at(entry_index).on_off = json["value"].asInt();
			}
			else if (field.compare("auto_manual") == 0) {
				m_Program_data.at(entry_index).auto_manual = json["value"].asInt();
			}

			n_ret = Write_Private_Data_Blocking(WRITEPROGRAM_T3000, entry_index, entry_index, g_bac_instance);
			if (n_ret > 0)
			{
				memcpy(&g_Program_data.at(panel_id).at(entry_index), &m_Program_data.at(entry_index), sizeof(Str_program_point));
				int n_index = SearchDataIndexByPanel(panel_id, READPROGRAM_T3000, entry_index);
				if ((n_index >= 0) && (n_index <= 255))
				{
					//第一时间更新全局数据，如果不这样，数据会来回变动
					memcpy(&m_backbround_data.at(n_index).ret_data.m_group_program_data, &g_Program_data[panel_id].at(entry_index), sizeof(Str_program_point));
				}
			}
			else
			{
				if (msg_source == 0)
					SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data timeout."));
				WrapErrorMessage(builder, tempjson, outmsg, _T("Write data timeout."));
				break;
			}
			::PostMessage(m_pragram_dlg_hwnd, WM_REFRESH_BAC_PROGRAM_LIST, entry_index, REFRESH_ON_ITEM);
			break;
		}
		case BAC_SCH:
		{

			if ((entry_index >= 0) && entry_index + 1 > BAC_SCHEDULE_COUNT)
			{
				if (msg_source == 0)
					SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Index is invalid."));
				WrapErrorMessage(builder, tempjson, outmsg, _T("Index is invalid."));
				break;
			}
			if (field.compare("output") == 0) {
				m_Weekly_data.at(entry_index).value = json["value"].asInt();
			}
			else if (field.compare("auto_manual") == 0) {
				m_Weekly_data.at(entry_index).auto_manual = json["value"].asInt();
			}

			n_ret = Write_Private_Data_Blocking(WRITESCHEDULE_T3000, entry_index, entry_index, g_bac_instance);
			if (n_ret > 0)
			{
				memcpy(&g_Weekly_data.at(panel_id).at(entry_index), &m_Weekly_data.at(entry_index), sizeof(Str_weekly_routine_point));
				int n_index = SearchDataIndexByPanel(panel_id, READWEEKLYROUTINE_T3000, entry_index);
				if ((n_index >= 0) && (n_index <= 255))
				{
					//第一时间更新全局数据，如果不这样，数据会来回变动
					memcpy(&m_backbround_data.at(n_index).ret_data.m_group_schedual_data, &g_Weekly_data[panel_id].at(entry_index), sizeof(Str_weekly_routine_point));
				}
			}
			else
			{
				if (msg_source == 0)
					SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data timeout."));
				WrapErrorMessage(builder, tempjson, outmsg, _T("Write data timeout."));
				break;
			}
			::PostMessage(m_weekly_dlg_hwnd, WM_REFRESH_BAC_WEEKLY_LIST, entry_index, REFRESH_ON_ITEM);
			break;
		}
		case BAC_HOL:
		{

			if ((entry_index >= 0) && entry_index + 1 > BAC_HOLIDAY_COUNT)
			{
				if (msg_source == 0)
					SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Index is invalid."));
				WrapErrorMessage(builder, tempjson, outmsg, _T("Index is invalid."));
				break;
			}
			if (field.compare("value") == 0) {
				m_Annual_data.at(entry_index).value = json["value"].asInt();
			}
			else if (field.compare("auto_manual") == 0) {
				m_Annual_data.at(entry_index).auto_manual = json["value"].asInt();
			}

			n_ret = Write_Private_Data_Blocking(WRITEHOLIDAY_T3000, entry_index, entry_index, g_bac_instance);
			if (n_ret > 0)
			{
				memcpy(&g_Annual_data.at(panel_id).at(entry_index), &m_Annual_data.at(entry_index), sizeof(Str_annual_routine_point));
				int n_index = SearchDataIndexByPanel(panel_id, READANNUALROUTINE_T3000, entry_index);
				if ((n_index >= 0) && (n_index <= 255))
				{
					//第一时间更新全局数据，如果不这样，数据会来回变动
					memcpy(&m_backbround_data.at(n_index).ret_data.m_group_annual_data, &g_Annual_data[panel_id].at(entry_index), sizeof(Str_annual_routine_point));
				}
			}
			else
			{
				if (msg_source == 0)
					SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data timeout."));
				WrapErrorMessage(builder, tempjson, outmsg, _T("Write data timeout."));
				break;
			}
			::PostMessage(m_annual_dlg_hwnd, WM_REFRESH_BAC_ANNUAL_LIST, entry_index, REFRESH_ON_ITEM);
			break;
		}
		case  BAC_WR_TIME:
		{
			if ((entry_index >= 0) && entry_index + 1 > BAC_SCHEDULE_COUNT)
			{
				if (msg_source == 0)
					SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Index is invalid."));
				WrapErrorMessage(builder, tempjson, outmsg, _T("Index is invalid."));
				break;
			}

			for (int j = 0; j < 9; j++)
			{
				for (int i = 0; i < 8; i++)
				{
					g_Schedual_Time_data[panel_id].at(entry_index).Schedual_Day_Time[i][j].time_minutes = json["data"]["time"][j][i]["minutes"].asInt();
					g_Schedual_Time_data[panel_id].at(entry_index).Schedual_Day_Time[i][j].time_hours = json["data"]["time"][j][i]["hours"].asInt();
					g_Schedual_time_flag[panel_id].at(entry_index).Time_flag[i][j] = json["data"]["time"][j][i]["flag"].asInt();
				}
			}


			n_ret = Write_Private_Data_Blocking(WRITETIMESCHEDULE_T3000, entry_index, entry_index, g_bac_instance);
			if (n_ret > 0)
			{
				;//memcpy(&g_Schedual_Time_data.at(panel_id).at(entry_index), &m_Schedual_Time_data.at(entry_index), sizeof(Str_schedual_time_point));
			}
			else
			{
				if (msg_source == 0)
					SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data timeout."));
				WrapErrorMessage(builder, tempjson, outmsg, _T("Write data timeout."));
				break;
			}
			break;


		}
		break;
		case BAC_AR_Y:
		{
			if ((entry_index >= 0) && entry_index + 1 > BAC_HOLIDAY_COUNT)
			{
				if (msg_source == 0)
					SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Index is invalid."));
				WrapErrorMessage(builder, tempjson, outmsg, _T("Index is invalid."));
				break;
			}
			for (int m = 0; m < ANNUAL_CODE_SIZE; m++)
			{
				gp_DayState[panel_id][entry_index][m] = json["data"]["data"][m].asInt();
			}

			n_ret = Write_Private_Data_Blocking(WRITEANNUALSCHEDULE_T3000, entry_index, entry_index, g_bac_instance);
			if (n_ret > 0)
			{
				;//memcpy(&gp_DayState[panel_id][entry_index], &m_DayState[panel_id][entry_index], sizeof(Str_schedual_time_point));
			}
			else
			{
				if (msg_source == 0)
					SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data timeout."));
				WrapErrorMessage(builder, tempjson, outmsg, _T("Write data timeout."));
				break;
			}

		}
		break;

		}
		tempjson["action"] = "UPDATE_ENTRY_RES";
		tempjson["data"]["status"] = true;
		const std::string output = Json::writeString(builder, tempjson);
		CString temp_cs(output.c_str());
		outmsg = temp_cs;
		//m_webView->PostWebMessageAsJson(temp_cs);
		break;
	}
	case WEBVIEW_MESSAGE_TYPE::GET_PANELS_LIST:
	{
		tempjson["action"] = "GET_PANELS_LIST_RES";

		int send_index = 0;
		for (int i = 0; i < g_bacnet_panel_info.size(); i++)
		{
			int nret = LoadOnlinePanelData(g_bacnet_panel_info.at(i).panel_number);
			if (nret > 0)
			{
				tempjson["data"][send_index]["panel_number"] = g_bacnet_panel_info.at(i).panel_number;
				tempjson["data"][send_index]["object_instance"] = g_bacnet_panel_info.at(i).object_instance;
				tempjson["data"][send_index]["serial_number"] = g_bacnet_panel_info.at(i).nseiral_number;
				tempjson["data"][send_index]["online_time"] = g_bacnet_panel_info.at(i).online_time; //Last response time .4bytes.   0  means 1970 1 1 0 
				tempjson["data"][send_index]["pid"] = g_bacnet_panel_info.at(i).npid;
				tempjson["data"][send_index]["panel_name"] = (char*)g_Device_Basic_Setting[g_bacnet_panel_info.at(i).panel_number].reg.panel_name;
				send_index++;
			}
			else
			{
				int temp_panel = g_bacnet_panel_info.at(i).panel_number;
				if ((temp_panel == 0) || (temp_panel >= 255))
					continue;
				//if (temp_panel != g_Device_Basic_Setting[temp_panel].reg.panel_number)
				//	continue;
				//if (g_bacnet_panel_info.at(i).nseiral_number != g_Device_Basic_Setting[temp_panel].reg.n_serial_number)
				//	continue;

				tempjson["data"][send_index]["panel_number"] = temp_panel;
				tempjson["data"][send_index]["object_instance"] = g_bacnet_panel_info.at(i).object_instance;// g_Device_Basic_Setting[temp_panel].reg.object_instance;
				tempjson["data"][send_index]["serial_number"] = g_bacnet_panel_info.at(i).nseiral_number;// g_Device_Basic_Setting[temp_panel].reg.n_serial_number;
				tempjson["data"][send_index]["online_time"] = g_bacnet_panel_info.at(i).online_time; //Last response time .4bytes.   0  means 1970 1 1 0 
				tempjson["data"][send_index]["pid"] = g_bacnet_panel_info.at(i).npid;// g_Device_Basic_Setting[temp_panel].reg.panel_type;
				//判断g_Device_Basic_Setting[g_bacnet_panel_info.at(i).panel_number].reg.panel_name 是否是空字符
				const char* panelNamePtr = reinterpret_cast<const char*>(g_Device_Basic_Setting[g_bacnet_panel_info.at(i).panel_number].reg.panel_name);

				if (IsNullOrEmptyOrWhitespace(panelNamePtr)) {
					tempjson["data"][send_index]["panel_name"] = std::string("(Unknown)");
				}
				else {
					tempjson["data"][send_index]["panel_name"] = panelNamePtr;
				}
				//tempjson["data"][send_index]["panel_name"] = (char*)g_Device_Basic_Setting[g_bacnet_panel_info.at(i).panel_number].reg.panel_name;
				send_index++;
			}
		}
		const std::string output = Json::writeString(builder, tempjson);
		CString temp_cs(output.c_str());
		outmsg = temp_cs;
		//m_webView->PostWebMessageAsJson(temp_cs);



		// Final log message - write to T3WebLog\YYYY-MM\MMDD\ if logging enabled
			WriteHandleWebViewMsgLog(_T("GET_PANELS_LIST"), outmsg, g_bacnet_panel_info.size());
		
		break;
	}
	case WEBVIEW_MESSAGE_TYPE::GET_ENTRIES:
	{
		tempjson["action"] = "GET_ENTRIES_RES";
		Json::Value data = json["data"];
		if (data.size()) {
			for (int i = 0; i < data.size(); i++) {
				Json::Value entry = data[i];
				int npanel_id = entry.get("panelId", Json::nullValue).asInt();
				int entry_index = entry.get("index", Json::nullValue).asInt();
				int entry_type = entry.get("type", Json::nullValue).asInt();
				//tempjson["data"][i]["panel_id"] = panel_id;
				tempjson["data"][i]["index"] = entry_index;

				if (debug_item_show == DEBUG_SHOW_MESSAGE_THREAD)
				{
					CString Mession_ret;
					Mession_ret.Format(_T("GET_ENTRIES panel = %d , entry_type=%d ,entry_index =%d."), npanel_id, entry_type, entry_index);
					DFTrace(Mession_ret);
				}

				if (entry_type == BAC_IN)
				{
					if (entry_index >= BAC_INPUT_ITEM_COUNT)
						continue;
					CString temp_cs4;
					int ret_index4 = Post_Background_Read_Message_ByPanel(npanel_id, READINPUT_T3000, entry_index + 1);  //send message to background ，read 199IN3
					if (ret_index4 >= 0)
					{
						//memcpy(&g_Input_data[npanel_id].at(entry_index), &m_backbround_data.at(ret_index4).ret_data.m_group_input_data, sizeof(Str_in_point));
					}
					else
					{
						continue;
					}
					tempjson["data"][i]["pid"] = npanel_id;
					tempjson["data"][i]["type"] = "INPUT";
					tempjson["data"][i]["index"] = entry_index;
					tempjson["data"][i]["id"] = "IN" + to_string(entry_index + 1);
					tempjson["data"][i]["command"] = to_string(npanel_id) + "IN" + to_string(entry_index + 1);
					tempjson["data"][i]["description"] = (char*)g_Input_data[npanel_id].at(entry_index).description;
					tempjson["data"][i]["label"] = (char*)g_Input_data[npanel_id].at(entry_index).label;
					tempjson["data"][i]["unit"] = g_Input_data[npanel_id].at(entry_index).range;
					tempjson["data"][i]["auto_manual"] = g_Input_data[npanel_id].at(entry_index).auto_manual;
					tempjson["data"][i]["value"] = g_Input_data[npanel_id].at(entry_index).value;
					tempjson["data"][i]["filter"] = g_Input_data[npanel_id].at(entry_index).filter;
					tempjson["data"][i]["control"] = g_Input_data[npanel_id].at(entry_index).control;
					tempjson["data"][i]["digital_analog"] = g_Input_data[npanel_id].at(entry_index).digital_analog;
					tempjson["data"][i]["range"] = g_Input_data[npanel_id].at(entry_index).range;
					tempjson["data"][i]["calibration_sign"] = g_Input_data[npanel_id].at(entry_index).calibration_sign;
					tempjson["data"][i]["calibration_h"] = g_Input_data[npanel_id].at(entry_index).calibration_h;
					tempjson["data"][i]["calibration_l"] = g_Input_data[npanel_id].at(entry_index).calibration_l;
					tempjson["data"][i]["decom"] = g_Input_data[npanel_id].at(entry_index).decom; //for input   0 "Normal"    1"Open"   2  "Shorted"
				}
				else if (entry_type == BAC_OUT)
				{
					if (entry_index >= BAC_OUTPUT_ITEM_COUNT)
						continue;
					int ret_index_out = Post_Background_Read_Message_ByPanel(npanel_id, READOUTPUT_T3000, entry_index + 1);  //send message to background ，read 199OUT3
					if (ret_index_out >= 0)
					{
						//memcpy(&g_Output_data[npanel_id].at(entry_index), &m_backbround_data.at(ret_index_out).ret_data.m_group_output_data, sizeof(Str_out_point));
					}
					else
					{
						continue;
					}
					tempjson["data"][i]["pid"] = npanel_id;
					tempjson["data"][i]["type"] = "OUTPUT";
					tempjson["data"][i]["index"] = entry_index;
					tempjson["data"][i]["id"] = "OUT" + to_string(entry_index + 1);
					tempjson["data"][i]["command"] = to_string(npanel_id) + "OUT" + to_string(entry_index + 1);
					tempjson["data"][i]["description"] = (char*)g_Output_data[npanel_id].at(entry_index).description;
					tempjson["data"][i]["label"] = (char*)g_Output_data[npanel_id].at(entry_index).label;
					tempjson["data"][i]["auto_manual"] = g_Output_data[npanel_id].at(entry_index).auto_manual;
					tempjson["data"][i]["value"] = g_Output_data[npanel_id].at(entry_index).value;
					tempjson["data"][i]["low_voltage"] = g_Output_data[npanel_id].at(entry_index).low_voltage;
					tempjson["data"][i]["high_voltage"] = g_Output_data[npanel_id].at(entry_index).high_voltage;
					tempjson["data"][i]["range"] = g_Output_data[npanel_id].at(entry_index).range;
					tempjson["data"][i]["control"] = g_Output_data[npanel_id].at(entry_index).control;
					tempjson["data"][i]["digital_analog"] = g_Output_data[npanel_id].at(entry_index).digital_analog;
					tempjson["data"][i]["hw_switch_status"] = g_Output_data[npanel_id].at(entry_index).hw_switch_status;
					tempjson["data"][i]["decom"] = g_Output_data[npanel_id].at(entry_index).decom; //for output   0 "Normal"    1"Alarm"  

				}
				else if (entry_type == BAC_VAR)
				{
					if (entry_index >= BAC_VARIABLE_ITEM_COUNT)
						continue;
					int ret_index_var = Post_Background_Read_Message_ByPanel(npanel_id, READVARIABLE_T3000, entry_index + 1);  //send message to background ，read 199OUT3
					if (ret_index_var >= 0)
					{
						//memcpy(&g_Variable_data[npanel_id].at(entry_index), &m_backbround_data.at(ret_index_var).ret_data.m_group_variable_data, sizeof(Str_variable_point));
					}
					else
					{
						continue;
					}

					tempjson["data"][i]["pid"] = npanel_id;
					tempjson["data"][i]["type"] = "VARIABLE";
					tempjson["data"][i]["index"] = entry_index;
					tempjson["data"][i]["id"] = "VAR" + to_string(entry_index + 1);
					tempjson["data"][i]["command"] = to_string(npanel_id) + "VAR" + to_string(i + 1);
					tempjson["data"][i]["description"] = (char*)g_Variable_data[npanel_id].at(entry_index).description;
					tempjson["data"][i]["label"] = (char*)g_Variable_data[npanel_id].at(entry_index).label;
					tempjson["data"][i]["auto_manual"] = g_Variable_data[npanel_id].at(entry_index).auto_manual;
					tempjson["data"][i]["value"] = g_Variable_data[npanel_id].at(entry_index).value;
					tempjson["data"][i]["range"] = g_Variable_data[npanel_id].at(entry_index).range;
					tempjson["data"][i]["control"] = g_Variable_data[npanel_id].at(entry_index).control;
					tempjson["data"][i]["digital_analog"] = g_Variable_data[npanel_id].at(entry_index).digital_analog;

				}
				else if (entry_type == BAC_PRG)
				{
					if (entry_index >= BAC_PROGRAM_ITEM_COUNT)
						continue;
					int ret_index_prg = Post_Background_Read_Message_ByPanel(npanel_id, READPROGRAM_T3000, entry_index + 1);  //send message to background ，read 199OUT3
					if (ret_index_prg >= 0)
					{
						memcpy(&g_Program_data[npanel_id].at(entry_index), &m_backbround_data.at(ret_index_prg).ret_data.m_group_program_data, sizeof(Str_program_point));
					}
					else
					{
						continue;
					}
					tempjson["data"][i]["pid"] = npanel_id;
					tempjson["data"][i]["type"] = "PROGRAM";
					tempjson["data"][i]["index"] = entry_index;
					tempjson["data"][i]["id"] = "PRG" + to_string(entry_index + 1);
					tempjson["data"][i]["command"] = to_string(npanel_id) + "PRG" + to_string(entry_index + 1);
					tempjson["data"][i]["description"] = (char*)g_Program_data[npanel_id].at(entry_index).description;
					tempjson["data"][i]["label"] = (char*)g_Program_data[npanel_id].at(entry_index).label;
					tempjson["data"][i]["auto_manual"] = g_Program_data[npanel_id].at(entry_index).auto_manual;
					tempjson["data"][i]["status"] = g_Program_data[npanel_id].at(entry_index).on_off;
				}
				else if (entry_type == BAC_SCH)
				{
					if (entry_index >= BAC_SCHEDULE_COUNT)
						continue;
					int ret_index_sch = Post_Background_Read_Message_ByPanel(npanel_id, READWEEKLYROUTINE_T3000, entry_index + 1);  //send message to background ，read 199OUT3
					if (ret_index_sch >= 0)
					{
						memcpy(&g_Weekly_data[npanel_id].at(entry_index), &m_backbround_data.at(ret_index_sch).ret_data.m_group_schedual_data, sizeof(Str_weekly_routine_point));
					}
					else
					{
						continue;
					}
					tempjson["data"][i]["pid"] = npanel_id;
					tempjson["data"][i]["type"] = "SCHEDULE";
					tempjson["data"][i]["index"] = entry_index;
					tempjson["data"][i]["id"] = "SCH" + to_string(entry_index + 1);
					tempjson["data"][i]["command"] = to_string(npanel_id) + "SCH" + to_string(entry_index + 1);
					tempjson["data"][i]["description"] = (char*)g_Weekly_data[npanel_id].at(entry_index).description;
					tempjson["data"][i]["label"] = (char*)g_Weekly_data[npanel_id].at(entry_index).label;
					tempjson["data"][i]["auto_manual"] = g_Weekly_data[npanel_id].at(entry_index).auto_manual;
					tempjson["data"][i]["output"] = g_Weekly_data[npanel_id].at(entry_index).value;
					tempjson["data"][i]["state1"] = g_Weekly_data[npanel_id].at(entry_index).override_1_value;
					tempjson["data"][i]["state2"] = g_Weekly_data[npanel_id].at(entry_index).override_2_value;
				}
				else if (entry_type == BAC_HOL)
				{
					if (entry_index >= BAC_HOLIDAY_COUNT)
						continue;
					CString temp_cs4;
					int ret_index_hol = Post_Background_Read_Message_ByPanel(npanel_id, READANNUALROUTINE_T3000, entry_index + 1);  //send message to background ，read 199IN3
					if (ret_index_hol >= 0)
					{
						memcpy(&g_Annual_data[npanel_id].at(entry_index), &m_backbround_data.at(ret_index_hol).ret_data.m_group_annual_data, sizeof(Str_annual_routine_point));
					}
					else
					{
						continue;
					}

					tempjson["data"][i]["pid"] = npanel_id;
					tempjson["data"][i]["type"] = "HOLIDAY";
					tempjson["data"][i]["index"] = entry_index;
					tempjson["data"][i]["id"] = "CAL" + to_string(entry_index + 1);
					tempjson["data"][i]["command"] = to_string(npanel_id) + "CAL" + to_string(entry_index + 1);
					tempjson["data"][i]["description"] = (char*)g_Annual_data[npanel_id].at(entry_index).description;
					tempjson["data"][i]["label"] = (char*)g_Annual_data[npanel_id].at(entry_index).label;
					tempjson["data"][i]["auto_manual"] = g_Annual_data[npanel_id].at(entry_index).auto_manual;
					tempjson["data"][i]["value"] = g_Annual_data[npanel_id].at(entry_index).value;

				}
				else if (entry_type == BAC_GRP)
				{
					if (entry_index >= BAC_SCREEN_COUNT)
						continue;
					int ret_index_out = Post_Background_Read_Message_ByPanel(npanel_id, READSCREEN_T3000, entry_index + 1);  //send message to background ，read 199OUT3
					if (ret_index_out >= 0)
					{
						memcpy(&g_screen_data[npanel_id].at(entry_index), &m_backbround_data.at(ret_index_out).ret_data.m_group_screen_data, sizeof(Control_group_point));
					}
					else
					{
						continue;
					}
					tempjson["data"][i]["pid"] = npanel_id;
					tempjson["data"][i]["type"] = "GRP";
					tempjson["data"][i]["index"] = entry_index;
					tempjson["data"][i]["id"] = "GRP" + to_string(entry_index + 1);
					tempjson["data"][i]["command"] = to_string(npanel_id) + "GRP" + to_string(entry_index + 1);
					tempjson["data"][i]["description"] = (char*)g_screen_data[npanel_id].at(entry_index).description;
					tempjson["data"][i]["label"] = (char*)g_screen_data[npanel_id].at(entry_index).label;
				}
				else if (entry_type == BAC_AMON)
				{
					if (entry_index >= BAC_MONITOR_COUNT)
						continue;
					int ret_index_monitor = Post_Background_Read_Message_ByPanel(npanel_id, READMONITOR_T3000, entry_index + 1);  //send message to background ，read 199OUT3
					if (ret_index_monitor >= 0)
					{
						memcpy(&g_monitor_data[npanel_id].at(entry_index), &m_backbround_data.at(ret_index_monitor).ret_data.m_group_monitor_data, sizeof(Str_monitor_point));
					}
					else
					{
						continue;
					}

					tempjson["data"][i]["pid"] = npanel_id;
					tempjson["data"][i]["type"] = "MON";
					tempjson["data"][i]["index"] = entry_index;
					tempjson["data"][i]["id"] = "MON" + to_string(entry_index + 1);
					tempjson["data"][i]["command"] = to_string(npanel_id) + "MON" + to_string(entry_index + 1);
					tempjson["data"][i]["label"] = (char*)g_monitor_data[npanel_id].at(entry_index).label;
					tempjson["data"][i]["hour_interval_time"] = g_monitor_data[npanel_id].at(entry_index).hour_interval_time;
					tempjson["data"][i]["minute_interval_time"] = g_monitor_data[npanel_id].at(entry_index).minute_interval_time;
					tempjson["data"][i]["second_interval_time"] = g_monitor_data[npanel_id].at(entry_index).second_interval_time;
					tempjson["data"][i]["status"] = g_monitor_data[npanel_id].at(entry_index).status;
					//There is also additional data that does not need to be passed to the webview interface

					tempjson["data"][i]["num_inputs"] = g_monitor_data[npanel_id].at(i).num_inputs; ///* total number of points */
					tempjson["data"][i]["an_inputs"] = g_monitor_data[npanel_id].at(i).an_inputs;  ///* number of analog points */
					for (int m = 0; m < MAX_POINTS_IN_MONITOR; m++)
					{
						tempjson["data"][i]["range"][m] = g_monitor_data[npanel_id].at(i).range[m]; //14个input对应的range
						//例如 例子1  111OUT45          panel = 111 , sub_panel = 0 . point_type = 0 ，number = 45 , network 默认为0
						//例如 例子2  123.45.MB_REG67   panel = 123 , sub_panel = 45, point_type = 2 , number = 67 , network 默认为0
						//例子3		  45678AV90         
						tempjson["data"][i]["input"][m]["panel"] = g_monitor_data[npanel_id].at(i).inputs[m].panel;
						tempjson["data"][i]["input"][m]["sub_panel"] = g_monitor_data[npanel_id].at(i).inputs[m].sub_panel;
						tempjson["data"][i]["input"][m]["point_type"] = g_monitor_data[npanel_id].at(i).inputs[m].point_type;
						tempjson["data"][i]["input"][m]["point_number"] = g_monitor_data[npanel_id].at(i).inputs[m].number;
						tempjson["data"][i]["input"][m]["network"] = g_monitor_data[npanel_id].at(i).inputs[m].network;
					}

				}
			}
			const std::string output = Json::writeString(builder, tempjson);
			CString temp_cs(output.c_str());
			outmsg = temp_cs;
			//m_webView->PostWebMessageAsJson(temp_cs);
		}
	}
	break;
	case OPEN_ENTRY_EDIT_WINDOW:
	{
		tempjson["action"] = "OPEN_ENTRY_EDIT_WINDOW";
		int npanel_id = json.get("panelId", Json::nullValue).asInt();
		int entry_index = json.get("entryIndex", Json::nullValue).asInt();
		int entry_type = json.get("entryType", Json::nullValue).asInt();
		if (npanel_id == bac_gloab_panel) //暂时只处理本地 panel的 点击响应
		{
			switch (entry_type)
			{
			case BAC_SCH:
			{
				if (entry_index < BAC_SCHEDULE_COUNT)
				{
					weekly_list_line = entry_index;
					WeeklyRoutine_Window->OnBnClickedButtonWeeklyScheduleEdit();
				}
			}
			break;
			case BAC_HOL:
			{
				if (entry_index < BAC_HOLIDAY_COUNT)
				{
					annual_list_line = entry_index;
					AnnualRoutine_Window->OnBnClickedButtonAnnualEdit();
				}
			}
			break;
			case BAC_PRG:
			{
				if (entry_index < BAC_PROGRAM_ITEM_COUNT)
				{
					program_list_line = entry_index;
					Program_Window->OnBnClickedButtonProgramEdit();

				}
			}
			break;
			default:
				break;
			}
		}
		else
		{

		}
	}
	break;
	case SAVE_IMAGE:
	{
		const std::string filename = json.get("filename", Json::nullValue).asString();
		int file_length = json.get("fileLength", Json::nullValue).asInt();
		const std::string file_data = Json::writeString(builder, json["fileData"]);
		char des_folder[512];
		memset(des_folder, 0, 512);

		CString web_image_folder = g_strExePth + _T("ResourceFile\\webview\\www\\uploads");
		int ret = FALSE;
		WIN32_FIND_DATA fd;
		HANDLE hFind_folder;
		hFind_folder = FindFirstFile(web_image_folder, &fd);
		if ((hFind_folder != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			ret = TRUE; //目录存在
		}
		FindClose(hFind_folder);
		if (ret == false)
		{
			SECURITY_ATTRIBUTES attrib;
			attrib.bInheritHandle = FALSE;
			attrib.lpSecurityDescriptor = NULL;
			attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
			CreateDirectory(web_image_folder, &attrib);
		}

		CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
		CString cs_filename = filename.c_str();

		CStringArray temp_array;
		CString temp_file_name_without_suffix;
		CString temp_suffix;
		SplitCStringA(temp_array, cs_filename, _T("."));
		if (temp_array.GetSize() >= 2)
		{
			CString temp_last;
			temp_last = temp_array.GetAt(temp_array.GetSize() - 1);
			int temp_length = temp_last.GetLength();
			int total_length = cs_filename.GetLength();
			temp_file_name_without_suffix = cs_filename.Left(total_length - temp_length - 1);
			temp_suffix = temp_last;
		}
		else
		{
			break; //  suffix error
		}
		int temp_name_index = 0;
		CString temp_image_path;
		do
		{
			if (temp_name_index > 0)
			{
				temp_image_path.Format(_T("%s\\%s-%d.%s"), web_image_folder.GetBuffer(), temp_file_name_without_suffix.GetBuffer(), temp_name_index, temp_suffix.GetBuffer());
				cs_filename.Format(_T("%s-%d.%s"), temp_file_name_without_suffix.GetBuffer(), temp_name_index, temp_suffix.GetBuffer());
			}
			else
			{
				temp_image_path = web_image_folder + _T("\\") + filename.c_str();
				cs_filename = filename.c_str();
			}
			hFind_folder = FindFirstFile(temp_image_path, &fd);
			if (hFind_folder != INVALID_HANDLE_VALUE)
			{
				//发现同名文件存在，变更同名文件文件名;
				temp_name_index++;
			}
			else
			{
				temp_name_index = 0;
				break;
			}
			FindClose(hFind_folder);

		} while ((temp_name_index > 0) && (temp_name_index <= 100));

		if (temp_name_index > 100)
		{
			break; // After 100 loops, no available file name can be found
		}

		char des_file_name[MAX_PATH];
		memset(des_file_name, 0, MAX_PATH);
		WideCharToMultiByte(CP_ACP, 0, cs_filename.GetBuffer(), -1, des_file_name, MAX_PATH, NULL, NULL);
		string newfilename = des_file_name;

		WideCharToMultiByte(CP_ACP, 0, temp_image_path.GetBuffer(), -1, des_folder, 512, NULL, NULL);
		std::ofstream file(des_folder, std::ios::binary);

		char* ret_result = (char*)malloc(sizeof(char) * file_length);	//申请返回字符串空间
		memset(ret_result, 0, sizeof(char) * file_length);		//初始化字符串空间
		const char* temp_ret = strstr((char const*)file_data.c_str(), "base64,"); //去除前面不用的部分;
		if (temp_ret == NULL)
			break;
		const std::string temp_image_data = strstr((char const*)file_data.c_str(), "base64,") + 7; //去除前面不用的部分;
		base64_decode((char const*)temp_image_data.c_str(), ret_result, (int)strlen(temp_image_data.c_str()));
		file.write(ret_result, file_length);
		free(ret_result);
		CString cs_temp_path;
		cs_temp_path.Format(_T("ResourceFile\\webview\\www\\uploads\\"), pFrame->m_strCurMainBuildingName);
		char temp_folder[256];
		memset(temp_folder, 0, 256);
		WideCharToMultiByte(CP_ACP, 0, cs_temp_path.GetBuffer(), -1, temp_folder, 256, NULL, NULL);
		strcat(temp_folder, filename.c_str());
		tempjson["action"] = "SAVE_IMAGE_RES";
		tempjson["data"]["name"] = filename;
		tempjson["data"]["path"] = "/uploads/" + newfilename;
		const std::string output = Json::writeString(builder, tempjson);
		CString temp_cs(output.c_str());
		outmsg = temp_cs;
		//m_webView->PostWebMessageAsJson(temp_cs);
	}
	break;
	case SAVE_LIBRAY_DATA:
	{
		CString temp_lib_file;
		temp_lib_file = _T("hvac_library.json");
		des_lib_file = image_fordor + _T("\\") + temp_lib_file;
		const std::string file_output = Json::writeString(builder, json["data"]);

		CFile file;

		CString file_temp_cs(file_output.c_str());
		file.Open(des_lib_file, CFile::modeCreate | CFile::modeWrite | CFile::modeCreate, NULL);
		file.Write(file_temp_cs, file_temp_cs.GetLength() * 2);
		file.Close();
		Json::Value tempjson;
		tempjson["action"] = "SAVE_LIBRAY_DATA_RES";
		tempjson["status"] = true;
		const std::string output = Json::writeString(builder, tempjson);
		CString temp_cs(output.c_str());
		outmsg = temp_cs;
		//m_webView->PostWebMessageAsJson(temp_cs);
	}
	break;
	case SAVE_NEW_LIBRARY_DATA:
	{
		CString temp_lib_file;
		temp_lib_file = _T("t3_hvac_library.json");
		des_lib_file = image_fordor + _T("\\") + temp_lib_file;
		const std::string file_output = Json::writeString(builder, json["data"]);

		CFile file;

		CString file_temp_cs(file_output.c_str());
		file.Open(des_lib_file, CFile::modeCreate | CFile::modeWrite | CFile::modeCreate, NULL);
		file.Write(file_temp_cs, file_temp_cs.GetLength() * 2);
		file.Close();
		Json::Value tempjson;
		tempjson["action"] = "SAVE_NEW_LIBRARY_DATA_RES";
		tempjson["status"] = true;
		const std::string output = Json::writeString(builder, tempjson);
		CString temp_cs(output.c_str());
		outmsg = temp_cs;
		//m_webView->PostWebMessageAsJson(temp_cs);
	}
	break;
	case DELETE_IMAGE:
	{
		const std::string file_path = Json::writeString(builder, json["data"]);
		CString web_image_folder = g_strExePth + _T("ResourceFile\\webview\\www");
		CString temp_delete_file_path;
		CString temp_file_name = file_path.c_str();
		temp_file_name = temp_file_name.TrimLeft(_T("\""));
		temp_file_name = temp_file_name.TrimRight(_T("\""));
		temp_delete_file_path = web_image_folder + temp_file_name;
		CFileFind temp_find;
		if (temp_find.FindFile(temp_delete_file_path)) //当 发现需要删除的的 IMAGE文件时 删除
		{
			DeleteFile(temp_delete_file_path);
		}
	}
	break;
	case GET_SELECTED_DEVICE_INFO:
	{
		Json::Value tempjson;
		tempjson["action"] = "GET_SELECTED_DEVICE_INFO_RES";
		tempjson["data"]["product_id"] = g_selected_product_id;
		tempjson["data"]["panel_id"] = bac_gloab_panel;
		tempjson["data"]["serial_number"] = g_selected_serialnumber;
		const std::string output = Json::writeString(builder, tempjson);
		CString temp_cs(output.c_str());
		outmsg = temp_cs;
		//m_webView->PostWebMessageAsJson(temp_cs);
	}
	break;
	case LOGGING_DATA:
	{
		//release 版本暂时不启用这个功能
		if (enable_trendlog_background_read == false)
		{
			break;
		}
		int temp_panel_id = json.get("panelId", Json::nullValue).asInt();
		int temp_serial_number = json.get("serialNumber", Json::nullValue).asInt();
		//temp_panel_id = 144; //test
		//temp_serial_number = 240488;
		//break;
		// 
		// LOG INPUT PARAMETERS for debugging
		CString debugLog;
		debugLog.Format(_T("LOGGING_DATA RECEIVED - panelId: %d, serialNumber: %d\n"), temp_panel_id, temp_serial_number);
		OutputDebugString(debugLog);
		// Local flag to enable/disable logging - set to false to disable
		//15分钟内收到这个命令直接break;
		static DWORD last_logging_time = 0;
		DWORD current_time = GetTickCount();
		if (current_time - last_logging_time < 15 * 1000) // 15 minutes in milliseconds
		{
			CString skipLog;
			skipLog.Format(_T("LOGGING_DATA SKIPPED - within 1 minute cooldown (panelId: %d, serialNumber: %d)\n"), temp_panel_id, temp_serial_number);
			OutputDebugString(skipLog);
			break; // Ignore the command if within 15 minutes
		}
		last_logging_time = current_time;


		Json::Value tempjson;
		tempjson["action"] = "LOGGING_DATA_RES";

		int nret = Post_ReadTrendlog_Message(temp_panel_id, temp_serial_number);
		if (nret < 0)
		{
			WrapErrorMessage(builder, tempjson, outmsg, _T("Panel is offline ."));
			break;
		}
		//发送消息加载所有panel 的 in out var 数据;
		int device_count = 0;
		for (int panel_idx = 0; panel_idx < g_bacnet_panel_info.size(); panel_idx++)
		{
#if 1
			int npanel_id = g_bacnet_panel_info.at(panel_idx).panel_number;

			if (g_bacnet_panel_info.at(panel_idx).object_instance != g_logging_time[npanel_id].bac_instance)
				continue;
			if (g_bacnet_panel_info.at(panel_idx).nseiral_number != g_logging_time[npanel_id].sn)
				continue;
			if (g_bacnet_panel_info.at(panel_idx).panel_number != g_logging_time[npanel_id].n_panel_number)
				continue;
			if (npanel_id != g_logging_time[npanel_id].n_panel_number)
				continue;
			if (g_logging_time[npanel_id].basic_setting_status != 1)
				continue;

			// Device passed validation - add device main info
			g_Device_Basic_Setting[npanel_id].reg.ip_addr;
			char ipStr[16]; // 用于存储转换后的IP字符串
			unsigned char* ipAddr = g_Device_Basic_Setting[npanel_id].reg.ip_addr;
			// 使用 sprintf 将 IP 地址转换为字符串
			sprintf(ipStr, "%d.%d.%d.%d", ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3]);

			CString deviceLog;
			deviceLog.Format(_T("� Reading g_Device_Basic_Setting[%d] - SerialNumber: %d, Name: %s, IP: %s\n"),
				npanel_id,
				g_Device_Basic_Setting[npanel_id].reg.n_serial_number,
				CString(g_Device_Basic_Setting[npanel_id].reg.panel_name),
				CString(ipStr));
			OutputDebugString(deviceLog);

			// Add device main info to data array
			tempjson["data"][device_count]["panel_id"] = npanel_id;
			tempjson["data"][device_count]["panel_name"] = (char*)g_Device_Basic_Setting[npanel_id].reg.panel_name;
			//tempjson["data"][device_count]["panel_serial_number"] = g_Device_Basic_Setting[npanel_id].reg.n_serial_number;
			tempjson["data"][device_count]["panel_serial_number"] = g_bacnet_panel_info.at(panel_idx).nseiral_number;
			tempjson["data"][device_count]["panel_ipaddress"] = ipStr;
			tempjson["data"][device_count]["input_logging_time"] = g_logging_time[npanel_id].input_log_time;
			tempjson["data"][device_count]["output_logging_time"] = g_logging_time[npanel_id].output_log_time;
			tempjson["data"][device_count]["variable_logging_time"] = g_logging_time[npanel_id].variable_log_time;

			// Add all INPUT points for this device in device_data array
			int point_idx = 0;
			for (int input_idx = 0; input_idx < BAC_INPUT_ITEM_COUNT; input_idx++) {
				tempjson["data"][device_count]["device_data"][point_idx]["pid"] = npanel_id;
				tempjson["data"][device_count]["device_data"][point_idx]["type"] = "INPUT";
				tempjson["data"][device_count]["device_data"][point_idx]["index"] = input_idx;
				tempjson["data"][device_count]["device_data"][point_idx]["id"] = "IN" + to_string(input_idx + 1);
				tempjson["data"][device_count]["device_data"][point_idx]["command"] = to_string(npanel_id) + "IN" + to_string(input_idx + 1);
				tempjson["data"][device_count]["device_data"][point_idx]["description"] = (char*)g_Input_data[npanel_id].at(input_idx).description;
				tempjson["data"][device_count]["device_data"][point_idx]["label"] = (char*)g_Input_data[npanel_id].at(input_idx).label;
				tempjson["data"][device_count]["device_data"][point_idx]["unit"] = g_Input_data[npanel_id].at(input_idx).range;
				tempjson["data"][device_count]["device_data"][point_idx]["auto_manual"] = g_Input_data[npanel_id].at(input_idx).auto_manual;
				tempjson["data"][device_count]["device_data"][point_idx]["value"] = g_Input_data[npanel_id].at(input_idx).value;
				tempjson["data"][device_count]["device_data"][point_idx]["filter"] = g_Input_data[npanel_id].at(input_idx).filter;
				tempjson["data"][device_count]["device_data"][point_idx]["control"] = g_Input_data[npanel_id].at(input_idx).control;
				tempjson["data"][device_count]["device_data"][point_idx]["digital_analog"] = g_Input_data[npanel_id].at(input_idx).digital_analog;
				tempjson["data"][device_count]["device_data"][point_idx]["range"] = g_Input_data[npanel_id].at(input_idx).range;
				tempjson["data"][device_count]["device_data"][point_idx]["calibration_sign"] = g_Input_data[npanel_id].at(input_idx).calibration_sign;
				tempjson["data"][device_count]["device_data"][point_idx]["calibration_h"] = g_Input_data[npanel_id].at(input_idx).calibration_h;
				tempjson["data"][device_count]["device_data"][point_idx]["calibration_l"] = g_Input_data[npanel_id].at(input_idx).calibration_l;
				tempjson["data"][device_count]["device_data"][point_idx]["decom"] = g_Input_data[npanel_id].at(input_idx).decom; //for input   0 "Normal"    1"Open"   2  "Shorted"
				point_idx++;
			}

			// Add all OUTPUT points for this device in device_data array
			for (int output_idx = 0; output_idx < BAC_OUTPUT_ITEM_COUNT; output_idx++) {
				tempjson["data"][device_count]["device_data"][point_idx]["pid"] = npanel_id;
				tempjson["data"][device_count]["device_data"][point_idx]["type"] = "OUTPUT";
				tempjson["data"][device_count]["device_data"][point_idx]["index"] = output_idx;
				tempjson["data"][device_count]["device_data"][point_idx]["id"] = "OUT" + to_string(output_idx + 1);
				tempjson["data"][device_count]["device_data"][point_idx]["command"] = to_string(npanel_id) + "OUT" + to_string(output_idx + 1);
				tempjson["data"][device_count]["device_data"][point_idx]["description"] = (char*)g_Output_data[npanel_id].at(output_idx).description;
				tempjson["data"][device_count]["device_data"][point_idx]["label"] = (char*)g_Output_data[npanel_id].at(output_idx).label;
				tempjson["data"][device_count]["device_data"][point_idx]["auto_manual"] = g_Output_data[npanel_id].at(output_idx).auto_manual;
				tempjson["data"][device_count]["device_data"][point_idx]["value"] = g_Output_data[npanel_id].at(output_idx).value;
				tempjson["data"][device_count]["device_data"][point_idx]["low_voltage"] = g_Output_data[npanel_id].at(output_idx).low_voltage;
				tempjson["data"][device_count]["device_data"][point_idx]["high_voltage"] = g_Output_data[npanel_id].at(output_idx).high_voltage;
				tempjson["data"][device_count]["device_data"][point_idx]["range"] = g_Output_data[npanel_id].at(output_idx).range;
				tempjson["data"][device_count]["device_data"][point_idx]["control"] = g_Output_data[npanel_id].at(output_idx).control;
				tempjson["data"][device_count]["device_data"][point_idx]["digital_analog"] = g_Output_data[npanel_id].at(output_idx).digital_analog;
				tempjson["data"][device_count]["device_data"][point_idx]["hw_switch_status"] = g_Output_data[npanel_id].at(output_idx).hw_switch_status;
				tempjson["data"][device_count]["device_data"][point_idx]["decom"] = g_Output_data[npanel_id].at(output_idx).decom; //for output   0 "Normal"    1"Alarm"
				point_idx++;
			}

			// Add all VARIABLE points for this device in device_data array
			for (int var_idx = 0; var_idx < BAC_VARIABLE_ITEM_COUNT; var_idx++) {
				tempjson["data"][device_count]["device_data"][point_idx]["pid"] = npanel_id;
				tempjson["data"][device_count]["device_data"][point_idx]["type"] = "VARIABLE";
				tempjson["data"][device_count]["device_data"][point_idx]["index"] = var_idx;
				tempjson["data"][device_count]["device_data"][point_idx]["id"] = "VAR" + to_string(var_idx + 1);
				tempjson["data"][device_count]["device_data"][point_idx]["command"] = to_string(npanel_id) + "VAR" + to_string(var_idx + 1);
				tempjson["data"][device_count]["device_data"][point_idx]["description"] = (char*)g_Variable_data[npanel_id].at(var_idx).description;
				tempjson["data"][device_count]["device_data"][point_idx]["label"] = (char*)g_Variable_data[npanel_id].at(var_idx).label;
				tempjson["data"][device_count]["device_data"][point_idx]["auto_manual"] = g_Variable_data[npanel_id].at(var_idx).auto_manual;
				tempjson["data"][device_count]["device_data"][point_idx]["value"] = g_Variable_data[npanel_id].at(var_idx).value;
				tempjson["data"][device_count]["device_data"][point_idx]["range"] = g_Variable_data[npanel_id].at(var_idx).range;
				tempjson["data"][device_count]["device_data"][point_idx]["control"] = g_Variable_data[npanel_id].at(var_idx).control;
				tempjson["data"][device_count]["device_data"][point_idx]["digital_analog"] = g_Variable_data[npanel_id].at(var_idx).digital_analog;
				tempjson["data"][device_count]["device_data"][point_idx]["unused"] = g_Variable_data[npanel_id].at(var_idx).unused;
				point_idx++;
			}
			device_count++;
#endif
		}

		// Generate final JSON response once for all devices
		const std::string output = Json::writeString(builder, tempjson);
		CString temp_cs(output.c_str());
		outmsg = temp_cs;

		// Final log message - write to T3WebLog\YYYY-MM\MMDD\ if logging enabled
			WriteHandleWebViewMsgLog(_T("LOGGING_DATA"), outmsg, device_count);
	}
	break;
	default:
		break;
	}
}


void BacnetWebViewAppWindow::ProcessWebviewMsg(CString msg)
{
	CString outmsg;
	HandleWebViewMsg(msg, outmsg);
	if (!outmsg.IsEmpty())
		m_webView->PostWebMessageAsJson(outmsg);
}

char base64char[] = {
		'A','B','C','D','E','F','G','H','I','J',
		'K','L','M','N','O','P','Q','R','S','T',
		'U','V','W','X','Y','Z','a','b','c','d',
		'e','f','g','h','i','j','k','l','m','n',
		'o','p','q','r','s','t','u','v','w','x',
		'y','z','0','1','2','3','4','5','6','7',
		'8','9','+', '/', '\0'
};


char* base64_decode(char const* base64Str, char* debase64Str, int encodeStrLen)
{
	int i = 0;
	int j = 0;
	int k = 0;
	char temp[4] = "";

	for (i = 0; i < encodeStrLen; i += 4) {
		for (j = 0; j < 64; j++) {
			if (*(base64Str + i) == base64char[j]) {
				temp[0] = j;
			}
		}

		for (j = 0; j < 64; j++) {
			if (*(base64Str + i + 1) == base64char[j]) {
				temp[1] = j;
			}
		}

		for (j = 0; j < 64; j++) {
			if (*(base64Str + i + 2) == base64char[j]) {
				temp[2] = j;
			}
		}

		for (j = 0; j < 64; j++) {
			if (*(base64Str + i + 3) == base64char[j]) {
				temp[3] = j;
			}
		}

		*(debase64Str + k++) = ((temp[0] << 2) & 0xFC) | ((temp[1] >> 4) & 0x03);
		if (*(base64Str + i + 2) == '=')
			break;

		*(debase64Str + k++) = ((temp[1] << 4) & 0xF0) | ((temp[2] >> 2) & 0x0F);
		if (*(base64Str + i + 3) == '=')
			break;

		*(debase64Str + k++) = ((temp[2] << 6) & 0xF0) | (temp[3] & 0x3F);
	}
	return debase64Str;
}


VOID CALLBACK BacnetWebViewAppWindow::onTimer(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{

	/*if (auto app = (BacnetWebViewAppWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA))
	{
		printf("Timer called\n");
		Json::Reader reader;
		Json::Value json;
		reader.parse(app->JsonData, json, false);
		int index = Find_Index_Input_By_Label("IDT");
		json["variable"]["name"] = "IDT";
		json["variable"]["value"] = m_Input_data.at(index).value / 1000;
		Json::StyledWriter styleWriter;
		Json::String writeStr = styleWriter.write(json);
		CString Response = writeStr.c_str();
		app->m_webView->ExecuteScript(L"MessageReceived(" + Response + ")", Callback<ICoreWebView2ExecuteScriptCompletedHandler>(app, &BacnetWebViewAppWindow::ExecuteScriptResponse).Get());
		index = Find_Index_Input_By_Label("ODT");
		json["variable"]["name"] = "ODT";
		json["variable"]["value"] = m_Input_data.at(index).value / 1000;
		writeStr = styleWriter.write(json);
		Response = writeStr.c_str();
		app->m_webView->ExecuteScript(L"MessageReceived(" + Response + ")", Callback<ICoreWebView2ExecuteScriptCompletedHandler>(app, &BacnetWebViewAppWindow::ExecuteScriptResponse).Get());
	}*/
}

std::vector<std::string> BacnetWebViewAppWindow::GetGraphicFiles(CString path)
{
	vector<string> files;
	DIR* dir;
	struct dirent* ent;
	CStringA charstr(path);
	if ((dir = opendir((const char*)charstr)) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			//  printf("%s\n", ent->d_name);
			const char* ext = strrchr(ent->d_name, '.');
			if ((ext) || (ext != ent->d_name))
			{
				if (ent->d_namlen > 2 && (strcmp(ext, ".grp") == 0))
					files.push_back(ent->d_name);
			}
		}
		closedir(dir);
	}
	else {
		/* could not open directory */
		perror("");
	}
	return files;

}
void BacnetWebViewAppWindow::get_png_image_dimensions(CString& file_path, unsigned int& width, unsigned int& height)
{
	unsigned char buf[8];

	std::ifstream in(file_path);
	in.seekg(16);
	in.read(reinterpret_cast<char*>(&buf), 8);

	width = (buf[0] << 24) + (buf[1] << 16) + (buf[2] << 8) + (buf[3] << 0);
	height = (buf[4] << 24) + (buf[5] << 16) + (buf[6] << 8) + (buf[7] << 0);
}
#pragma endregion WebviewRelatedMethods
//#include <cstdio>
//#include <iostream>


//void redirect_stdout_stderr() {
//	std::freopen("output.log", "w", stdout); // Redirect stdout to output.log
//	std::freopen("error.log", "w", stderr);  // Redirect stderr to error.log
//}

//BOOL WINAPI ConsoleHandler(DWORD dwCtrlType)
//{
//	switch (dwCtrlType)
//	{
//	case CTRL_CLOSE_EVENT:
//		// 处理控制台关闭事件，防止程序终止
//		//std::cout << "控制台窗口关闭事件被捕获，程序继续运行。" << std::endl;
//		Sleep(1);
//		return TRUE;
//	default:
//		return FALSE;
//	}
//}

//void setup_console() 
//{
//	AllocConsole();
//	freopen("CONOUT$", "w", stdout);
//	freopen("CONOUT$", "w", stderr);
//
//	// 设置控制台关闭事件处理程序
//	//SetConsoleCtrlHandler(ConsoleHandler, TRUE);
//}

// Function to convert wide string (wchar_t) to a regular string (std::string)
std::string wide_string_to_string(const std::wstring& wide_str) {
	std::string result(wide_str.begin(), wide_str.end());
	return result;
}

// Function to get the directory of the executable
std::string get_executable_directory() {
	wchar_t path[MAX_PATH];
	GetModuleFileNameW(NULL, path, MAX_PATH);  // Use wide-character version
	std::wstring full_path(path);

	// Find the last backslash to separate the directory from the executable name
	size_t pos = full_path.find_last_of(L"\\/");  // Note the L for wide-string literal
	if (pos != std::wstring::npos) {
		std::wstring exe_dir_w = full_path.substr(0, pos);  // Extract wide-string directory part
		return wide_string_to_string(exe_dir_w);  // Convert to std::string and return
	}
	return "";
}

int webview_run_server() {
	std::string exe_dir = get_executable_directory();

	SetCurrentDirectoryA(exe_dir.c_str());
	// setup_console();
	// redirect_stdout_stderr();

	RustError result = run_server();
	if (result != RustError::Ok) {

		SetPaneString(BAC_SHOW_MISSION_RESULTS, L"Couldn't run the webview API server");
		return 1;
	}

	return 0;
}

// Function to clear EBWebView cache folder
void BacnetWebViewAppWindow::ClearEBWebViewCache()
{
	PWSTR userDataPath;
	HRESULT hrfolder = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &userDataPath);
	if (SUCCEEDED(hrfolder)) {
		std::wstring userDataFolder(userDataPath);
		userDataFolder += L"\\T3000\\EBWebView";

		if (PathFileExists(userDataFolder.c_str())) {
			// Multiple deletion attempts to handle file locks
			for (int attempts = 0; attempts < 3; attempts++) {
				DeleteDirectoryRecursive(userDataFolder);
				if (!PathFileExists(userDataFolder.c_str()))
					break;
				Sleep(100);
			}
		}
		CoTaskMemFree(userDataPath);
	}
}
