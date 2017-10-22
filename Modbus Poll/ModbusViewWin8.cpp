

#include "stdafx.h"
#include "resource.h"

#include "ModbusViewWin8.h"
#include "Broadcast.h"
#include "IOWindow.h"

#include <boost/bind.hpp>

//-----------------------------------------------------------------------------

using namespace Defs;





ModbusViewWin8::ModbusViewWin8()
{
	init();
}

//-----------------------------------------------------------------------------


Defs::Breakpoint ModbusViewWin8::ToggleBreakpoint(int line, const CString& path)
{
	if (lua_.get() == 0)
		return BPT_NO_CODE;

	return lua_->ToggleBreakpoint(line + 1) ? BPT_EXECUTE : BPT_NONE;
}



void ModbusViewWin8::SimEvent(int event, int data)
{
	if (event == Lua::NewLine)
		current_line_ = data;

//	CBroadcast::SendMessageToPopups(CBroadcast::WM_USER_NEW_LINE, event, data);
	AfxGetMainWnd()->PostMessage(CBroadcast::WM_USER_NEW_LINE, event, data);

	fin_stat = event == Lua::Finished ? SYM_FIN : SYM_RUN;
}


void ModbusViewWin8::SetNewContext(std::auto_ptr<Lua> lua)
{
	lua_ = lua;
	current_line_ = 0;
//	fin_stat = SYM_STOP;
	fin_stat = SYM_OK;
//	log_.Clear();

//	theApp.global_.ClearOutput();

	if (CWnd* terminal= io_window())
		terminal->SendMessage(CIOWindow::CMD_CLS);

	if (lua_.get())
		lua_->SetCallback(boost::bind(&ModbusViewWin8::SimEvent, this, _1, _2));
}


int ModbusViewWin8::GetCurrentLine() const
{
	return current_line_;
}


Defs::SymStat ModbusViewWin8::StepInto()
{
	if (lua_.get() == 0)
		return SYM_FIN;

	lua_->StepInto();
	return SYM_OK;
}

//-----------------------------------------------------------------------------

Defs::SymStat ModbusViewWin8::StepOver()
{
	if (lua_.get() == 0)
		return SYM_FIN;

	lua_->StepOver();
	return SYM_OK;
}


//-----------------------------------------------------------------------------

Defs::SymStat ModbusViewWin8::StepOut()
{
	if (lua_.get() == 0)
		return SYM_FIN;

	lua_->StepOut();
	return SYM_OK;
}


//-----------------------------------------------------------------------------

Defs::SymStat ModbusViewWin8::Run()
{
	if (lua_.get() == 0)
		return SYM_FIN;

	lua_->Run();
	return SYM_OK;
}

//-----------------------------------------------------------------------------

//void ModbusViewWin8::SkipToAddr(UINT16 addr)
//{
//}
//
//
//Defs::SymStat ModbusViewWin8::SkipInstr()
//{
//	return SYM_OK;
//}

//-----------------------------------------------------------------------------


//void ModbusViewWin8::ExitSym()
//{
//	//ASSERT(running == false);
//	//ResetPointer();               // schowanie strza³ki
//	//CMainFrame* main = (CMainFrame*) AfxGetApp()->main_wnd_;
//	//  main->ShowRegisterBar(false);
//}

//-----------------------------------------------------------------------------

CString ModbusViewWin8::GetLastStatMsg()
{
	if (lua_.get())
		return lua_->Status().c_str();

	return "Inactive";
}


//CString ModbusViewWin8::GetStatMsg(SymStat stat)
//{
//	CString msg;
	//switch (stat)
	//{
	//case SYM_OK:
	//case SYM_BPT_TEMP:
	//	msg.LoadString(IDS_SYM_STAT_OK);
	//	break;
	//case SYM_BPT_EXECUTE:
	//	msg.LoadString(IDS_SYM_STAT_BPX);
	//	break;
	//case SYM_BPT_READ:
	//	msg.LoadString(IDS_SYM_STAT_BPR);
	//	break;
	//case SYM_BPT_WRITE:
	//	msg.LoadString(IDS_SYM_STAT_BPW);
	//	break;
	//case SYM_ILLEGAL_CODE:
	//	msg.LoadString(IDS_SYM_STAT_ILL);
	//	break;
	//case SYM_STOP:
	//	msg.LoadString(IDS_SYM_STAT_STOP);
	//	break;
	//case SYM_FIN:
	//	msg.LoadString(IDS_SYM_STAT_FIN);
	//	break;
	//case SYM_RUN:
	//	msg.LoadString(IDS_SYM_STAT_RUN);
	//	break;
	//case SYM_INP_WAIT:
	//	msg.LoadString(IDS_SYM_STAT_INP_WAIT);
	//	break;
	//case SYM_ILL_WRITE:
	//	msg.LoadString(IDS_SYM_ILL_WRITE);
	//	break;
	//default:
	//	ASSERT(false);
	//}
