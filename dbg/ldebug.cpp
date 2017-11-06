#include <stdarg.h>
#include <stddef.h>
#include <string.h>


#define ldebug_c
#define T3000Program_CORE

#include "T3000Program.h"

#include "lapi.h"
#include "lcode.h"
#include "ldebug.h"
#include "ldo.h"
#include "lfunc.h"
#include "lobject.h"
#include "lopcodes.h"
#include "lstate.h"
#include "lstring.h"
#include "ltable.h"
#include "ltm.h"
#include "lvm.h"



static const char *getfuncname (T3000Program_State *L, CallInfo *ci, const char **name);


static int currentpc (T3000Program_State *L, CallInfo *ci) {
  if (!isT3000Program(ci)) return -1;  /* function is not a T3000Program function? */
  if (ci == L->ci)
    ci->savedpc = L->savedpc;
  return pcRel(ci->savedpc, ci_func(ci)->l.p);
}


static int currentline (T3000Program_State *L, CallInfo *ci) {
  int pc = currentpc(L, ci);
  if (pc < 0)
    return -1;  /* only active T3000Program functions have current-line information */
  else
    return getline(ci_func(ci)->l.p, pc);
}


/*
** this function can be called asynchronous (e.g. during a signal)
*/
T3000Program_API int T3000Program_sethook (T3000Program_State *L, T3000Program_Hook func, int mask, int count) {
  if (func == NULL || mask == 0) {  /* turn off hooks? */
    mask = 0;
    func = NULL;
  }
  L->hook = func;
  L->basehookcount = count;
  resethookcount(L);
  L->hookmask = cast_byte(mask);
  return 1;
}


T3000Program_API T3000Program_Hook T3000Program_gethook (T3000Program_State *L) {
  return L->hook;
}


T3000Program_API int T3000Program_gethookmask (T3000Program_State *L) {
  return L->hookmask;
}


T3000Program_API int T3000Program_gethookcount (T3000Program_State *L) {
  return L->basehookcount;
}


T3000Program_API int T3000Program_getstack (T3000Program_State *L, int level, T3000Program_Debug *ar) {
  int status;
  CallInfo *ci;
  T3000Program_lock(L);
  for (ci = L->ci; level > 0 && ci > L->base_ci; ci--) {
    level--;
    if (f_isT3000Program(ci))  /* T3000Program function? */
      level -= ci->tailcalls;  /* skip lost tail calls */
  }
  if (level == 0 && ci > L->base_ci) {  /* level found? */
    status = 1;
    ar->i_ci = cast_int(ci - L->base_ci);
  }
  else if (level < 0) {  /* level is of a lost tail call? */
    status = 1;
    ar->i_ci = 0;
  }
  else status = 0;  /* no such level */
  T3000Program_unlock(L);
  return status;
}


static Proto *getT3000Programproto (CallInfo *ci) {
  return (isT3000Program(ci) ? ci_func(ci)->l.p : NULL);
}


static const char *findlocal (T3000Program_State *L, CallInfo *ci, int n) {
  const char *name;
  Proto *fp = getT3000Programproto(ci);
  if (fp && (name = T3000ProgramF_getlocalname(fp, n, currentpc(L, ci))) != NULL)
    return name;  /* is a local variable in a T3000Program function */
  else {
    StkId limit = (ci == L->ci) ? L->top : (ci+1)->func;
    if (limit - ci->base >= n && n > 0)  /* is 'n' inside 'ci' stack? */
      return "(*temporary)";
    else
      return NULL;
  }
}


T3000Program_API const char *T3000Program_getlocal (T3000Program_State *L, const T3000Program_Debug *ar, int n) {
  CallInfo *ci = L->base_ci + ar->i_ci;
  const char *name = findlocal(L, ci, n);
  T3000Program_lock(L);
  if (name)
      T3000ProgramA_pushobject(L, ci->base + (n - 1));
  T3000Program_unlock(L);
  return name;
}


