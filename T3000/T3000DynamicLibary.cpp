

#include "stdafx.h"
#include "resource.h"
#include "DynamicHelp.h"
#include "CCrystalTextBuffer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



const int min_w= 80;
const int min_h= 50;
CString CDynamicHelp::wnd_class_;
static const TCHAR* REGISTRY_SECTION_= _T("DynamicHelp");
static const TCHAR* REGISTRY_WIDTH_= _T("Height");


CDynamicHelp::CDynamicHelp()
{
	if (wnd_class_.IsEmpty())
		RegisterWndClass();

	cx_left_border_ = cx_right_border_ = 0;
	cy_top_border_ = cy_bottom_border_ = 0;
	MRU_width_ = 300;
	default_size_ = CSize(MRU_width_, AfxGetApp()->GetProfileInt(REGISTRY_SECTION_, REGISTRY_WIDTH_, 500));
	header_height_ = 16;
}


void CDynamicHelp::RegisterWndClass()
{
	wnd_class_ = AfxRegisterWndClass(0, ::LoadCursor(NULL,IDC_ARROW), 0, 0);
}


CDynamicHelp::~CDynamicHelp()
{}


BEGIN_MESSAGE_MAP(CDynamicHelp, CControlBar)
	//{{AFX_MSG_MAP(CDynamicHelp)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_NCPAINT()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_COMMAND(IDCLOSE, OnCloseWnd)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnToolTipGetText)
	ON_MESSAGE(WM_USER, OnDelayedResize)
END_MESSAGE_MAP()

CSize CDynamicHelp::CalcFixedLayout(BOOL stretch, BOOL horz)
{
	CSize bar_size;

	if (horz)
	{
		bar_size.cx = min_w;
		bar_size.cy = min_h;

		if (stretch)
			bar_size.cx = 32767;
	}
	else
	{
		bar_size.cx = min_w;
		bar_size.cy = min_h;

		if (stretch)
			bar_size.cy = 32767;
	}

	return bar_size;
}

//-----------------------------------------------------------------------------


CSize CDynamicHelp::CalcDynamicLayout(int length, DWORD mode)
{
	return CalcLayout(mode, length);
}


CSize CDynamicHelp::CalcLayout(DWORD mode, int length)
{
	CSize result_size(50, 50);
	if (mode & (LM_HORZDOCK | LM_VERTDOCK))
	{
		CRect rect;
		CWnd* main_wnd= AfxGetMainWnd();
		if (CMDIFrameWnd* frame_wnd= dynamic_cast<CMDIFrameWnd*>(main_wnd))
			::GetClientRect(frame_wnd->wndMDI_client_, rect);
		else
			main_wnd->GetClientRect(rect);
		result_size = rect.Size();
		result_size += CSize(10, 10);
		if (mode & 0x8000)
			MRU_width_ = length;
		result_size.cx = MRU_width_;
	}

	if (mode & LM_MRUWIDTH)
		SizeToolBar(MRU_width_);
	else if (mode & LM_HORZDOCK)
		return CSize(result_size.cx, min_h);
	else if (mode & LM_VERTDOCK)
		return CSize(MRU_width_, result_size.cy);
	else if (length != -1)
		SizeToolBar(length, !!(mode & LM_LENGTHY));
	else if (style_ & CBRS_FLOATING)
		SizeToolBar(MRU_width_);
	else
		SizeToolBar(mode & LM_HORZ ? 32767 : 0);

	result_size = default_size_;

	if (mode & LM_COMMIT)
		MRU_width_ = result_size.cx;

	return result_size;
}


void CDynamicHelp::SizeToolBar(int length, bool vert/*= false*/)
{
	if (vert)
		default_size_.cy = length;
	else
		default_size_.cx = length;

	if (default_size_.cx < min_w)
		default_size_.cx = min_w;
	if (default_size_.cy < min_h)
		default_size_.cy = min_h;

	InvalidateRect(NULL);
}

//-----------------------------------------------------------------------------
void CDynamicHelp::OnNcPaint()
{
	EraseNonClient();
}


void CDynamicHelp::OnUpdateCmdUI(CFrameWnd* target, BOOL disable_if_no_hndler)
{}

//-----------------------------------------------------------------------------
static const TCHAR* title_= _T("Dynamic Help");

