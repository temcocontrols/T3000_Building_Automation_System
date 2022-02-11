#include "stdafx.h"
#include "BacnetWebViewToolBar.h"
#include "BacnetWebView.h"

#define IDE_ADDRESSBAR                  1000
#define IDE_ADDRESSBAR_GO               1001
#define IDE_BACK                        1002
#define IDE_FORWARD                     1003
#define IDE_ADDRESSBAR_RELOAD           1004
#define IDE_CANCEL                      1005
#define IDC_EDIT_INPUT                  1006
#define IDC_STATIC_LABEL                1007
#define IDC_EDIT_DESCRIPTION            1008

BacnetWebViewToolbar::BacnetWebViewToolbar() : m_items{ Item_LAST } 
{}

BacnetWebViewToolbar::~BacnetWebViewToolbar()
{
    if (m_font)
    {
        DeleteObject(m_font);
        m_font = nullptr;
    }
}

void BacnetWebViewToolbar::Initialize(BacnetWebViewAppWindow* appWindow)
{
    m_appWindow = appWindow;
    HWND mainWindow = m_appWindow->GetMainWindow();

    m_items[Item_BackButton] = CreateWindow(
        L"button", L"Back", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, 0, 0, 0, 0,
        mainWindow, (HMENU)IDE_BACK, nullptr, 0);
    m_items[Item_ForwardButton] = CreateWindow(
        L"button", L"Forward", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, 0, 0, 0, 0,
        mainWindow, (HMENU)IDE_FORWARD, nullptr, 0);
    m_items[Item_ReloadButton] = CreateWindow(
        L"button", L"Reload", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, 0, 0, 0, 0,
        mainWindow, (HMENU)IDE_ADDRESSBAR_RELOAD, nullptr, 0);
    m_items[Item_CancelButton] = CreateWindow(
        L"button", L"Cancel", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, 0, 0, 0, 0,
        mainWindow, (HMENU)IDE_CANCEL, nullptr, 0);
    m_items[Item_AddressBar] = CreateWindow(
        L"edit", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, 0, 0, 0, 0,
        mainWindow, (HMENU)IDE_ADDRESSBAR, nullptr, 0);
    m_items[Item_GoButton] = CreateWindow(
        L"button", L"Go", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | BS_DEFPUSHBUTTON,
        0, 0, 0, 0, mainWindow, (HMENU)IDE_ADDRESSBAR_GO, nullptr, 0);

    RECT availableBounds = { 0 };
    GetClientRect(mainWindow, &availableBounds);
    DisableAllItems();
}

void BacnetWebViewToolbar::SetItemEnabled(Item item, bool enabled)
{
    EnableWindow(m_items[item], enabled);
}

void BacnetWebViewToolbar::DisableAllItems()
{
    for (HWND hwnd : m_items)
    {
        EnableWindow(hwnd, FALSE);
    }
}

HWND BacnetWebViewToolbar::GetItem(Item item) const
{
    return m_items[item];
}

const std::vector<HWND>& BacnetWebViewToolbar::GetItems() const
{
    return m_items;
}