//	return msg;
//}

//-----------------------------------------------------------------------------


//void ModbusViewWin8::Restart(const COutputMem &mem)
//{
//	//ctx.Reset(mem);
//	//old = ctx;
//	//fin_stat = SYM_OK;
//	//log_.Clear();
//}


//void ModbusViewWin8::SymStart(UINT16 org)
//{
//	//ctx.pc = org;
//	//ctx.s = 0xFF;
//	//if (debug)
//	//{
//	//	CDebugLine dl;
//	//	debug->GetLine(dl,org);
//	//	//    ASSERT(dl.flags != Defs::DBG_EMPTY);      // brak wiersza odp. pocz¹tkowi programu
//	//	SetPointer(dl.line,org);    // ustawienie strza³ki (->) przed aktualnym wierszem
//	//}
//}


//void ModbusViewWin8::SetPointer(const CLine &line, UINT16 addr) // ustawienie strza³ki (->) przed aktualnym wierszem
//{
	//POSITION posDoc= theApp.doc_deasm_template_->GetFirstDocPosition();
	//while (posDoc != NULL)        // s¹ okna z deasemblera?
	//{
	//	CDocument *doc= theApp.doc_deasm_template_->GetNextDoc(posDoc);
	//	ASSERT(doc->IsKindOf(RUNTIME_CLASS(CDeasm6502Doc)));
	//	((CDeasm6502Doc*)doc)->SetPointer(addr,true);
	//}

	//LuaSrcView* view= FindDocView(line.file);  // odszukanie okna dokumentu
	//if (fuid_last_view_ != line.file && ::IsWindow(last_view_))        // zmiana okna?
	//{
	//	if (LuaSrcView* view= dynamic_cast<LuaSrcView*>(CWnd::FromHandlePermanent(last_view_)))
	//		SetPointer(view, -1, false);              // ukrycie strza³ki
	//	last_view_ = 0;
	//}
	//if (!view && debug)
	//{
	//	if (const TCHAR* path= debug->GetFilePath(line.file))
	//	{                                           // próba otwarcia dokumentu...
	//		StudioApp* app= static_cast<StudioApp*>( AfxGetApp() );
	//		app->do_not_add_to_recent_file_list_ = true;
	//		CDocument* doc= app->OpenDocumentFile(path);
	//		app->do_not_add_to_recent_file_list_ = false;
	//		if (LuaSrcDoc* src_doc= dynamic_cast<LuaSrcDoc*>(doc))
	//		{
	//			POSITION pos = src_doc->GetFirstViewPosition();
	//			if (pos != NULL)
	//				view = dynamic_cast<LuaSrcView*>(src_doc->GetNextView(pos));
	//		}
	//	}
	//}
	//if (!view)
	//{
	//	//    ResetPointer();   // schowanie strza³ki, jeœli by³a
	//	return;             // nie ma okna dokumentu zawieraj¹cego aktualny wiersz
	//}

	//SetPointer(view, line.ln, true);  // wymuszenie przesuniêcia zawartoœci okna, jeœli potrzeba
	//fuid_last_view_ = line.file;
	//last_view_ = view->m_hWnd;
//}

//void ModbusViewWin8::SetPointer(LuaSrcView* view, int line, bool scroll)
//{
	//if (view == 0)
	//	return;

	//CDocument* doc= view->GetDocument();
	//POSITION pos= doc->GetFirstViewPosition();
	//while (pos != NULL)
	//{
	//	if (LuaSrcView* src_view= dynamic_cast<LuaSrcView*>(doc->GetNextView(pos)))
	//		src_view->SetPointer(line, scroll && src_view == view);
	//}
//}


//void ModbusViewWin8::ResetPointer()   // schowanie strza³ki
//{
	//POSITION posDoc= theApp.doc_deasm_template_->GetFirstDocPosition();
	//while (posDoc != NULL)        // s¹ okna z deasemblera?
	//{
	//	if (CDeasm6502Doc* doc= dynamic_cast<CDeasm6502Doc*>( theApp.doc_deasm_template_->GetNextDoc(posDoc) ))
	//		doc->SetPointer(-1,true);
	//}

	//if (fuid_last_view_)
	//{
	//	if (LuaSrcView* view= FindDocView(fuid_last_view_))
	//		SetPointer(view, -1, false);    // zmazanie strza³ki
	//}
	//fuid_last_view_ = 0;
//}


//LuaSrcView *ModbusViewWin8::FindDocView(FileUID fuid)
//{
	//if (debug == NULL)
	//	return NULL;
	//if (CFrameWnd* frame= dynamic_cast<CFrameWnd*>(AfxGetMainWnd()))
	//	if (CFrameWnd* active= frame->GetActiveFrame())
	//		if (LuaSrcView* view= dynamic_cast<LuaSrcView*>(active->GetActiveView()))
	//			if (debug->GetFileUID(view->GetDocument()->GetPathName()) == fuid)
	//				return view;

