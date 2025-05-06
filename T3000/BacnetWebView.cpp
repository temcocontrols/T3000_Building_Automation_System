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
extern int Write_Webview_Data_Special(int panelid, UINT nserialnumber, int nscreenindex,int element_count);
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
};

#define READ_INPUT_VARIABLE  0
#define READ_OUTPUT_VARIABLE 1
#define READ_VARIABLE 2
#define READ_SCHEDULE 3
#define READ_CALENDER 4
#define READ_PROGRAM 5

int save_button_click = 0;
extern char* ispoint_ex(char* token, int* num_point, byte* var_type, byte* point_type, int* num_panel, int* num_net, int network, unsigned char& sub_panel, byte panel, int* netpresent);


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
extern BacnetWeeklyRoutine* WeeklyRoutine_Window ;
extern BacnetAnnualRoutine* AnnualRoutine_Window ;
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
		if (Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub >= WEBVIEW_JSON_FEATURE) //643 
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
		//FreeConsole(); //
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
	//
	//CRect rectDlg;
	////GetClientRect(rectDlg);// //1
	//::GetWindowRect(this,;// //2
	//ScreenToClient(rectDlg);

	//printf(:: % d,  : % d,  : % d,  : % d\r\n, rectDlg.bottom, rectDlg.right, rectDlg.Width(), rectDlg.Height());
	CloseWebView(true);
	m_isClosed = true;
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

	/*wil::com_ptr<ICoreWebView2CompositionController> compositionController =
		m_controller.query<ICoreWebView2CompositionController>();*/
	m_dropTarget = Make<DropTarget>();
	m_dropTarget->Init(m_mainWindow, coreWebView2.get(), m_controller.get());

	if (m_webView != nullptr && m_initialUri != L"")
		m_webView->Navigate(m_initialUri.c_str());
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

