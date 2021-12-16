#include <afxwin.h>
#include "BacnetWebView.h"
#include "resource.h"
#include <Shellapi.h>
#include <ShlObj_core.h>
#include <functional>
#include <stdexcept>
#include <regex>
#include <gdiplus.h>

#include <wil/com.h>
#include <wrl.h>

using namespace Microsoft::WRL;
size_t thread_local BacnetWebViewAppWindow::s_appInstances = 0;

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
    std::wstring title = L"BacnetWebView Demo Application 0.2";
    m_mainWindow = CreateWindowExW(WS_EX_CONTROLPARENT, GetWindowClass(), title.c_str(), 
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, 
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
}

PCWSTR BacnetWebViewAppWindow::GetWindowClass()
{
    // Only do this once
    static PCWSTR windowClass = [] () {
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
            ResizeEverything();
            return true;
        }
    }
    break;
    case WM_PAINT:
    {
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

	/*wil::com_ptr<ICoreWebView2CompositionController> compositionController =
		m_controller.query<ICoreWebView2CompositionController>();*/
    m_dropTarget = Make<DropTarget>();
    m_dropTarget->Init(m_mainWindow, coreWebView2.get(), m_controller.get());
    
    if (m_webView != nullptr && m_initialUri != L"")
        m_webView->Navigate(m_initialUri.c_str());
}

#pragma endregion WebviewRelatedMethods