T3000Program_API const char *T3000Program_setlocal (T3000Program_State *L, const T3000Program_Debug *ar, int n) {
  CallInfo *ci = L->base_ci + ar->i_ci;
  const char *name = findlocal(L, ci, n);
  T3000Program_lock(L);
  if (name)
      setobjs2s(L, ci->base + (n - 1), L->top - 1);
  L->top--;  /* pop value */
  T3000Program_unlock(L);
  return name;
}


static void funcinfo (T3000Program_Debug *ar, Closure *cl) {
  if (cl->c.isC) {
    ar->source = "=[C]";
    ar->linedefined = -1;
    ar->lastlinedefined = -1;
    ar->what = "C";
  }
  else {
    ar->source = getstr(cl->l.p->source);
    ar->linedefined = cl->l.p->linedefined;
    ar->lastlinedefined = cl->l.p->lastlinedefined;
    ar->what = (ar->linedefined == 0) ? "main" : "T3000Program";
  }
  T3000ProgramO_chunkid(ar->short_src, ar->source, T3000Program_IDSIZE);
}


static void info_tailcall (T3000Program_Debug *ar) {
  ar->name = ar->namewhat = "";
  ar->what = "tail";
  ar->lastlinedefined = ar->linedefined = ar->currentline = -1;
  ar->source = "=(tail call)";
  T3000ProgramO_chunkid(ar->short_src, ar->source, T3000Program_IDSIZE);
  ar->nups = 0;
}


static void collectvalidlines (T3000Program_State *L, Closure *f) {
  if (f == NULL || f->c.isC) {
    setnilvalue(L->top);
  }
  else {
    Table *t = T3000ProgramH_new(L, 0, 0);
    int *lineinfo = f->l.p->lineinfo;
    int i;
    for (i=0; i<f->l.p->sizelineinfo; i++)
      setbvalue(T3000ProgramH_setnum(L, t, lineinfo[i]), 1);
    sethvalue(L, L->top, t); 
  }
  incr_top(L);
}


static int auxgetinfo (T3000Program_State *L, const char *what, T3000Program_Debug *ar,
                    Closure *f, CallInfo *ci) {
  int status = 1;
  if (f == NULL) {
    info_tailcall(ar);
    return status;
  }
  for (; *what; what++) {
    switch (*what) {
      case 'S': {
        funcinfo(ar, f);
        break;
      }
      case 'l': {
        ar->currentline = (ci) ? currentline(L, ci) : -1;
        break;
      }
      case 'u': {
        ar->nups = f->c.nupvalues;
        break;
      }
      case 'n': {
        ar->namewhat = (ci) ? getfuncname(L, ci, &ar->name) : NULL;
        if (ar->namewhat == NULL) {
          ar->namewhat = "";  /* not found */
          ar->name = NULL;
        }
        break;
      }
      case 'L':
      case 'f':  /* handled by T3000Program_getinfo */
        break;
      default: status = 0;  /* invalid option */
    }
  }
  return status;
}


T3000Program_API int T3000Program_getinfo (T3000Program_State *L, const char *what, T3000Program_Debug *ar) {
  int status;
  Closure *f = NULL;
  CallInfo *ci = NULL;
  T3000Program_lock(L);
  if (*what == '>') {
    StkId func = L->top - 1;
    T3000Programi_apicheck(L, ttisfunction(func));
    what++;  /* skip the '>' */
    f = clvalue(func);
    L->top--;  /* pop function */
  }
  else if (ar->i_ci != 0) {  /* no tail call? */
    ci = L->base_ci + ar->i_ci;
    T3000Program_assert(ttisfunction(ci->func));
    f = clvalue(ci->func);
  }
  status = auxgetinfo(L, what, ar, f, ci);
  if (strchr(what, 'f')) {
    if (f == NULL) setnilvalue(L->top);
    else setclvalue(L, L->top, f);
    incr_top(L);
  }
  if (strchr(what, 'L'))
    collectvalidlines(L, f);
  T3000Program_unlock(L);
  return status;
}


/*
** {======================================================
** Symbolic Execution and code checker
** =======================================================
*/

#define check(x)		if (!(x)) return 0;

#define checkjump(pt,pc)	check(0 <= pc && pc < pt->sizecode)

#define checkreg(pt,reg)	check((reg) < (pt)->maxstacksize)



