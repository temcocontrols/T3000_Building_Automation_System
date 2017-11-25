
#ifndef ldebug_h
#define ldebug_h


#include "lstate.h"


#define pcRel(pc, p)	(cast(int, (pc) - (p)->code) - 1)

#define getline(f,pc)	(((f)->lineinfo) ? (f)->lineinfo[pc] : 0)

#define resethookcount(L)	(L->hookcount = L->basehookcount)


T3000ProgramI_FUNC void T3000ProgramG_typeerror (T3000Program_State *L, const TValue *o,
                                             const char *opname);
T3000ProgramI_FUNC void T3000ProgramG_concaterror (T3000Program_State *L, StkId p1, StkId p2);
T3000ProgramI_FUNC void T3000ProgramG_aritherror (T3000Program_State *L, const TValue *p1,
                                              const TValue *p2);
T3000ProgramI_FUNC int T3000ProgramG_ordererror (T3000Program_State *L, const TValue *p1,
                                             const TValue *p2);
T3000ProgramI_FUNC void T3000ProgramG_runerror (T3000Program_State *L, const char *fmt, ...);
T3000ProgramI_FUNC void T3000ProgramG_errormsg (T3000Program_State *L);
T3000ProgramI_FUNC int T3000ProgramG_checkcode (const Proto *pt);
T3000ProgramI_FUNC int T3000ProgramG_checkopenop (Instruction i);

#endif
