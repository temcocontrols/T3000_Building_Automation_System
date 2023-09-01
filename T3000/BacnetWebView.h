#ifndef _BACNET_WEBVIEW_HEADER
#define _BACNET_WEBVIEW_HEADER
#include "targetver.h"
#include <Windows.h>
#include <string>
#include <functional>
#include <wrl.h>
#include <wil/com.h>
#include "webview2.h"
#include "BacnetWebViewToolBar.h"
#include "BacnetWebViewDropTarget.h"
#include <vector>

#pragma region defines
#define IDM_GET_BROWSER_VERSION_AFTER_CREATION 170
#define IDM_CLOSE_WEBVIEW               116
#define IDM_CLOSE_WEBVIEW_CLEANUP       175
#define IDM_SCENARIO_POST_WEB_MESSAGE   2001
#define IDM_SCENARIO_ADD_HOST_OBJECT    2003
#define IDM_SCENARIO_WEB_VIEW_EVENT_MONITOR 2002
#define IDM_SCENARIO_JAVA_SCRIPT   199
#define IDM_SCENARIO_TYPE_SCRIPT   200
#define IDM_SCENARIO_AUTHENTICATION     2000
#define IDM_SCENARIO_COOKIE_MANAGEMENT  1999
#define IDM_SCENARIO_DOM_CONTENT_LOADED 2004
#define IDM_SCENARIO_NAVIGATEWITHWEBRESOURCEREQUEST 2005
#define IDM_GET_BROWSER_VERSION_BEFORE_CREATION 171
#define IDM_EXIT                        105
#define IDM_CREATION_MODE_WINDOWED 193
#pragma endregion

class BacnetWebViewAppWindow
{
public:
    CString des_file;
    CString des_file_zip;
    CString des_lib_file;
public:
    BacnetWebViewAppWindow(
        UINT creationModeId,
        std::wstring initialUri = L"",
        bool isMainWindow = false,
        std::function<void()> webviewCreatedCallback = nullptr,
        bool customWindowRect = false,
        RECT windowRect = { 0 },
        bool shouldHaveToolbar = true);

#pragma region Getters_Controller_Window
    ICoreWebView2Controller* GetWebViewController()
    {
        return m_controller.get();
    }
    ICoreWebView2* GetWebView()
    {
        return m_webView.get();
    }
    ICoreWebView2Environment* GetWebViewEnvironment()
    {
        return m_webViewEnvironment.get();
    }
    HWND GetMainWindow()
    {
        return m_mainWindow;
    }
#pragma endregion 

#pragma region com_apis
    void AddRef();
    void Release();
    void NotifyClosed();
#pragma endregion

#pragma region API
    void SetTitleText(PCWSTR titleText);
    RECT GetWindowBounds();
    std::wstring GetLocalUri(std::wstring path);
    void ReinitializeWebView();
    void RunAsync(std::function<void(void)> callback);
#pragma endregion

#pragma region static_public_helpers
    static int RunMessagePump();
#pragma endregion

private:
    std::string JsonData = R"~(
        {
					'action':2,
					'type':'input',
					'device_id':'3000',
                    'variable' : {
						'name':"abc",
						'index':0,
                        'type' : 1,
                        'value' : 10
                    },
                    'target_variable' : {
						'name' : "abc",
						'index':0,
						'type' : 1,
						'value' : 'wwe'
                    }
                }
        )~";
    void InitialWebPoint();
    static PCWSTR GetWindowClass();    
    static LRESULT CALLBACK WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    bool HandleWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* result);
    bool ExecuteWebViewCommands(WPARAM wParam, LPARAM lParam);
    bool ExecuteAppCommands(WPARAM wParam, LPARAM lParam);
    void ResizeEverything();
    void InitializeWebView();
    HRESULT OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment);
    HRESULT OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2Controller* controller);

    HRESULT WebMessageReceived(ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args);
    HRESULT ExecuteScriptResponse(HRESULT errorCode, LPCWSTR result);
    void ProcessWebviewMsg(CString msg); 
    static VOID CALLBACK onTimer(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
    void RegisterEventHandlers();
    void ReinitializeWebViewWithNewBrowser();
    void RestartApp();
    void CloseWebView(bool cleanupUserDataFolder = false);
    void CloseAppWindow();
    void ChangeLanguage();
    void UpdateCreationModeMenu();

    std::vector<std::string> GetGraphicFiles(CString path);
    void get_png_image_dimensions(CString& file_path, unsigned int& width, unsigned int& height);
    std::wstring GetLocalPath(std::wstring path, bool keep_exe_path);

    std::wstring m_initialUri;
    HWND m_mainWindow = nullptr;
    BacnetWebViewToolbar m_toolbar;
    std::function<void()> m_onWebViewFirstInitialized;
    DWORD m_creationModeId = 0;
    int m_refCount = 1;
    bool m_isClosed = false;
    ULONG_PTR m_gdiplusToken;
    static thread_local size_t s_appInstances;

    wil::com_ptr<ICoreWebView2Environment> m_webViewEnvironment;
    wil::com_ptr<ICoreWebView2Controller> m_controller;
    wil::com_ptr<ICoreWebView2> m_webView;
    wil::com_ptr<ICoreWebView2_3> m_webView3;
    wil::com_ptr<DropTarget> m_dropTarget;

    HBITMAP m_appBackgroundImageHandle;
    BITMAP m_appBackgroundImage;
    HDC m_memHdc;
    public:
        int JsonDataToStruct(std::string file_output);

};


#endif
