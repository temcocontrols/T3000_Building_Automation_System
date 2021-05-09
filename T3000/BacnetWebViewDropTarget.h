#pragma once
#include <wil/wrl.h>
#include <wil/com.h>
#include <WebView2.h>
#include <ShObjIdl_core.h>
#include <ShlGuid.h>

class DropTarget : public Microsoft::WRL::RuntimeClass<
    Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>,
    IDropTarget>
{

public:
    DropTarget() {};
    virtual ~DropTarget() {};

    // Initialize the drop target by associating it with the given HWND.
    void Init(HWND window, ICoreWebView2* viewComponent,
        ICoreWebView2Controller* webViewCompositionController)
    {
        m_window = window;
        m_viewComponent = viewComponent;
        m_webViewCompositionController = webViewCompositionController;
    }


    // IDropTarget implementation:
    HRESULT __stdcall DragEnter(IDataObject* dataObject,
        DWORD keyState,
        POINTL cursorPosition,
        DWORD* effect) override {
        return S_OK;
    }
    HRESULT __stdcall DragOver(DWORD keyState,
        POINTL cursor_position,
        DWORD* effect) override {
        return S_OK;
    }
    HRESULT __stdcall DragLeave() override { 
        return S_OK; 
    }
    HRESULT __stdcall Drop(IDataObject* dataObject,
        DWORD keyState,
        POINTL cursorPosition,
        DWORD* effect) override
    {
        POINT point = { cursorPosition.x, cursorPosition.y };
        wil::com_ptr<IDropTargetHelper> dropHelper = DropHelper();
        if (dropHelper) 
        {
            return dropHelper->Drop(dataObject, &point, *effect);
        }

        FORMATETC format;
        STGMEDIUM medium;
        dataObject->GetData(&format, &medium);
        
        return S_FALSE;
        //return m_webViewCompositionController->Drop(dataObject, keyState, point, effect);
    }

private:
    ICoreWebView2* m_viewComponent = nullptr;

    // Returns the hosting HWND.
    HWND GetHWND() { return m_window; }

    wil::com_ptr<IDropTargetHelper> DropHelper()
    {
        if (!m_dropTargetHelper)
        {
            CoCreateInstance(
                CLSID_DragDropHelper, 0, CLSCTX_INPROC_SERVER, IID_IDropTargetHelper,
                reinterpret_cast<void**>(&m_dropTargetHelper));
        }
        return m_dropTargetHelper;
    }

    wil::com_ptr<IDropTargetHelper> m_dropTargetHelper;

    // The HWND of the source. This HWND is used to determine coordinates for
    // mouse events that are sent to the renderer notifying various drag states.
    HWND m_window;

    wil::com_ptr<ICoreWebView2Controller> m_webViewCompositionController;
};
