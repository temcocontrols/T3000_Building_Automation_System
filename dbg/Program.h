
#ifndef lfunc_h
#define lfunc_h


#include "lobject.h"


#define sizeCclosure(n)	(cast(int, sizeof(CClosure)) + \
                         cast(int, sizeof(TValue)*((n)-1)))

#define sizeLclosure(n)	(cast(int, sizeof(LClosure)) + \
                         cast(int, sizeof(TValue *)*((n)-1)))


ProgrammaI_FUNC Proto *ProgrammaF_newproto (Programma_State *L);
ProgrammaI_FUNC Closure *ProgrammaF_newCclosure (Programma_State *L, int nelems, Table *e);
ProgrammaI_FUNC Closure *ProgrammaF_newLclosure (Programma_State *L, int nelems, Table *e);
ProgrammaI_FUNC UpVal *ProgrammaF_newupval (Programma_State *L);
ProgrammaI_FUNC UpVal *ProgrammaF_findupval (Programma_State *L, StkId level);
ProgrammaI_FUNC void ProgrammaF_close (Programma_State *L, StkId level);
ProgrammaI_FUNC void ProgrammaF_freeproto (Programma_State *L, Proto *f);
ProgrammaI_FUNC void ProgrammaF_freeclosure (Programma_State *L, Closure *c);
ProgrammaI_FUNC void ProgrammaF_freeupval (Programma_State *L, UpVal *uv);
ProgrammaI_FUNC const char *ProgrammaF_getlocalname (const Proto *func, int local_number,
                                         int pc);


#endif
