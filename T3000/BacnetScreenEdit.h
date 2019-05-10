#pragma once
#include "RelayLabel.h"
#include "GraphicView.h"
#include "../SQLiteDriver/CppSQLite3.h"

class CBacnetScreenEdit : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetScreenEdit)

public:
	CBacnetScreenEdit(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetScreenEdit();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_SCREENS_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	int m_nFoucsIndext;
	BOOL m_LbtDown;
public:
	int m_bac_select_label;
	bool m_bac_lbuttondown;
	bool m_bac_hotkey_lb_down;
	afx_msg void OnBnClickedButtonScreenEditTest();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	bool UpdateDeviceLabelFlash();

	void InitGraphic(int nSerialNum,int nInstanceID,unsigned char screen_number);
	//void InitGraphic(int nSerialNum,int nInstanceID);
	//void ClearAllLabels();
	void ReloadLabelsFromDB();
	void PainNoImageInfo(CDC* pDC);
	void Bacnet_Add_Label();
	void Bacnet_Edit_Label(bool allow_change = true);
	int JudgeClickItem(CPoint & point);
	//void Add_Label();
	//void Edit_Label();
	int HitTestEx(CPoint & point);

	afx_msg LRESULT OnHotKey(WPARAM wParam,LPARAM lParam);
	vector<Bacnet_Label_Info> m_bac_label_vector;
	vector<Label_information> m_RelayLabelLst;
	vector<CRelayLabel> m_labelCtrlLst;
	CString m_strImgPathName;
	CString m_strScreenName;
	unsigned int m_nSerialNumber;
	int m_nTstatID;
	int m_InstanceID;
	unsigned short m_Mac;
	CRelayLabel m_Label;
	BOOL m_bImgExist;
	CppSQLite3DB SqliteDBT3000;
	CppSQLite3Query q;
	int m_cxScreen;
	int m_cyScreen;
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT  RedrawScreeneditWindow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT  Add_label_Handle(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT  Edit_label_Handle(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT  Delete_Label_Handle(WPARAM wParam, LPARAM lParam);
	virtual void OnCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonScreenExit();
	afx_msg void OnBnClickedButtonDelete();
	void AddLabel(unsigned char point_type,uint8_t point_number,uint8_t main_panel,uint8_t sub_panel,unsigned int point_x,unsigned int point_y, unsigned char network = 0);
	void SaveBacLabel(int nItem);

	HICON default_icon;
	HICON default_on_icon;
	HICON default_off_icon;

	HICON default_input_icon;
	HICON default_output_icon;
	HICON default_variable_icon;
	HICON default_program_icon;
	HICON default_pid_icon;
	HICON default_holiday_icon;
	HICON default_schedual_icon;
	HICON default_trendlog_icon;
	HICON default_screen_icon;

	HICON lock_icon;
	HICON unlock_icon;
    
	int m_paint_right_limit ;
	int m_paint_botton_limit ;
	CString m_building_image_folder;
	bool m_enable_send_remote_point;
	bool m_full_screen_mode;
//	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    static DWORD WINAPI  ReadAllPanelThreadfun(LPVOID lpVoid);
    static DWORD WINAPI  ReadGroupDataThreadfun(LPVOID lpVoid);
    static DWORD WINAPI  ReadStandardThreadfun(LPVOID lpVoid);
    afx_msg void OnClose();
};

#define LOCK_ICON_SIZE_X 48
#define LOCK_ICON_SIZE_Y 48