static int precheck (const Proto *pt) {
  check(pt->maxstacksize <= MAXSTACK);
  T3000Program_assert(pt->numparams+(pt->is_vararg & VARARG_HASARG) <= pt->maxstacksize);
  T3000Program_assert(!(pt->is_vararg & VARARG_NEEDSARG) ||
              (pt->is_vararg & VARARG_HASARG));
  check(pt->sizeupvalues <= pt->nups);
  check(pt->sizelineinfo == pt->sizecode || pt->sizelineinfo == 0);
  check(GET_OPCODE(pt->code[pt->sizecode-1]) == OP_RETURN);
  return 1;
}


#define checkopenop(pt,pc)	T3000ProgramG_checkopenop((pt)->code[(pc)+1])

int T3000ProgramG_checkopenop (Instruction i) {
  switch (GET_OPCODE(i)) {
    case OP_CALL:
    case OP_TAILCALL:
    case OP_RETURN:
    case OP_SETLIST: {
      check(GETARG_B(i) == 0);
      return 1;
    }
    default: return 0;  /* invalid instruction after an open call */
  }
}


static int checkArgMode (const Proto *pt, int r, enum OpArgMask mode) {
  switch (mode) {
    case OpArgN: check(r == 0); break;
    case OpArgU: break;
    case OpArgR: checkreg(pt, r); break;
    case OpArgK:
      check(ISK(r) ? INDEXK(r) < pt->sizek : r < pt->maxstacksize);
      break;
  }
  return 1;
}


static Instruction symbexec (const Proto *pt, int lastpc, int reg) {
  int pc;
  int last;  /* stores position of last instruction that changed `reg' */
  last = pt->sizecode-1;  /* points to final return (a `neutral' instruction) */
  check(precheck(pt));
  for (pc = 0; pc < lastpc; pc++) {
    Instruction i = pt->code[pc];
    OpCode op = GET_OPCODE(i);
    int a = GETARG_A(i);
    int b = 0;
    int c = 0;
    check(op < NUM_OPCODES);
    checkreg(pt, a);
    switch (getOpMode(op)) {
      case iABC: {
        b = GETARG_B(i);
        c = GETARG_C(i);
        check(checkArgMode(pt, b, getBMode(op)));
        check(checkArgMode(pt, c, getCMode(op)));
        break;
      }
      case iABx: {
        b = GETARG_Bx(i);
        if (getBMode(op) == OpArgK) check(b < pt->sizek);
        break;
      }
      case iAsBx: {
        b = GETARG_sBx(i);
        if (getBMode(op) == OpArgR) {
          int dest = pc+1+b;
          check(0 <= dest && dest < pt->sizecode);
          if (dest > 0) {
            /* cannot jump to a setlist count */
            Instruction d = pt->code[dest-1];
            check(!(GET_OPCODE(d) == OP_SETLIST && GETARG_C(d) == 0));
          }
        }
        break;
      }
    }
    if (testAMode(op)) {
      if (a == reg) last = pc;  /* change register `a' */
    }
    if (testTMode(op)) {
      check(pc+2 < pt->sizecode);  /* check skip */
      check(GET_OPCODE(pt->code[pc+1]) == OP_JMP);
    }
    switch (op) {
      case OP_LOADBOOL: {
        check(c == 0 || pc+2 < pt->sizecode);  /* check its jump */
        break;
      }
      case OP_LOADNIL: {
        if (a <= reg && reg <= b)
          last = pc;  /* set registers from `a' to `b' */
        break;
      }
      case OP_GETUPVAL:
      case OP_SETUPVAL: {
        check(b < pt->nups);
        break;
      }
      case OP_GETGLOBAL:
      case OP_SETGLOBAL: {
        check(ttisstring(&pt->k[b]));
        break;
      }
      case OP_SELF: {
        checkreg(pt, a+1);
        if (reg == a+1) last = pc;
        break;
      }
      case OP_CONCAT: {
        check(b < c);  /* at least two operands */
        break;
      }
      case OP_TFORLOOP: {
        check(c >= 1);  /* at least one result (control variable) */
        checkreg(pt, a+2+c);  /* space for results */
        if (reg >= a+2) last = pc;  /* affect all regs above its base */
        break;
      }
      case OP_FORLOOP:
      case OP_FORPREP:
        checkreg(pt, a+3);
        /* go through */
      case OP_JMP: {
        int dest = pc+1+b;
        /* not full check and jump is forward and do not skip `lastpc'? */
        if (reg != NO_REG && pc < dest && dest <= lastpc)
          pc += b;  /* do the jump */
        break;
      }
      case OP_CALL:
      case OP_TAILCALL: {
        if (b != 0) {
          checkreg(pt, a+b-1);
        }
        c--;  /* c = num. returns */
        if (c == T3000Program_MULTRET) {
          check(checkopenop(pt, pc));
        }
        else if (c != 0)
          checkreg(pt, a+c-1);
        if (reg >= a) last = pc;  /* affect all registers above base */
        break;
      }
      case OP_RETURN: {
        b--;  /* b = num. returns */
        if (b > 0) checkreg(pt, a+b-1);
        break;
      }
      case OP_SETLIST: {
        if (b > 0) checkreg(pt, a + b);
        if (c == 0) pc++;
        break;
      }
      case OP_CLOSURE: {
        int nup, j;
        check(b < pt->sizep);
        nup = pt->p[b]->nups;
        check(pc + nup < pt->sizecode);
        for (j = 1; j <= nup; j++) {
          OpCode op1 = GET_OPCODE(pt->code[pc + j]);
          check(op1 == OP_GETUPVAL || op1 == OP_MOVE);
        }
        if (reg != NO_REG)  /* tracing? */
          pc += nup;  /* do not 'execute' these pseudo-instructions */
        break;
      }
      case OP_VARARG: {
        check((pt->is_vararg & VARARG_ISVARARG) &&
             !(pt->is_vararg & VARARG_NEEDSARG));
        b--;
        if (b == T3000Program_MULTRET) check(checkopenop(pt, pc));
        checkreg(pt, a+b-1);
        break;
      }
      default: break;
    }
  }
  return pt->code[last];
}

