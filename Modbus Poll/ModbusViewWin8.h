

#ifndef _ModbusViewWin8_h_
#define _ModbusViewWin8_h_


#include "Defs.h"
#include "Lua.h"


class ModbusViewWin8
{

private:
	//IOFunc io_func;
	CWnd* io_window();		// find terminal window
	CWnd* io_open_window();	// open terminal window
	Defs::SymStat fin_stat;


public:

	CString GetLastStatMsg();

	ModbusViewWin8(); //: eventRedraw(true,true)
//	{ init(); }

	void Restart();
	void SymStart(UINT16 org);

	Defs::SymStat StepInto();
	Defs::SymStat StepOver();
	Defs::SymStat StepOut();
	Defs::SymStat Run();


	Defs::Breakpoint ToggleBreakpoint(int line, const CString& path);

//	void OutputChar(int c);

	bool IsFinished() const;
	bool IsRunning() const;		// is Lua program running?
	void Break();				// break program execution
	void AbortProg();
	bool IsStopped() const;
	bool IsActive() const;

	int GetCurrentLine() const;



	void SetNewContext(std::auto_ptr<Lua> lua);

	Lua* GetLua() const		{ return lua_.get(); }

private:
	void SimEvent(int event, int line);
	std::auto_ptr<Lua> lua_;
	int current_line_;

	void init();
};

#endif
