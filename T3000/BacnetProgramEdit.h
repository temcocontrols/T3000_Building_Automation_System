#pragma once
#include "afxwin.h"
#include "CM5\CStatic\staticex.h"
#include <vector> // 新增：用于存储带类型的关键词

// 自动联想功能常量定义
#define MAX_AUTOCOMPLETE_COUNT  20     // 最大显示候选词数量
const int KEY_F2 = 1002;
const int KEY_F3 = 1003;
const int KEY_F7 = 1007;
const int KEY_F6 = 1006;
const int KEY_F8 = 1008;
const int KEY_F9 = 1009;

// 新增：关键词类型枚举（与语法高亮颜色一一对应）
enum KeywordType
{
    LIST_COLOR_TYPE_CUSTOM = 0,    // 自定义关键字（默认黑色）
    LIST_COLOR_TYPE_FUNC,          // 函数（func_table，蓝色）
    LIST_COLOR_TYPE_COMMAND,       // 命令（table，绿色）
    LIST_COLOR_TYPE_INPUT,         // 输入点（m_Input_data，紫色）
    LIST_COLOR_TYPE_OUTPUT,        // 输出点（m_Output_data，橙色）
    LIST_COLOR_TYPE_VARIABLE       // 变量（m_Variable_data，棕色）
};

// 新增：带类型的关键词结构体
struct AutoCompleteKeyword
{
    CString text;
    KeywordType type;
};

// CBacnetProgramEdit dialog
class CBacnetProgramEdit : public CDialogEx
{
    DECLARE_DYNAMIC(CBacnetProgramEdit)
public:
    CBacnetProgramEdit(CWnd* pParent = NULL);   // standard constructor
    virtual ~CBacnetProgramEdit();

    // Dialog Data
    enum { IDD = IDD_DIALOG_BACNET_PROGRAM_EDIT };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    afx_msg LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);//手动加入.
    DECLARE_MESSAGE_MAP()
    bool Run_once_mutex;
public:
    afx_msg void OnDestroy();
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    LRESULT  ProgramResumeMessageCallBack(WPARAM wParam, LPARAM lParam);
    LRESULT  Fresh_Program_RichEdit(WPARAM wParam, LPARAM lParam);
    afx_msg void OnSend();
    afx_msg void OnClose();
    afx_msg void OnClear();
    afx_msg void OnLoadfile();
    afx_msg void OnSavefile();
    virtual void OnCancel();
    afx_msg void OnEnSetfocusRichedit2Program();
    void Initial_static();
    CListBox m_information_window;
    CStaticEx m_pool_size;
    CStaticEx m_program_size;
    CStaticEx m_free_memory;
    afx_msg void OnRefresh();
    bool m_edit_changed;
    afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
    void SetRicheditFont(long nStartchar, long nEndchar, DWORD nColor);
    afx_msg void OnProgramIdeSettings();
    int Bacnet_Show_Debug(CString& retselstring);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    void UpdateDataProgramText();
    void GetColor();
    void SetBackFont();
    void Syntax_analysis();
    void Delete_Repeat_Char_Vec();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    //	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnPropertiesGotodefinition();
    afx_msg void OnBnClickedButtonProgramEditHelp();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnRenumber();

public:
    // ========== 自动联想功能核心函数 ==========
    void InitAutoCompleteKeywords();       // 初始化联想关键词库
    void ShowAutoComplete(CPoint point);   // 显示联想面板
    void HideAutoComplete();                // 隐藏联想面板
    void MatchAutoCompleteKeywords(CString strInput); // 匹配候选词
    void OnAutoCompleteSelect();            // 选中候选词并补全文本

protected:
    // ========== 自动联想相关控件与变量 ==========
    CListBox m_autoCompleteList;    // 联想候选列表框（自绘）
    CFont m_autoCompleteFont;       // 联想列表字体（与RichEdit保持一致）
    std::vector<AutoCompleteKeyword> m_autoCompleteKeywords; // 替换原CStringArray，存储带类型的关键词
    bool m_bAutoCompleteVisible;    // 联想面板是否显示
    CString m_strCurrentInput;      // 当前输入的待匹配文本
    int m_nInputStartPos;           // 输入文本的起始位置
    bool m_bInAutoCompleteLogic;    // 防重入标志，避免EN_CHANGE递归触发

    // ========== 新增：自绘列表框消息处理函数 ==========
    afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
    afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);

    // ========== 新增消息处理函数声明 ==========
    afx_msg void OnEnChangeRichedit2Program(); // RichEdit内容变化事件
    afx_msg void OnEnSelchangeRichedit2Program(NMHDR* pNMHDR, LRESULT* pResult); // 修复：RichEdit光标变化事件
    afx_msg void OnLbnDblclkAutocompleteList(); // 联想列表双击事件
    afx_msg void OnLbnSelchangeAutocompleteList(); // 联想列表选中变化事件
};