#undef check
#undef checkjump
#undef checkreg

/* }====================================================== */


int T3000ProgramG_checkcode (const Proto *pt) {
  return (symbexec(pt, pt->sizecode, NO_REG) != 0);
}


static const char *kname (Proto *p, int c) {
  if (ISK(c) && ttisstring(&p->k[INDEXK(c)]))
    return svalue(&p->k[INDEXK(c)]);
  else
    return "?";
}


static const char *getobjname (T3000Program_State *L, CallInfo *ci, int stackpos,
                               const char **name) {
  if (isT3000Program(ci)) {  /* a T3000Program function? */
    Proto *p = ci_func(ci)->l.p;
    int pc = currentpc(L, ci);
    Instruction i;
    *name = T3000ProgramF_getlocalname(p, stackpos+1, pc);
    if (*name)  /* is a local? */
      return "local";
    i = symbexec(p, pc, stackpos);  /* try symbolic execution */
    T3000Program_assert(pc != -1);
    switch (GET_OPCODE(i)) {
      case OP_GETGLOBAL: {
        int g = GETARG_Bx(i);  /* global index */
        T3000Program_assert(ttisstring(&p->k[g]));
        *name = svalue(&p->k[g]);
        return "global";
      }
      case OP_MOVE: {
        int a = GETARG_A(i);
        int b = GETARG_B(i);  /* move from `b' to `a' */
        if (b < a)
          return getobjname(L, ci, b, name);  /* get name for `b' */
        break;
      }
      case OP_GETTABLE: {
        int k = GETARG_C(i);  /* key index */
        *name = kname(p, k);
        return "field";
      }
      case OP_GETUPVAL: {
        int u = GETARG_B(i);  /* upvalue index */
        *name = p->upvalues ? getstr(p->upvalues[u]) : "?";
        return "upvalue";
      }
      case OP_SELF: {
        int k = GETARG_C(i);  /* key index */
        *name = kname(p, k);
        return "method";
      }
      default: break;
    }
  }
  return NULL;  /* no useful name found */
}