bool CDynamicHelp::Create(CWnd* parent_wnd, UINT id)
{
	CControlBar::Create(wnd_class_, title_,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		CRect(10, 10, 100, 100), parent_wnd, id);

	SetBarStyle(CBRS_ALIGN_RIGHT | CBRS_FLYBY | CBRS_GRIPPER | CBRS_SIZE_DYNAMIC);

	close_wnd_.Create(WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | CCS_NORESIZE | CCS_NOPARENTALIGN | CCS_NODIVIDER,
		CRect(0,0,0,0), this, -1);
	close_wnd_.SetButtonStructSize(sizeof(TBBUTTON));
	close_wnd_.AddBitmap(1, IDB_CLOSE_TB);
	close_wnd_.SetBitmapSize(CSize(8, 8));
	TBBUTTON btn;
	btn.iBitmap = 0;
	btn.idCommand = IDCLOSE;
	btn.fsState = TBSTATE_ENABLED;
	btn.fsStyle = TBSTYLE_BUTTON;
	btn.data = 0;
	btn.iString = 0;
	close_wnd_.AddButtons(1, &btn);

	Resize();

	return true;
}

//-----------------------------------------------------------------------------

BOOL CDynamicHelp::OnEraseBkgnd(CDC* dc)
{
	if (!IsFloating())
	{
		CRect rect;
		GetClientRect(rect);
		dc->FillSolidRect(rect.right - header_height_ - 2, rect.top, header_height_, header_height_, ::GetSysColor(COLOR_3DFACE));
	}

	return true;
}

//-----------------------------------------------------------------------------
static COLORREF rgb_help_bkgnd_= RGB(255,255,240);

void CDynamicHelp::DoPaint(CDC* dc)
{
	CRect rect;
	GetClientRect(rect);

	if (IsFloating())
	{
		dc->FillSolidRect(rect, rgb_help_bkgnd_);
	}
	else
	{
		dc->FillSolidRect(rect.left, rect.top, rect.Width(), header_height_, ::GetSysColor(COLOR_3DFACE));

		int w= rect.Width() - 9 - header_height_;
		int x= rect.left + 5;
		if (w > 0)
		{
			COLORREF rgb_light= RGB(255,255,255);
			COLORREF rgb_dark= ::GetSysColor(COLOR_3DSHADOW);
			dc->Draw3dRect(x, rect.top + 5, w, 3, rgb_light, rgb_dark);
			dc->Draw3dRect(x, rect.top + 9, w, 3, rgb_light, rgb_dark);
		}
		int height= rect.Height() - header_height_;
		if (height > 0)
			dc->FillSolidRect(rect.left, rect.top + header_height_, rect.Width(), height, rgb_help_bkgnd_);
	}
}


BOOL CDynamicHelp::OnToolTipGetText(UINT uId, NMHDR* nm_hdr, LRESULT* result)
{
  NMTTDISPINFO* TTT= (NMTTDISPINFO*)nm_hdr;

  TTT->lpszText = TTT->szText;
  TTT->szText[0] = 0;
  TTT->hinst = NULL;

  *result = 0;
  return TRUE;
}


