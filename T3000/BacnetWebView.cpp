
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

#include "BacnetWebView.h"
#include "MainFrm.h"
using namespace Microsoft::WRL;
size_t thread_local BacnetWebViewAppWindow::s_appInstances = 0;

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
	GET_ENTRY = 0,
	GET_INITIAL_DATA = 1,
	SAVE_GRAPHIC_DATA = 2,
	UPDATE_ENTRY = 3,
};

enum WEBVIEW_ENTRY_TYPE
{
	INPUT_TYPE = 0,
	OUTPUT = 1,
	VARIABLE = 2,
	PROGRAM = 3,
	SCHEDULE = 4,
	HOLIDAY = 5
};

#define READ_INPUT_VARIABLE  0
#define READ_OUTPUT_VARIABLE 1
#define READ_VARIABLE 2
#define READ_SCHEDULE 3
#define READ_CALENDER 4
#define READ_PROGRAM 5


extern char* ispoint_ex(char* token, int* num_point, byte* var_type, byte* point_type, int* num_panel, int* num_net, int network, unsigned char& sub_panel, byte panel, int* netpresent);

extern CBacnetProgram* Program_Window;

BacnetWebViewAppWindow::BacnetWebViewAppWindow(
	UINT creationModeId,
	std::wstring initialUri,
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
	std::wstring title = L"BacnetWebView Demo Application 0.3";
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

	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	CString image_fordor = g_strExePth + CString("Database\\Buildings\\") + pFrame->m_strCurMainBuildingName + _T("\\image");
	CString temp_item;
	temp_item.Format(_T("%u_%d.txt"), g_selected_serialnumber, screen_list_line);
	des_file = image_fordor + _T("\\") + temp_item;

	SetWindowLongPtr(m_mainWindow, GWLP_USERDATA, (LONG_PTR)this);
	ShowWindow(m_mainWindow, SW_SHOWDEFAULT);
	UpdateWindow(m_mainWindow);

	wil::unique_cotaskmem_string version_info;
	HRESULT hr = GetAvailableCoreWebView2BrowserVersionString(nullptr, &version_info);
	if (hr == S_OK && version_info != nullptr)
	{
		InitializeWebView();
	}

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
		int retValue = 0;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, NULL);
		NotifyClosed();
		if (--s_appInstances == 0)
		{
			PostQuitMessage(retValue);
		}
		DeleteObject(m_appBackgroundImageHandle);
		DeleteDC(m_memHdc);
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
		WCHAR userDataFolder[MAX_PATH] = L"";
		// Obtain the absolute path for relative paths that include "./" or "../"
		_wfullpath(
			userDataFolder, GetLocalPath(L".WebView2", true).c_str(), MAX_PATH);
		std::wstring userDataFolderPath(userDataFolder);

		std::wstring message = L"Are you sure you want to clean up the user data folder at\n";
		message += userDataFolderPath;
		message += L"\n?\nWarning: This action is not reversible.\n\n";
		message += L"Click No if there are other open WebView instances.\n";

		if (MessageBox(m_mainWindow, message.c_str(), L"Cleanup User Data Folder", MB_YESNO) ==
			IDYES)
		{
			//CHECK_FAILURE(DeleteFileRecursive(userDataFolderPath));
		}
	}
}

