#ifndef _BACNET_VEBVIEW_TOOLBAR_HEADER
#define _BACNET_VEBVIEW_TOOLBAR_HEADER
#include <vector>
#include <Windows.h>

class BacnetWebViewAppWindow;

class BacnetWebViewToolbar
{
public:
    enum Item
    {
        Item_BackButton,
        Item_ForwardButton,
        Item_ReloadButton,
        Item_CancelButton,
        Item_AddressBar,
        Item_GoButton,
        Item_LAST,
    };

    BacnetWebViewToolbar();
    ~BacnetWebViewToolbar();
    void Initialize(BacnetWebViewAppWindow* appWindow);
    void SetItemEnabled(Item item, bool enabled);
    void DisableAllItems();
    HWND GetItem(Item item) const;
    const std::vector<HWND>& GetItems() const;

private:
    BacnetWebViewAppWindow* m_appWindow = nullptr;
    HFONT m_font = nullptr;

    std::vector<HWND> m_items;
};
#endif