static const char *getfuncname (T3000Program_State *L, CallInfo *ci, const char **name) {
  Instruction i;
  if ((isT3000Program(ci) && ci->tailcalls > 0) || !isT3000Program(ci - 1))
    return NULL;  /* calling function is not T3000Program (or is unknown) */
  ci--;  /* calling function */
  i = ci_func(ci)->l.p->code[currentpc(L, ci)];
  if (GET_OPCODE(i) == OP_CALL || GET_OPCODE(i) == OP_TAILCALL ||
      GET_OPCODE(i) == OP_TFORLOOP)
    return getobjname(L, ci, GETARG_A(i), name);
  else
    return NULL;  /* no useful name can be found */
}


/* only ANSI way to check whether a pointer points to an array */
static int isinstack (CallInfo *ci, const TValue *o) {
  StkId p;
  for (p = ci->base; p < ci->top; p++)
    if (o == p) return 1;
  return 0;
}


void T3000ProgramG_typeerror (T3000Program_State *L, const TValue *o, const char *op) {
  const char *name = NULL;
  const char *t = T3000ProgramT_typenames[ttype(o)];
  const char *kind = (isinstack(L->ci, o)) ?
                         getobjname(L, L->ci, cast_int(o - L->base), &name) :
                         NULL;
  if (kind)
    T3000ProgramG_runerror(L, "attempt to %s %s " T3000Program_QS " (a %s value)",
                op, kind, name, t);
  else
    T3000ProgramG_runerror(L, "attempt to %s a %s value", op, t);
}


void T3000ProgramG_concaterror (T3000Program_State *L, StkId p1, StkId p2) {
  if (ttisstring(p1) || ttisnumber(p1)) p1 = p2;
  T3000Program_assert(!ttisstring(p1) && !ttisnumber(p1));
  T3000ProgramG_typeerror(L, p1, "concatenate");
}


void T3000ProgramG_aritherror (T3000Program_State *L, const TValue *p1, const TValue *p2) {
  TValue temp;
  if (T3000ProgramV_tonumber(p1, &temp) == NULL)
    p2 = p1;  /* first operand is wrong */
  T3000ProgramG_typeerror(L, p2, "perform arithmetic on");
}


int T3000ProgramG_ordererror (T3000Program_State *L, const TValue *p1, const TValue *p2) {
  const char *t1 = T3000ProgramT_typenames[ttype(p1)];
  const char *t2 = T3000ProgramT_typenames[ttype(p2)];
  if (t1[2] == t2[2])
    T3000ProgramG_runerror(L, "attempt to compare two %s values", t1);
  else
    T3000ProgramG_runerror(L, "attempt to compare %s with %s", t1, t2);
  return 0;
}


static void addinfo (T3000Program_State *L, const char *msg) {
  CallInfo *ci = L->ci;
  if (isT3000Program(ci)) {  /* is T3000Program code? */
    char buff[T3000Program_IDSIZE];  /* add file:line information */
    int line = currentline(L, ci);
    T3000ProgramO_chunkid(buff, getstr(getT3000Programproto(ci)->source), T3000Program_IDSIZE);
    T3000ProgramO_pushfstring(L, "%s:%d: %s", buff, line, msg);
  }
}


void T3000ProgramG_errormsg (T3000Program_State *L) {
  if (L->errfunc != 0) {  /* is there an error handling function? */
    StkId errfunc = restorestack(L, L->errfunc);
    if (!ttisfunction(errfunc)) T3000ProgramD_throw(L, T3000Program_ERRERR);
    setobjs2s(L, L->top, L->top - 1);  /* move argument */
    setobjs2s(L, L->top - 1, errfunc);  /* push function */
    incr_top(L);
    T3000ProgramD_call(L, L->top - 2, 1);  /* call it */
  }
  T3000ProgramD_throw(L, T3000Program_ERRRUN);
}


void T3000ProgramG_runerror (T3000Program_State *L, const char *fmt, ...) {
  va_list argp;
  va_start(argp, fmt);
  addinfo(L, T3000ProgramO_pushvfstring(L, fmt, argp));
  va_end(argp);
  T3000ProgramG_errormsg(L);
}