//	return 0;
//}


//=============================================================================

CWnd *ModbusViewWin8::io_open_window()  // otwarcie okna terminala
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,ID_VIEW_IO_WINDOW);
	return io_window();
}


CWnd *ModbusViewWin8::io_window()     // odszukanie okna terminala
{
	static CString name;
	static bool loaded= false;
	if (!loaded)
	{
		name.LoadString(IDS_IO_WINDOW);
		loaded = true;
	}
	return CWnd::FindWindow(NULL,name);
}

#if 0

UINT8 ModbusViewWin8::io_function()
{
	CWnd *terminal= io_window();
	if (terminal == NULL)
		terminal = io_open_window();
	if (terminal == 0 || !::IsWindow(terminal->m_hWnd))
	{
		io_func = IO_NONE;
		return 0;
	}

	int arg= 0;

	if (io_func == TERMINAL_IN)
	{
		arg = terminal->SendMessage(CIOWindow::CMD_IN);
	}
	else
	{
		if (io_func == TERMINAL_GET_X_POS || io_func == TERMINAL_GET_Y_POS)
			arg = terminal->SendMessage(CIOWindow::CMD_POSITION, io_func == TERMINAL_GET_X_POS ? 0x3 : 0x2);
		else
			arg = 0;
	}

	io_func = IO_NONE;

	if (arg == -1)  // break?
	{
		Break();
		return 0;
	}

	return UINT8(arg);
}

Defs::SymStat ModbusViewWin8::io_function(UINT8 arg)
{
	CWnd *terminal= io_window();
	if (terminal == NULL)
		terminal = io_open_window();

	switch (io_func)
	{
	case TERMINAL_OUT:
		if (terminal)
			terminal->SendMessage(CIOWindow::CMD_PUTC, arg, 0);
		break;

	case TERMINAL_OUT_CHR:
		if (terminal)
			terminal->SendMessage(CIOWindow::CMD_PUTC, arg, 1);
		break;

	case TERMINAL_OUT_HEX:
		if (terminal)
			terminal->SendMessage(CIOWindow::CMD_PUTC, arg, 2);
		break;

	case TERMINAL_CLS:
		if (terminal)
			terminal->SendMessage(CIOWindow::CMD_CLS);
		break;

	case TERMINAL_IN:
		ASSERT(false);
		/*		if (terminal)
		arg = UINT8(terminal->SendMessage(CIOWindow::CMD_IN));
		else
		arg = 0; */
		break;

	case TERMINAL_SET_X_POS:
	case TERMINAL_SET_Y_POS:
		if (terminal)
			terminal->SendMessage(CIOWindow::CMD_POSITION, io_func == TERMINAL_SET_X_POS ? 0x1 : 0x0, arg);
		break;

	default:
		ASSERT(false);            // nierozpoznana funkcja
	}

	io_func = IO_NONE;
	return SYM_OK;
}


//void ModbusViewWin8::OutputChar(int c)
//{
//	CWnd* terminal= io_window();
//	if (terminal == NULL)
//		terminal = io_open_window();
//
//	if (terminal)
//		terminal->SendMessage(CIOWindow::CMD_PUTC, c, 0);
//}
#endif


///////////////////////////////////////////////////////////////////////////////

void ModbusViewWin8::init()
{
//	running = false;
//	fuid_last_view_ = 0;
	fin_stat = SYM_OK;
//	thread = 0;
//    io_enabled = false;
//    io_addr = 0xE000;
//	io_func = IO_NONE;
	current_line_ = 0;
}


void ModbusViewWin8::Break()
{
	if (lua_.get())
		lua_->Break();
}


void ModbusViewWin8::AbortProg()
{
	lua_.reset();
}


bool ModbusViewWin8::IsRunning() const
{
	if (lua_.get())
		return lua_->IsRunning();
	else
		return false;
}


bool ModbusViewWin8::IsStopped() const
{
	if (lua_.get())
		return lua_->IsStopped();
	else
		return false;	// it's not stopped, because it is gone
}


bool ModbusViewWin8::IsFinished() const
{
	if (lua_.get())
		return lua_->IsFinished();
	else
		return true;
}


bool ModbusViewWin8::IsActive() const
{
	return lua_.get() != 0;
}



//std::string ModbusViewWin8::GetCallStack() const
//{
//	if (lua_.get())
//		return lua_->GetCallStack();
//	else
//		return std::string();
//}


//bool ModbusViewWin8::GetGlobalVars(std::vector<Lua::Field>& vars, bool deep) const
//{
//	if (lua_.get())
//		return lua_->GetGlobalVars(vars, deep);
//	return false;
//}
//
//
//bool ModbusViewWin8::GetLocalVars(std::vector<Lua::Var>& vars) const
//{
//	if (lua_.get())
//		return lua_->GetLocalVars(vars);
//	return false;
//}