void CDynamicHelp::Resize()
{
	CRect rect;
	GetClientRect(rect);

	if (help_wnd_.m_hWnd)
	{
		if (IsFloating())	// if it's floating it has a title already
		{
			// correction for afxData.cxBorder2 junk
			rect.DeflateRect(2, 2, 2, 2);

			if (close_wnd_.m_hWnd)
				close_wnd_.ShowWindow(SW_HIDE);

			help_wnd_.SetWindowPos(0, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
		}
		else
		{
			// it needs it's own title

			// correction for afxData.cxBorder2 junk
			rect.DeflateRect(2, 0, 2, 8);
			if (rect.bottom < rect.top)
				rect.bottom = rect.top;

			if (close_wnd_.m_hWnd)
				close_wnd_.SetWindowPos(0, rect.right - 16, rect.top + 2, 15, 14, SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);

			rect.top += header_height_;
			help_wnd_.SetWindowPos(0, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}
}


void CDynamicHelp::OnSize(UINT type, int cx, int cy)
{
	CControlBar::OnSize(type, cx, cy);

	Resize();
}


void CDynamicHelp::OnDestroy()
{
	AfxGetApp()->WriteProfileInt(REGISTRY_SECTION_, REGISTRY_WIDTH_, default_size_.cy);

	CControlBar::OnDestroy();
}


int CDynamicHelp::OnCreate(LPCREATESTRUCT create_struct)
{
	if (CControlBar::OnCreate(create_struct) == -1)
		return -1;

	DWORD dwStyle= WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL;
	help_wnd_.CWnd::Create(_T("RichEdit20A"), NULL, dwStyle, CRect(0,0,0,0), this, IDS_DYNAMIC_HELP);

	if (help_wnd_.m_hWnd == 0)
		return -1;

	help_wnd_.SetReadOnly();
	help_wnd_.SetBackgroundColor(false, rgb_help_bkgnd_);
	help_wnd_.SendMessage(EM_SETTEXTMODE, TM_RICHTEXT | TM_SINGLECODEPAGE);
	help_wnd_.SendMessage(EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, 0);

//	SetContextHelp(_T("<b>LDA</b> blah blah<p><u>importante</u> reioioi khjkcxn opoiiso fjoo."));
/*
CFile file("C:\\LDA2.rtf", CFile::modeRead);
CString str;
file.Read(str.GetBuffer(file.GetLength()), file.GetLength());
str.ReleaseBuffer(file.GetLength());
SetContextHelp(str);
*/
	return 0;
}


void CDynamicHelp::SetContextHelp(const TCHAR* text, const TCHAR* header/*= 0*/)
{
	CString text= _T(
		"{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang1033"
		"{\\fonttbl"
			"{\\f0\\fmodern\\fprq1\\fcharset0 Courier New;}"
			"{\\f1\\fswiss\\fcharset0 Arial;}"
		"}"
		"{"
			"\\colortbl ;\\red255\\green255\\blue255;\\red192\\green192\\blue192;\\red160\\green160\\blue160;\\red224\\green224\\blue224;"
		"}"
		"\\viewkind4\\uc1\\pard\\li0");

	const TCHAR* header_before= _T("\\f1\\fs20\\sa25\\sb25\\cf1\\cb3\\highlight3\\b\\ql\\~  \\~\\~\\~");
	const TCHAR* header_after= _T("\\~\\~\\~  \\~\\highlight0\\par");
	const TCHAR* font= _T("\\pard\\cf0\\b0\\f0\\fs20\\ql");

	if (header)
	{
		text += header_before;
		text += header;
		text += header_after;
		text += font;
		text += _T("\\par\\li60 ");
	}
	else
	{
		text += font;
	}

	#define PLAIN	_T("\\f1\\fs20\\b0 ")
	CString str= text;
	str.Replace(_T("{"), _T("\\{"));
	str.Replace(_T("}"), _T("\\}"));
	str.Replace(_T("#title#"), _T("\\fi350\\f0\\fs30\\b "));
	str.Replace(_T("#text#"), _T("\\par\\fi0\\f1\\fs20\\b0 "));
	str.Replace(_T("#syntax#"), PLAIN _T("\\par\\par\\sa70 Syntax:\\par\\sa0\\f0\\fs20\\b "));
	str.Replace(_T("#exmpl#"), PLAIN _T("\\par\\sa70 Example:\\par\\sa0\\f0\\fs20\\b0 "));
	str.Replace(_T("#desc#"), PLAIN _T("\\par\\sa70 Description:\\par\\sa0\\f1\\fs20\\b0 "));
	str.Replace(_T("#flags#"), PLAIN _T("\\par\\par Affects flags: \\f0\\fs24\\b0 "));
	str.Replace(_T("#modes#"), PLAIN _T("\\par\\par\\sa70 Addressing Modes:\\par\\sa0\\f0\\fs20\\b0 "));
	str.Replace(_T("<pre>"), _T("\\f0 "));
	str.Replace(_T("<small>"), _T("\\f1\\fs16 "));
	str.Replace(_T("|"), _T("\\f1 "));
	str.Replace(_T("\n"), _T("\\par "));

	text += str;
	text += _T("}");

	if (help_wnd_)
	{
		CString old;
		help_wnd_.GetWindowText(old);

		if (old != text)
			help_wnd_.SetWindowText(text);

		CRect rect(0,0,0,0);
		help_wnd_.GetRect(rect);
	}
}


void CDynamicHelp::DisplayHelp(const CString& line, int word_start, int word_end)
{
	CString help;
	const TCHAR* header= 0;

	if (line.IsEmpty() || word_start >= word_end)
		;
	else if (word_start == 0)	// no instruction can start here; also almost all directives are prohibited in the first column
	{
		// test io_area only
		//
	}
	else
	{
		int comment= line.Find(_T(';'));
		if (comment < 0 || comment > word_end)	// not inside a comment?
		{
			CString word= line.Mid(word_start, word_end - word_start);

			extern int MatchingDirectives(const CString& word, CString& out);
			CString GetDirectiveDesc(const CString& directive);
			extern int MatchingInstructions(const CString& word, CString& result);
			CString GetInstructionDesc(const CString& instruction);

			int matching= MatchingInstructions(word, help);
			if (matching == 1)
				help = GetInstructionDesc(help), header = _T("Instruction");
			else if (matching > 1)
				header = _T("Instructions");
			else
			{
				int matching= MatchingDirectives(word, help);
				if (matching == 1)
					help = GetDirectiveDesc(help), header = _T("Directive");
				else if (matching > 1)
					header = _T("Directives");
			}
		}
	}

	SetContextHelp(help, header);
}


void CDynamicHelp::OnBarStyleChange(DWORD old_style, DWORD new_style)
{
	PostMessage(WM_USER);
}

LRESULT CDynamicHelp::OnDelayedResize(WPARAM, LPARAM)
{
	Resize();
	return 0;
}


void CDynamicHelp::OnCloseWnd()
{
	if (CFrameWnd* frame_wnd= GetDockingFrame())
		frame_wnd->ShowControlBar(this, false, TRUE);

}