void BacnetWebViewAppWindow::CloseAppWindow()
{
	CloseWebView();
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
	auto options = Microsoft::WRL::Make<CoreWebView2EnvironmentOptions>();
	HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(
		subFolder, nullptr, options.Get(),
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
		TRACE(receivedMessage);
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

void BacnetWebViewAppWindow::ProcessWebviewMsg(CString msg)
{
	Json::Value json;
	std::string message = CT2A(msg);
	Json::Reader reader;
	reader.parse(message, json, false);
	int type = json.get("action", Json::nullValue).asInt();
	Json::StreamWriterBuilder builder;
	builder["indentation"] = ""; // If you want whitespace-less output
	Json::Value tempjson;
	switch (type)
	{
	case WEBVIEW_MESSAGE_TYPE::GET_ENTRY:
	{
		int panel_id = json.get("panelId", Json::nullValue).asInt();
		int entry_id = json.get("entryId", Json::nullValue).asInt();
		int entry_type = json.get("entryType", Json::nullValue).asInt();

		int entry_index = entry_id -1;

		tempjson["action"] = "updateEntry";
		tempjson["panelId"] = panel_id;
		tempjson["entryId"] = entry_id;
		tempjson["entryType"] = entry_type;

		switch (entry_type)
		{
		case WEBVIEW_ENTRY_TYPE::INPUT_TYPE:
		{
			if ((entry_id > 0) && entry_id > BAC_INPUT_ITEM_COUNT)
			{
				break;
			}

			tempjson["data"]["index"] = entry_index;
			tempjson["data"]["id"] = "IN" + to_string(entry_id);
			tempjson["data"]["description"] = (char*)m_Input_data.at(entry_index).description;
			tempjson["data"]["label"] = (char*)m_Input_data.at(entry_index).label;
			tempjson["data"]["unit"] = m_Input_data.at(entry_index).range;
			tempjson["data"]["auto_manual"] = m_Input_data.at(entry_index).auto_manual;
			tempjson["data"]["value"] = m_Input_data.at(entry_index).value;
			tempjson["data"]["filter"] = m_Input_data.at(entry_index).filter;
			tempjson["data"]["control"] = m_Input_data.at(entry_index).control;
			tempjson["data"]["digital_analog"] = m_Input_data.at(entry_index).digital_analog;
			tempjson["data"]["range"] = m_Input_data.at(entry_index).range;
			tempjson["data"]["calibration_sign"] = m_Input_data.at(entry_index).calibration_sign;
			tempjson["data"]["calibration_h"] = m_Input_data.at(entry_index).calibration_h;
			tempjson["data"]["calibration_l"] = m_Input_data.at(entry_index).calibration_l;

			break;
		}
		case WEBVIEW_ENTRY_TYPE::OUTPUT:
		{
			if ((entry_id > 0) && entry_id > BAC_OUTPUT_ITEM_COUNT)
			{
				break;
			}

			tempjson["data"]["index"] = entry_index;
			tempjson["data"]["id"] = "OUT" + to_string(entry_id);
			tempjson["data"]["description"] = (char*)m_Output_data.at(entry_index).description;
			tempjson["data"]["label"] = (char*)m_Output_data.at(entry_index).label;
			tempjson["data"]["auto_manual"] = m_Output_data.at(entry_index).auto_manual;
			tempjson["data"]["value"] = m_Output_data.at(entry_index).value;
			tempjson["data"]["low_voltage"] = m_Output_data.at(entry_index).low_voltage;
			tempjson["data"]["high_voltage"] = m_Output_data.at(entry_index).high_voltage;
			tempjson["data"]["range"] = m_Output_data.at(entry_index).range;
			tempjson["data"]["control"] = m_Output_data.at(entry_index).control;
			tempjson["data"]["digital_analog"] = m_Output_data.at(entry_index).digital_analog;
			tempjson["data"]["hw_switch_status"] = m_Output_data.at(entry_index).hw_switch_status;

			break;
		}
		case WEBVIEW_ENTRY_TYPE::VARIABLE:
		{
			if ((entry_id > 0) && entry_id > BAC_VARIABLE_ITEM_COUNT)
			{
				break;
			}

			tempjson["data"]["index"] = entry_index;
			tempjson["data"]["id"] = "VAR" + to_string(entry_id);
			tempjson["data"]["description"] = (char*)m_Variable_data.at(entry_index).description;
			tempjson["data"]["label"] = (char*)m_Variable_data.at(entry_index).label;
			tempjson["data"]["auto_manual"] = m_Variable_data.at(entry_index).auto_manual;
			tempjson["data"]["value"] = m_Variable_data.at(entry_index).value;
			tempjson["data"]["range"] = m_Variable_data.at(entry_index).range;
			tempjson["data"]["control"] = m_Variable_data.at(entry_index).control;
			tempjson["data"]["digital_analog"] = m_Variable_data.at(entry_index).digital_analog;

			break;
		}
		case WEBVIEW_ENTRY_TYPE::PROGRAM:
		{
			if ((entry_id > 0) && entry_id > BAC_PROGRAM_ITEM_COUNT)
			{
				break;
			}

			tempjson["data"]["index"] = entry_index;
			tempjson["data"]["id"] = "PRG" + to_string(entry_id);
			tempjson["data"]["description"] = (char*)m_Program_data.at(entry_index).description;
			tempjson["data"]["label"] = (char*)m_Program_data.at(entry_index).label;
			tempjson["data"]["auto_manual"] = m_Program_data.at(entry_index).auto_manual;
			tempjson["data"]["status"] = m_Program_data.at(entry_index).on_off;

			break;
		}
		case WEBVIEW_ENTRY_TYPE::SCHEDULE:
		{
			if ((entry_id > 0) && entry_id > BAC_SCHEDULE_COUNT)
			{
				break;
			}

			tempjson["data"]["index"] = entry_index;
			tempjson["data"]["id"] = "SCH" + to_string(entry_id);
			tempjson["data"]["description"] = (char*)m_Weekly_data.at(entry_index).description;
			tempjson["data"]["label"] = (char*)m_Weekly_data.at(entry_index).label;
			tempjson["data"]["auto_manual"] = m_Weekly_data.at(entry_index).auto_manual;
			tempjson["data"]["output"] = m_Weekly_data.at(entry_index).value;
			tempjson["data"]["state1"] = m_Weekly_data.at(entry_index).override_1_value;
			tempjson["data"]["state2"] = m_Weekly_data.at(entry_index).override_2_value;

			break;
		}
		case WEBVIEW_ENTRY_TYPE::HOLIDAY:
		{
			if ((entry_id > 0) && entry_id > BAC_HOLIDAY_COUNT)
			{
				break;
			}

			tempjson["data"]["index"] = entry_index;
			tempjson["data"]["id"] = "CAL" + to_string(entry_id);
			tempjson["data"]["description"] = (char*)m_Annual_data.at(entry_index).description;
			tempjson["data"]["label"] = (char*)m_Annual_data.at(entry_index).label;
			tempjson["data"]["auto_manual"] = m_Annual_data.at(entry_index).auto_manual;
			tempjson["data"]["value"] = m_Annual_data.at(entry_index).value;

			break;
		}

		break;
		}

		const std::string output = Json::writeString(builder, tempjson);
		CString temp_cs(output.c_str());

		m_webView->PostWebMessageAsJson(temp_cs);
		//Post_Refresh_One_Message(g_bac_instance, READINPUT_T3000, entry_index, entry_index, sizeof(Str_in_point));
		break;
	}
	case WEBVIEW_MESSAGE_TYPE::GET_INITIAL_DATA:
	{
		CFile file;


		CFileFind temp_find;
		if (temp_find.FindFile(des_file) == 0)
		{
			break;
		}

		file.Open(des_file, CFile::modeRead, NULL);
		DWORD len = file.GetLength();
		if (len == 0)
			break;
		WCHAR* nbuff = new WCHAR[len + 1];
		memset(nbuff, 0, 2 * (len + 1));
		file.Read(nbuff, len * 2 + 1);   //Read( void* lpBuf, UINT nCount ) lpBuf是用于接收读取到的数据的Buf指针nCount是从文件读取的字节数
		Json::Value tempjson;
		tempjson["action"] = "setInitialData";
		wstring nbuff_wstring(nbuff);
		string nbuff_str(nbuff_wstring.begin(), nbuff_wstring.end());
		tempjson["data"] = nbuff_str;
		const std::string output = Json::writeString(builder, tempjson);
		CString tempjson_str(output.c_str());
		TRACE(nbuff);
		m_webView->PostWebMessageAsJson(tempjson_str);
		delete nbuff;
		break;
	}
	case WEBVIEW_MESSAGE_TYPE::SAVE_GRAPHIC_DATA:
	{
		CFile file;

		const std::string file_output = Json::writeString(builder, json["data"]);
		CString file_temp_cs(file_output.c_str());
		file.Open(des_file, CFile::modeCreate | CFile::modeWrite | CFile::modeCreate, NULL);
		file.Write(file_temp_cs, file_temp_cs.GetLength() * 2);
		file.Close();
		Json::Value tempjson;
		tempjson["action"] = "saveGraphicResponse";
		tempjson["data"]["status"] = true;
		const std::string output = Json::writeString(builder, tempjson);
		CString temp_cs(output.c_str());

		m_webView->PostWebMessageAsJson(temp_cs);

		break;
	}
	case WEBVIEW_MESSAGE_TYPE::UPDATE_ENTRY:
	{
		Json::Value tempjson;
		int panel_id = json.get("panelId", Json::nullValue).asInt();
		int entry_id = json.get("entryId", Json::nullValue).asInt();
		int entry_type = json.get("entryType", Json::nullValue).asInt();
		const std::string field = json.get("field", Json::nullValue).asString();

		int entry_index = entry_id - 1;
		switch (entry_type)
		{
		case WEBVIEW_ENTRY_TYPE::INPUT_TYPE:
		{

			if ((entry_id > 0) && entry_id > BAC_INPUT_ITEM_COUNT)
			{
				break;
			}
			if (field.compare("control") == 0) {
				m_Input_data.at(entry_index).control = json["data"][field].asInt();
			}
			else if (field.compare("auto_manual") == 0) {
				m_Input_data.at(entry_index).auto_manual = json["data"][field].asInt();
			}
			
			Write_Private_Data_Blocking(WRITEINPUT_T3000, entry_index, entry_index, g_bac_instance);
			::PostMessage(m_input_dlg_hwnd, WM_REFRESH_BAC_INPUT_LIST, entry_index, REFRESH_ON_ITEM);
			break;
		}
		}
		tempjson["action"] = "updateEntryResponse";
		tempjson["data"]["status"] = true;
		const std::string output = Json::writeString(builder, tempjson);
		CString temp_cs(output.c_str());

		m_webView->PostWebMessageAsJson(temp_cs);

		break;
	}
	break;
	}
}

//void BacnetWebViewAppWindow::ProcessWebviewMsg(CString msg)
//{
//   
//    Json::Value json;
//    std::string message = CT2A(msg);
//    Json::Reader reader;
//     /*Json::Value json;
//     Json::wri writer;
//     //writer.write()
//   std::string str = R"~(
//        {
//            "id": 1,
//            "parameters":
//            {
//                "var1": "bla",
//                "var2": 7,
//                "var3": "base64 encoded binary data"
//            }
//        }
//        )~";//CT2A(receivedMessage);
//    reader.parse(str, json, false);*/
//    /*int  id = json.get("id", Json::nullValue).asInt();
//    Json::Value param = json.get("parameters", Json::nullValue);
//    int var2 = param.get("var2", Json::nullValue).asInt();*/
//    
//    reader.parse(message, json, false);
//    int type = json.get("action", Json::nullValue).asInt();
//    switch (type)
//    {
//         case WEBVIEW_MESSAGE_TYPE::READ_VARIABLES:
//         {
//             int sub_action = json.get("sub_action", Json::nullValue).asInt();
//
//                 if (auto app = (BacnetWebViewAppWindow*)GetWindowLongPtr(m_mainWindow, GWLP_USERDATA))
//                 {
//                     Json::Reader reader;
//                     Json::Value tempjson;
//                     reader.parse(app->JsonData, tempjson, false);
//                     tempjson["action"] = sub_action;
//                     if (sub_action == READ_INPUT_VARIABLE)
//                     {
//                         tempjson["size"] = m_Input_data.size() - 1;
//                         for (int i = 0; i < m_Input_data.size(); i++)
//                         {
//                             tempjson["data"][i]["index"] = i;
//                             tempjson["data"][i]["id"] = "IN"+ to_string(i);
//                             tempjson["data"][i]["desc"] = (char*)m_Input_data.at(i).description;
//                             tempjson["data"][i]["label"] = (char*)m_Input_data.at(i).label;
//                             tempjson["data"][i]["unit"] = m_Input_data.at(i).range;
//                             tempjson["data"][i]["auto_manual"] = m_Input_data.at(i).auto_manual;
//                         }
//                     }
//                     else  if (sub_action == READ_OUTPUT_VARIABLE)
//                     {
//                         tempjson["size"] = m_Output_data.size() - 1;
//                         for (int i = 0; i < m_Output_data.size(); i++)
//                         {
//                             tempjson["data"][i]["index"] = i;
//                             tempjson["data"][i]["id"] = "OUT" + to_string(i);
//                             tempjson["data"][i]["desc"] = (char*)m_Output_data.at(i).description;
//                             tempjson["data"][i]["label"] = (char*)m_Output_data.at(i).label;
//                             tempjson["data"][i]["unit"] = m_Output_data.at(i).range;
//                             tempjson["data"][i]["auto_manual"] = m_Output_data.at(i).auto_manual;
//                         }
//                     }
//                     else  if (sub_action == READ_VARIABLE)
//                     {
//                         tempjson["size"] = m_Variable_data.size() - 1;
//                         for (int i = 0; i < m_Variable_data.size(); i++)
//                         {
//                             tempjson["data"][i]["index"] = i;
//                             tempjson["data"][i]["id"] = "VAR" + to_string(i);
//                             tempjson["data"][i]["desc"] = (char*)m_Variable_data.at(i).description;
//                             tempjson["data"][i]["label"] = (char*)m_Variable_data.at(i).label;
//                             tempjson["data"][i]["unit"] = m_Variable_data.at(i).range;
//                             tempjson["data"][i]["auto_manual"] = m_Variable_data.at(i).auto_manual;
//                         }
//                     }
//                     else  if (sub_action == READ_SCHEDULE)
//                     {
//                         tempjson["size"] = m_Weekly_data.size() - 1;
//                         for (int i = 0; i < m_Weekly_data.size(); i++)
//                         {
//                             tempjson["data"][i]["index"] = i;
//                             tempjson["data"][i]["id"] = "SCH" + to_string(i);
//                             tempjson["data"][i]["desc"] = (char*)m_Weekly_data.at(i).description;
//                             tempjson["data"][i]["label"] = (char*)m_Weekly_data.at(i).label;
//                             tempjson["data"][i]["unit"] ="";
//                             tempjson["data"][i]["auto_manual"] = m_Weekly_data.at(i).auto_manual;
//                         }
//                     }
//                     else  if (sub_action == READ_CALENDER)
//                     {
//                         tempjson["size"] = m_Annual_data.size() - 1;
//                         for (int i = 0; i < m_Annual_data.size(); i++)
//                         {
//                             tempjson["data"][i]["index"] = i;
//                             tempjson["data"][i]["id"] = "CAL" + to_string(i);
//                             tempjson["data"][i]["desc"] = (char*)m_Annual_data.at(i).description;
//                             tempjson["data"][i]["label"] = (char*)m_Annual_data.at(i).label;
//                             tempjson["data"][i]["unit"] = "";
//                             tempjson["data"][i]["auto_manual"] = m_Annual_data.at(i).auto_manual;
//                         }
//                     }
//                     else  if (sub_action == READ_PROGRAM)
//                     {
//                         tempjson["size"] = m_Program_data.size() - 1;
//                         for (int i = 0; i < m_Program_data.size(); i++)
//                         {
//                             tempjson["data"][i]["index"] = i;
//                             tempjson["data"][i]["id"] = "PRG" + to_string(i);
//                             tempjson["data"][i]["desc"] = (char*)m_Program_data.at(i).description;
//                             tempjson["data"][i]["label"] = (char*)m_Program_data.at(i).label;
//                             tempjson["data"][i]["unit"] = "";
//                             tempjson["data"][i]["Status"] = m_Program_data.at(i).on_off;
//                             tempjson["data"][i]["auto_manual"] = m_Program_data.at(i).auto_manual;
//                         }
//                     }
//                     Json::StyledWriter styleWriter;
//                     Json::String writeStr = styleWriter.write(tempjson);
//                     CString Response = writeStr.c_str();
//                     app->m_webView->ExecuteScript(L"PopulateVariableList(" + Response + ")", Callback<ICoreWebView2ExecuteScriptCompletedHandler>(app, &BacnetWebViewAppWindow::ExecuteScriptResponse).Get());
//                 }
//             break;
//         }
//        /* case WEBVIEW_MESSAGE_TYPE::READ_OUTPUT_VARIABLES:
//         {
//             if (auto app = (BacnetWebViewAppWindow*)GetWindowLongPtr(m_mainWindow, GWLP_USERDATA))
//             {
//                 Json::Reader reader;
//                 Json::Value tempjson;
//                 reader.parse(app->JsonData, tempjson, false);
//                 tempjson["action"] = WEBVIEW_MESSAGE_TYPE::READ_OUTPUT_VARIABLES;
//                 tempjson["size"] = m_Output_data.size() - 1;
//                 for (int i = 0; i < m_Output_data.size(); i++)
//                 {
//                     tempjson["data"][i]["index"] = i;
//                     tempjson["data"][i]["desc"] = (char*)m_Output_data.at(i).description;
//                     tempjson["data"][i]["label"] = (char*)m_Output_data.at(i).label;
//                     tempjson["data"][i]["unit"] = m_Output_data.at(i).range;
//                 }
//                 Json::StyledWriter styleWriter;
//                 Json::String writeStr = styleWriter.write(tempjson);
//                 CString Response = writeStr.c_str();
//                 app->m_webView->ExecuteScript(L"PopulateVariableList(" + Response + ")", Callback<ICoreWebView2ExecuteScriptCompletedHandler>(app, &BacnetWebViewAppWindow::ExecuteScriptResponse).Get());
//             }
//             break;
//         }
//         case WEBVIEW_MESSAGE_TYPE::READ_VARIABLE:
//         {
//             if (auto app = (BacnetWebViewAppWindow*)GetWindowLongPtr(m_mainWindow, GWLP_USERDATA))
//             {
//                 Json::Reader reader;
//                 Json::Value tempjson;
//                 reader.parse(app->JsonData, tempjson, false);
//                 tempjson["action"] = WEBVIEW_MESSAGE_TYPE::READ_VARIABLES;
//                 tempjson["size"] = m_Variable_data.size() - 1;
//                 for (int i = 0; i < m_Variable_data.size(); i++)
//                 {
//                     tempjson["data"][i]["index"] = i;
//                     tempjson["data"][i]["desc"] = (char*)m_Variable_data.at(i).description;
//                     tempjson["data"][i]["label"] = (char*)m_Variable_data.at(i).label;
//                     tempjson["data"][i]["unit"] = m_Variable_data.at(i).range;
//                 }
//                 Json::StyledWriter styleWriter;
//                 Json::String writeStr = styleWriter.write(tempjson);
//                 CString Response = writeStr.c_str();
//                 app->m_webView->ExecuteScript(L"PopulateVariableList(" + Response + ")", Callback<ICoreWebView2ExecuteScriptCompletedHandler>(app, &BacnetWebViewAppWindow::ExecuteScriptResponse).Get());
//             }
//             break;
//         }*/
//         case WEBVIEW_MESSAGE_TYPE::READ_DATA:
//         {
//            if (auto app = (BacnetWebViewAppWindow*)GetWindowLongPtr(m_mainWindow, GWLP_USERDATA))
//            {
//                Json::Reader reader;
//                Json::Value tempjson;
//                reader.parse(app->JsonData, tempjson, false);
//                tempjson["action"] = WEBVIEW_MESSAGE_TYPE::READ_DATA;
//                tempjson["element_id"] = json.get("element_id", Json::nullValue).asInt();
//                int var_index = json.get("object_variable", Json::nullValue).asInt();
//                tempjson["device_type"] = bacnet_device_type;
//
//                if (json.get("object_type", Json::nullValue).asInt() == 0)// input
//                {
//                    CString unit, value;
//                    ((CBacnetInput*)pDialog[WINDOW_INPUT])->GetInputUnitandValue(var_index, unit, value);
//                    CT2CA string(value);
//                    std::string tempstr (string);
//                    tempjson["value"] = tempstr.c_str();
//                    CT2CA stringunit(unit);
//                    std::string tempstrunit(stringunit);
//                    tempjson["unit"] = tempstrunit.c_str();
//                    tempjson["object_name"] = (char*)m_Input_data.at(var_index).description;
//                    tempjson["object_label"] = (char*)m_Input_data.at(var_index).label;
//                    tempjson["auto_manual"] = m_Input_data.at(var_index).auto_manual;
//                }
//                else if (json.get("object_type", Json::nullValue).asInt() == 1)// output
//                {
//                    tempjson["value"] = m_Output_data.at(var_index).value / 1000;
//                    tempjson["unit"] = m_Output_data.at(var_index).range;
//                    tempjson["object_name"] = (char*)m_Output_data.at(var_index).description;
//                    tempjson["object_label"] = (char*)m_Output_data.at(var_index).label;
//                    tempjson["auto_manual"] = m_Output_data.at(var_index).auto_manual;
//                }
//                else if (json.get("object_type", Json::nullValue).asInt() == 2)// variables
//                {
//                    tempjson["value"] = m_Variable_data.at(var_index).value / 1000;
//
//                    tempjson["unit"] = m_Variable_data.at(var_index).range;
//                    tempjson["object_name"] = (char*)m_Variable_data.at(var_index).description;
//                    tempjson["object_label"] = (char*)m_Variable_data.at(var_index).label;
//                    tempjson["auto_manual"] = m_Variable_data.at(var_index).auto_manual;
//                }
//                else  if (json.get("object_type", Json::nullValue).asInt() == READ_SCHEDULE)
//                {
//                    tempjson["object_name"] = (char*)m_Weekly_data.at(var_index).description;
//                    tempjson["object_label"] = (char*)m_Weekly_data.at(var_index).label;
//                    tempjson["auto_manual"] = m_Weekly_data.at(var_index).auto_manual;
//                }
//                else  if (json.get("object_type", Json::nullValue).asInt() == READ_CALENDER)
//                {
//                    tempjson["object_name"] = (char*)m_Annual_data.at(var_index).description;
//                    tempjson["object_label"] = (char*)m_Annual_data.at(var_index).label;
//                    tempjson["auto_manual"] = m_Annual_data.at(var_index).auto_manual;
//                }
//                else  if (json.get("object_type", Json::nullValue).asInt() == READ_PROGRAM)
//                {
//                    tempjson["object_name"] = (char*)m_Program_data.at(var_index).description;
//                    tempjson["object_label"] = (char*)m_Program_data.at(var_index).label;
//                    tempjson["auto_manual"] = m_Program_data.at(var_index).auto_manual;
//                    tempjson["status"] = m_Program_data.at(var_index).on_off;
//                }
//                Json::StyledWriter styleWriter;
//                Json::String writeStr = styleWriter.write(tempjson);
//                CString Response = writeStr.c_str();
//                app->m_webView->ExecuteScript(L"UpdateElementsDataFromDevice(" + Response + ")", Callback<ICoreWebView2ExecuteScriptCompletedHandler>(app, &BacnetWebViewAppWindow::ExecuteScriptResponse).Get());
//                    
//            }
//             break;
//         }
//         case WEBVIEW_MESSAGE_TYPE::SAVE_GRAPHIC:
//         {
//             if (auto app = (BacnetWebViewAppWindow*)GetWindowLongPtr(m_mainWindow, GWLP_USERDATA))
//             {
//                 CString filename = json.get("graphic_name", Json::nullValue).asCString();
//                 Json::String tmpString = json.get("data", Json::nullValue).asString();
//                 CString temp_now_building_name = g_strCurBuildingDatabasefilePath;
//                 PathRemoveFileSpec(temp_now_building_name.GetBuffer(MAX_PATH));
//                 temp_now_building_name.ReleaseBuffer();
//                 CFile file;
//                 CString tempfile;
//                 if (CreateDirectory(temp_now_building_name + _T("\\Graphic"), NULL) ||
//                     ERROR_ALREADY_EXISTS == GetLastError())
//                 {
//
//                     tempfile = temp_now_building_name + _T("\\Graphic\\" + filename + ".grp");
//                     if (file.Open(tempfile, CFile::modeCreate | CFile::modeWrite, NULL))
//                     {
//                         file.Write(tmpString.c_str(), tmpString.size());
//                         file.Close();
//                     }  // CopyFile(...)
//                 }
//                 else {
//                     CString error = "Not Able to Create Directory at:  " + temp_now_building_name + _T("\\Graphic");
//                     app->m_webView->ExecuteScript(L"DisplayError( " + error + ")", Callback<ICoreWebView2ExecuteScriptCompletedHandler>(app, &BacnetWebViewAppWindow::ExecuteScriptResponse).Get());
//                 }
//             }
//             break;
//         }
//         case WEBVIEW_MESSAGE_TYPE::LIST_GRAPHICS:
//         {
//             if (auto app = (BacnetWebViewAppWindow*)GetWindowLongPtr(m_mainWindow, GWLP_USERDATA))
//             {
//                 CString temp_now_building_name = g_strCurBuildingDatabasefilePath;
//                 PathRemoveFileSpec(temp_now_building_name.GetBuffer(MAX_PATH));
//                 temp_now_building_name.ReleaseBuffer();
//                 CString path = temp_now_building_name + _T("\\Graphic");
//                 //const TCHAR* x = path.GetBuffer();
//                 vector<string> files = GetGraphicFiles(path);
//
//                 Json::Reader reader;
//                 Json::Value tempjson;
//                 reader.parse(app->JsonData, tempjson, false);
//                 tempjson["action"] = WEBVIEW_MESSAGE_TYPE::LIST_GRAPHICS;
//
//                 for (int i = 0; i < files.size(); i++)
//                 {
//                    char* token = strtok(&files.at(i)[0], ".");
//                    if (token) {
//                        tempjson["files"][i] = token;
//                    }
//                 }
//                 Json::StyledWriter styleWriter;
//                 Json::String writeStr = styleWriter.write(tempjson);
//                 CString Response = writeStr.c_str();
//                 app->m_webView->ExecuteScript(L"GraphicFilesList(" + Response + ")", Callback<ICoreWebView2ExecuteScriptCompletedHandler>(app, &BacnetWebViewAppWindow::ExecuteScriptResponse).Get());
//             }
//             break;
//         }
//         case WEBVIEW_MESSAGE_TYPE::LOAD_GRAPHICS:
//         {
//             if (auto app = (BacnetWebViewAppWindow*)GetWindowLongPtr(m_mainWindow, GWLP_USERDATA))
//             {
//
//                 CString filename = json.get("graphic_name", Json::nullValue).asCString();
//                 CString temp_now_building_name = g_strCurBuildingDatabasefilePath;
//                 PathRemoveFileSpec(temp_now_building_name.GetBuffer(MAX_PATH));
//                 temp_now_building_name.ReleaseBuffer();
//                 CString path = temp_now_building_name + _T("\\Graphic\\"+ filename+".grp");
//                 
//
//                 Json::Reader reader;
//                 Json::Value tempjson;
//                 reader.parse(app->JsonData, tempjson, false);
//                 tempjson["action"] = WEBVIEW_MESSAGE_TYPE::LIST_GRAPHICS;
//                 CFile file;
//                 CString tmpString;
//                 if (file.Open(path, CFile::modeRead, NULL))
//                 {
//                     DWORD len = file.GetLength();
//                     char* Buf = new char[len + 1];
//                     Buf[len + 1] = 0;
//                     file.Read(Buf, len);
//                     file.Close();
//                   
//                     MultiByteToWideChar(CP_ACP, 0, (char*)Buf, (int)strlen((char*)Buf) + 1, tmpString.GetBuffer(len), len);
//                     tmpString.ReleaseBuffer();
//                     tempjson["data"] = Buf;
//                 }
//                 Json::StyledWriter styleWriter;
//                 Json::String writeStr = styleWriter.write(tempjson);
//                 CString Response = writeStr.c_str();
//                 app->m_webView->ExecuteScript(L"LoadGraphicFile(" + tmpString + ")", Callback<ICoreWebView2ExecuteScriptCompletedHandler>(app, &BacnetWebViewAppWindow::ExecuteScriptResponse).Get());
//             }
//             break;
//         }
//         case WEBVIEW_MESSAGE_TYPE::TRIGGER_T3000_DLG:
//         {
//             if (auto app = (BacnetWebViewAppWindow*)GetWindowLongPtr(m_mainWindow, GWLP_USERDATA))
//             {
//                 if (json.get("object_type", Json::nullValue).asInt() == 3)// Schedule
//                 {
//                     int index = json.get("index", Json::nullValue).asInt();
//                     weekly_list_line = index;
//                     if ((n_read_item_index >= 0) && (n_read_item_index < BAC_SCHEDULE_COUNT))
//                     {
//                         n_read_item_index = weekly_list_line;
//                         ::PostMessage(BacNet_hwd, WM_RS485_MESSAGE, product_type, READTIMESCHEDULE_T3000/*BAC_PRG*/);//
//                     }
//
//                 }
//                 else if (json.get("object_type", Json::nullValue).asInt() == 4)// Calender
//                 {
//                     int index = json.get("index", Json::nullValue).asInt();
//                     annual_list_line = index;
//                     ::PostMessage(BacNet_hwd, WM_FRESH_CM_LIST, MENU_CLICK, TYPE_ANNUALCODE);
//                 }
//                 else if (json.get("object_type", Json::nullValue).asInt() == 5)// Program
//                 {
//                     int index = json.get("index", Json::nullValue).asInt();
//                     program_list_line = index;
//                      Program_Window->OnBnClickedButtonProgramEdit();
//                 }
//             }
//             break;
//         }
//         case WEBVIEW_MESSAGE_TYPE::UPDATE_DATA:
//         {
//             if (auto app = (BacnetWebViewAppWindow*)GetWindowLongPtr(m_mainWindow, GWLP_USERDATA))
//             {
//
//                 int var_index = json.get("index", Json::nullValue).asInt();
//                 if (json.get("object_type", Json::nullValue).asInt() == 0)// input
//                 {
//                     m_Input_data.at(var_index).value = json.get("value", Json::nullValue).asFloat()*1000;
//                     m_Input_data.at(var_index).auto_manual = json.get("auto_manual", Json::nullValue).asInt();
//
//                    // ::PostMessage(m_input_dlg_hwnd, WM_LIST_ITEM_CHANGED, var_index, INPUT_AUTO_MANUAL);
//                    // ::PostMessage(m_input_dlg_hwnd, WM_LIST_ITEM_CHANGED, var_index, INPUT_VALUE);
//
//                     //::PostMessage(m_input_dlg_hwnd, WM_REFRESH_BAC_INPUT_LIST, var_index, REFRESH_ON_ITEM);
//
//                      Write_Private_Data_Blocking(WRITEINPUT_T3000, var_index, var_index, g_bac_instance);
//                      ::PostMessage(m_input_dlg_hwnd, WM_REFRESH_BAC_INPUT_LIST, var_index, REFRESH_ON_ITEM);
//                 }
//                 else if (json.get("object_type", Json::nullValue).asInt() == 1)// output
//                 {
//                     m_Output_data.at(var_index).value = json.get("value", Json::nullValue).asFloat() * 1000;
//                     m_Output_data.at(var_index).auto_manual = json.get("auto_manual", Json::nullValue).asInt();
//
//                     /*::PostMessage(m_input_dlg_hwnd, WM_LIST_ITEM_CHANGED, var_index, INPUT_AUTO_MANUAL);
//                     ::PostMessage(m_input_dlg_hwnd, WM_LIST_ITEM_CHANGED, var_index, OUTPUT_VALUE);
//
//                     ::PostMessage(m_output_dlg_hwnd, WM_REFRESH_BAC_OUTPUT_LIST, var_index, REFRESH_ON_ITEM);*/
//
//                     Write_Private_Data_Blocking(WRITEOUTPUT_T3000, var_index, var_index, g_bac_instance);
//
//                     ::PostMessage(m_output_dlg_hwnd, WM_REFRESH_BAC_OUTPUT_LIST, var_index, REFRESH_ON_ITEM);
//
//                 }
//                 else if (json.get("object_type", Json::nullValue).asInt() == 2)// variables
//                 {
//                     m_Variable_data.at(var_index).value = json.get("value", Json::nullValue).asFloat() * 1000;
//                     m_Variable_data.at(var_index).auto_manual = json.get("auto_manual", Json::nullValue).asInt();
//                    // ::PostMessage(m_variable_dlg_hwnd, WM_REFRESH_BAC_VARIABLE_LIST, var_index, REFRESH_ON_ITEM);
//
//                     Write_Private_Data_Blocking(WRITEVARIABLE_T3000, var_index, var_index, g_bac_instance);
//                     ::PostMessage(m_variable_dlg_hwnd, WM_LIST_ITEM_CHANGED, var_index, VARIABLE_AUTO_MANUAL);
//
//                 }
//                 else  if (json.get("object_type", Json::nullValue).asInt() == READ_SCHEDULE)
//                 {
//                     m_Weekly_data.at(var_index).auto_manual = json.get("auto_manual", Json::nullValue).asFloat();
//
//                     Write_Private_Data_Blocking(WRITESCHEDULE_T3000, var_index, var_index, g_bac_instance);
//
//                     ::PostMessage(m_weekly_dlg_hwnd, WM_REFRESH_BAC_WEEKLY_LIST, var_index, REFRESH_ON_ITEM);
//                    // ::PostMessage(m_weekly_dlg_hwnd, WM_LIST_ITEM_CHANGED, var_index, WEEKLY_ROUTINE_AUTO_MANUAL);
//                 }
//                 else  if (json.get("object_type", Json::nullValue).asInt() == READ_CALENDER)
//                 {
//                     m_Annual_data.at(var_index).auto_manual = json.get("auto_manual", Json::nullValue).asFloat();
//
//                     Write_Private_Data_Blocking(WRITEHOLIDAY_T3000, var_index, var_index, g_bac_instance);
//                     ::PostMessage(m_annual_dlg_hwnd, WM_REFRESH_BAC_ANNUAL_LIST, var_index, REFRESH_ON_ITEM);
//                     //::PostMessage(m_annual_dlg_hwnd, WM_LIST_ITEM_CHANGED, var_index, ANNUAL_ROUTINE_AUTO_MANUAL);
//                 }
//                 else  if (json.get("object_type", Json::nullValue).asInt() == READ_PROGRAM)
//                 {
//                     m_Program_data.at(var_index).auto_manual = json.get("auto_manual", Json::nullValue).asFloat();
//                     m_Program_data.at(var_index).on_off = json.get("status", Json::nullValue).asInt();
//
//                     //::PostMessage(m_pragram_dlg_hwnd, WM_LIST_ITEM_CHANGED, var_index, PROGRAM_AUTO_MANUAL);
//
//                     Write_Private_Data_Blocking(WRITEPROGRAM_T3000, var_index, var_index, g_bac_instance);
//                     ::PostMessage(m_pragram_dlg_hwnd, WM_REFRESH_BAC_PROGRAM_LIST, var_index, REFRESH_ON_ITEM);
//                 }
//             }
//             break;
//         }
//         case WEBVIEW_MESSAGE_TYPE::ADD_VAR:
//         {
//             if (auto app = (BacnetWebViewAppWindow*)GetWindowLongPtr(m_mainWindow, GWLP_USERDATA))
//             {
//                 
//                 CString value = json.get("Value", Json::nullValue).asCString();
//                 float X_Pos = json.get("xpos", Json::nullValue).asFloat();
//                 float Y_Pos = json.get("ypos", Json::nullValue).asInt();
//                 POINT Insert_Point;
//                 Insert_Point.x = X_Pos;
//                 Insert_Point.y = Y_Pos;
//
//                 char* temp_char = new char[200];
//
//                 value.MakeUpper();// label only support upper case;
//                 WideCharToMultiByte(CP_ACP, NULL, value.GetBuffer(), -1, temp_char, 200, NULL, NULL);
//                // ::PostMessage(m_screenedit_dlg_hwnd, ADD_LABEL_MESSAGE, (WPARAM)temp_char, (LPARAM)(&Insert_Point));
//                 int temp_number = -1;
//                 byte temp_value_type = -1;
//                 byte temp_point_type = -1;
//                 int temp_panel = -1;
//                 int temp_net = -1;
//                 int k = 0;
//                 unsigned char sub_panel = 0;
//                 char* tempcs = NULL;
//                 //int temp1;
//                 //tempcs = ispoint(temp_point,&temp_number,&temp_value_type,&temp_point_type,&temp_panel,&temp_net,0,Station_NUM,&k);
//                /* tempcs = ispoint_ex(temp_char, &temp_number, &temp_value_type, &temp_point_type, &temp_panel, &temp_net, 0, sub_panel, Station_NUM, &k);
//                 if (tempcs == NULL)
//                 {
//                     AfxMessageBox(L"Invalid Label , Please input a label such as 'IN8' , 'Mainpanel-Subpanel-OUT9', 'Mainpanel:Subpanel:OUT9' ,'VAR100'.\r\nFor more information , please reference the user manual.");
//                    
//                 }*/
//
//
//                 if (temp_number != 0)	//
//                     temp_number = temp_number - 1;
//             }
//             break;
//         case WEBVIEW_MESSAGE_TYPE::LOAD_BG_IMG:
//         {
//             if (auto app = (BacnetWebViewAppWindow*)GetWindowLongPtr(m_mainWindow, GWLP_USERDATA))
//             {
//                 const TCHAR szFilter[] = _T("Image files (*.jpg, *.png) | *.jpg; *.png");
//                 CFileDialog dlg(1, _T("image"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
//                     szFilter, CWnd::FromHandle(m_mainWindow));
//                 CString destination;
//                 if (dlg.DoModal() == IDOK)
//                 {
//                     CString sFilePath = dlg.GetPathName();
//                     CString temp_now_building_name = g_strCurBuildingDatabasefilePath;
//                     PathRemoveFileSpec(temp_now_building_name.GetBuffer(MAX_PATH));
//                     temp_now_building_name.ReleaseBuffer();
//                     if (CreateDirectory(temp_now_building_name + _T("\\image\\"), NULL) ||
//                         ERROR_ALREADY_EXISTS == GetLastError())
//                     {
//                         destination = temp_now_building_name + _T("\\image\\" + dlg.GetFileName());
//                         CopyFile(sFilePath, destination, 1);                        
//                     }
//                     else {
//                         CString error = "Not Able to Create Directory at:  " + temp_now_building_name + _T("\\image");
//                         app->m_webView->ExecuteScript(L"DisplayError( " + error + ")", Callback<ICoreWebView2ExecuteScriptCompletedHandler>(app, &BacnetWebViewAppWindow::ExecuteScriptResponse).Get());
//                         return;
//                     }
//                   
//                 }
//                 if(destination!=""){
//                     unsigned int width;
//                     unsigned int height;
//
//                     get_png_image_dimensions(destination, width, height);
//                     Json::Reader reader;
//                     Json::Value tempjson;
//                     reader.parse(app->JsonData, tempjson, false);
//                     char cTemp1[255];
//                     memset(cTemp1, 0, 255);
//                     WideCharToMultiByte(CP_ACP, 0, destination.GetBuffer(), -1, cTemp1, destination.GetLength(), NULL, NULL);
//
//                     tempjson["file"] = cTemp1;
//                         tempjson["width"] = width;
//                         tempjson["height"] = height;
//                     Json::StyledWriter styleWriter;
//                     Json::String writeStr = styleWriter.write(tempjson);
//                     CString Response = writeStr.c_str();
//                     Sleep(100);
//                    app->m_webView->ExecuteScript(L"UpdateBackgroundImage(" + Response + ")", Callback<ICoreWebView2ExecuteScriptCompletedHandler>(app, &BacnetWebViewAppWindow::ExecuteScriptResponse).Get());
//                 }
//             }
//             break;
//         }
//         }
//        //case 2:// Update
//        //{
//
//        //    Str_in_point m_temp_Input_data;
//        //    CString type = json1.get("type", Json::nullValue).asCString();
//        //    CString deviceId = json1.get("device_id", Json::nullValue).asCString();
//        //    Json::Value variable = json1.get("variable", Json::nullValue);
//        //    CString varName = variable.get("name", Json::nullValue).asCString();
//        //    int  varIndex = variable.get("index", Json::nullValue).asInt();
//        //    int  varValue = variable.get("value", Json::nullValue).asInt();
//        //    int index =  Find_Index_Input_By_Label(varName);
//        //    //json1["variable"]["name"] = "IN-DOOR";
//        //    memcpy_s(&m_temp_Input_data, sizeof(Str_in_point), &m_Input_data.at(index), sizeof(Str_in_point));
//
//        //    if (varValue * 1000 != m_Input_data.at(index).value)
//        //        m_Input_data.at(index).value = varValue * 1000;
//
//        //   int change = memcmp(&m_temp_Input_data, &m_Input_data.at(index), sizeof(Str_in_point));
//        //    if (change != 0)
//        //    {
//        //        Post_Write_Message(g_bac_instance, WRITEINPUT_T3000, index, index, sizeof(Str_in_point), m_input_dlg_hwnd, "", index, 4);
//        //        Sleep(50);
//        //        //PostMessage(WM_REFRESH_BAC_INPUT_LIST, index, REFRESH_ON_ITEM);
//        //        ::PostMessage(m_input_dlg_hwnd, WM_REFRESH_BAC_INPUT_LIST, index, REFRESH_ON_ITEM);
//        //    }
//
//        //    /*json1["variable"]["value"] = m_Input_data.at(index).value / 1000;
//        //    Json::StyledWriter styleWriter;
//        //    Json::String writeStr = styleWriter.write(json1);
//        //    CString Response = writeStr.c_str();
//        //    m_webView->ExecuteScript(L"MessageReceived("+ Response +")", Callback<ICoreWebView2ExecuteScriptCompletedHandler>(this, &BacnetWebViewAppWindow::ExecuteScriptResponse).Get());*/
//
//        //    /*index = Find_Index_Input_By_Label("ODT");
//        //    json1["variable"]["name"] = "OUT-DOOR";
//        //    json1["variable"]["value"] = m_Input_data.at(index).value / 1000;
//        //    writeStr = styleWriter.write(json1);
//        //    Response = writeStr.c_str();
//        //    m_webView->ExecuteScript(L"MessageReceived(" + Response + ")", Callback<ICoreWebView2ExecuteScriptCompletedHandler>(this, &BacnetWebViewAppWindow::ExecuteScriptResponse).Get());*/
//        //    break;
//        //}
//       
//    }
//    
//}
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