#include <mutex>
std::mutex handleWebViewMsgMutex;
void HandleWebViewMsg(CString msg ,CString &outmsg, int msg_source = 0)
{
	//  std::lock_guard 
	std::lock_guard<std::mutex> lock(handleWebViewMsgMutex);

	Json::Value json;
	std::string message = CT2A(msg);
	Json::Reader reader;
	reader.parse(message, json, false);
	int action = json.get("action", Json::nullValue).asInt();


	Json::StreamWriterBuilder builder;
	builder["indentation"] = ""; // If you want whitespace-less output
	Json::Value tempjson;
	int grp_index = 0;  //;
	int grp_serial_number = 0; //  zip;
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
		char ipStr[16]; // IP
		unsigned char* ipAddr = g_Device_Basic_Setting[npanel_id].reg.ip_addr;
		//  sprintf  IP 
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
			//There is also additional data that does not need to be passed to the webview interface
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
		if (msg_source == 0)//T3000
		{
			grp_serial_number = g_selected_serialnumber; 
			grp_index = screen_list_line;
			panel_id = bac_gloab_panel;
		}
		else if(msg_source == 1)//
		{
			panel_id = json.get("panelId", Json::nullValue).asInt(); //anelId
			grp_index = json.get("viewitem", Json::nullValue).asInt(); //T3000ndex   ndex
			grp_serial_number = json.get("serialNumber", Json::nullValue).asInt();
			//panel_id  grp_index  anel ip;
			
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
			int n_read_result = Read_Webview_Data_Special(panel_id, grp_serial_number, grp_index );
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
		if (temp_find.FindFile(des_file) == 0) //  grp grp webview ;
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
		if (temp_find.FindFile(des_lib_file) == 0) //  lib grp webview ;
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
			filelib.Read(nlibbuff, len * 2 + 1);   //Read( void* lpBuf, UINT nCount ) lpBufBufnCount
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
		file.Read(nbuff, len * 2 + 1);   //Read( void* lpBuf, UINT nCount ) lpBufBufnCount
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
		int panelId = json.get("panelId", Json::nullValue).asInt(); //anelId
		int save_grp_index = -1;

		
		if (msg_source == 0)//T3000
		{
			grp_serial_number = g_selected_serialnumber; //
			save_grp_index = screen_list_line;
			panelId = bac_gloab_panel; //panelID0 ;
			save_button_click = 1;
		}
		else if (msg_source == 1)//
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

		// ;
		//panel  .
		int n_write_result =  Write_Webview_Data_Special(panelId, grp_serial_number, save_grp_index, temp_elementcount);
		if (n_write_result == -3)
		{
			if (msg_source == 0)
				SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write too often. Ignore this request!"));
			WrapErrorMessage(builder, tempjson, outmsg, _T("Write too often. Ignore this request!"));
			break;
		}
		else if(n_write_result < 0)
		{
			if (msg_source == 0)
				SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Failed to store data!"));
			WrapErrorMessage(builder, tempjson, outmsg, _T("Failed to store data!"));
			break;
		}

		

		break;
	}
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
					//
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
					//
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
					//
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
					//
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
					//
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
					//
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
				send_index++; //  
			}
		}
		const std::string output = Json::writeString(builder, tempjson);
		CString temp_cs(output.c_str());
		outmsg = temp_cs;
		//m_webView->PostWebMessageAsJson(temp_cs);
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

				if (entry_type == BAC_IN)
				{
					if (entry_index >= BAC_INPUT_ITEM_COUNT)
						continue;
					CString temp_cs4;
					int ret_index4 = Post_Background_Read_Message_ByPanel(npanel_id, READINPUT_T3000, entry_index + 1);  //send message to background ead 199IN3
					if (ret_index4 >= 0)
					{
						memcpy(&g_Input_data[npanel_id].at(entry_index), &m_backbround_data.at(ret_index4).ret_data.m_group_input_data, sizeof(Str_in_point));
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
					int ret_index_out = Post_Background_Read_Message_ByPanel(npanel_id, READOUTPUT_T3000, entry_index + 1);  //send message to background ead 199OUT3
					if (ret_index_out >= 0)
					{
						memcpy(&g_Output_data[npanel_id].at(entry_index), &m_backbround_data.at(ret_index_out).ret_data.m_group_output_data, sizeof(Str_out_point));
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
					int ret_index_var = Post_Background_Read_Message_ByPanel(npanel_id, READVARIABLE_T3000, entry_index + 1);  //send message to background ead 199OUT3
					if (ret_index_var >= 0)
					{
						memcpy(&g_Variable_data[npanel_id].at(entry_index), &m_backbround_data.at(ret_index_var).ret_data.m_group_variable_data, sizeof(Str_variable_point));
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
					int ret_index_prg = Post_Background_Read_Message_ByPanel(npanel_id, READPROGRAM_T3000, entry_index + 1);  //send message to background ead 199OUT3
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
					int ret_index_sch = Post_Background_Read_Message_ByPanel(npanel_id, READWEEKLYROUTINE_T3000, entry_index + 1);  //send message to background ead 199OUT3
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
					int ret_index_hol = Post_Background_Read_Message_ByPanel(npanel_id, READANNUALROUTINE_T3000, entry_index + 1);  //send message to background ead 199IN3
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
					int ret_index_out = Post_Background_Read_Message_ByPanel(npanel_id, READSCREEN_T3000, entry_index + 1);  //send message to background ead 199OUT3
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
					int ret_index_monitor = Post_Background_Read_Message_ByPanel(npanel_id, READMONITOR_T3000, entry_index + 1);  //send message to background ead 199OUT3
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
		if (npanel_id == bac_gloab_panel) // panel 
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
			ret = TRUE; //
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
				//;
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

		char* ret_result = (char*)malloc(sizeof(char) * file_length);	//
		memset(ret_result, 0, sizeof(char) * file_length);		//
		const char* temp_ret = strstr((char const*)file_data.c_str(), "base64,"); //;
		if (temp_ret == NULL)
			break;
		const std::string temp_image_data = strstr((char const*)file_data.c_str(), "base64,") + 7; //;
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
		if (temp_find.FindFile(temp_delete_file_path)) //  IMAGE 
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
	default:
		break;
	}

}


void BacnetWebViewAppWindow::ProcessWebviewMsg(CString msg)
{
	CString outmsg;
	HandleWebViewMsg(msg, outmsg);
	if(!outmsg.IsEmpty())
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
//		// 
//		//std::cout << "" << std::endl;
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
//	// 
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