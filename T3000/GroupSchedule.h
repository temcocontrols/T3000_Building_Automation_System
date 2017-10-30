


#ifndef GroupSchedule_h
#define GroupSchedule_h


#include <stddef.h>
#include <stdio.h>

#include "lua.h"





/* extra error code for `GroupSchedule_load' */
#define LUA_ERRFILE     (LUA_ERRERR+1)


typedef struct GroupSchedule_Reg {
  const char *name;
  lua_CFunction func;
} GroupSchedule_Reg;



GroupSchedule_API void (luaI_openlib) (lua_State *L, const char *libname,
                                const GroupSchedule_Reg *l, int nup);
GroupSchedule_API void (GroupSchedule_register) (lua_State *L, const char *libname,
                                const GroupSchedule_Reg *l);
GroupSchedule_API int (GroupSchedule_getmetafield) (lua_State *L, int obj, const char *e);
GroupSchedule_API int (GroupSchedule_callmeta) (lua_State *L, int obj, const char *e);
GroupSchedule_API int (GroupSchedule_typerror) (lua_State *L, int narg, const char *tname);
GroupSchedule_API int (GroupSchedule_argerror) (lua_State *L, int numarg, const char *extramsg);
GroupSchedule_API const char *(GroupSchedule_checklstring) (lua_State *L, int numArg,
                                                          size_t *l);
GroupSchedule_API const char *(GroupSchedule_optlstring) (lua_State *L, int numArg,
                                          const char *def, size_t *l);
GroupSchedule_API lua_Number (GroupSchedule_checknumber) (lua_State *L, int numArg);
GroupSchedule_API lua_Number (GroupSchedule_optnumber) (lua_State *L, int nArg, lua_Number def);

GroupSchedule_API lua_Integer (GroupSchedule_checkinteger) (lua_State *L, int numArg);
GroupSchedule_API lua_Integer (GroupSchedule_optinteger) (lua_State *L, int nArg,
                                          lua_Integer def);

GroupSchedule_API void (GroupSchedule_checkstack) (lua_State *L, int sz, const char *msg);
GroupSchedule_API void (GroupSchedule_checktype) (lua_State *L, int narg, int t);
GroupSchedule_API void (GroupSchedule_checkany) (lua_State *L, int narg);

GroupSchedule_API int   (GroupSchedule_newmetatable) (lua_State *L, const char *tname);
GroupSchedule_API void *(GroupSchedule_checkudata) (lua_State *L, int ud, const char *tname);

GroupSchedule_API void (GroupSchedule_where) (lua_State *L, int lvl);
GroupSchedule_API int (GroupSchedule_error) (lua_State *L, const char *fmt, ...);

GroupSchedule_API int (GroupSchedule_checkoption) (lua_State *L, int narg, const char *def,
                                   const char *const lst[]);

GroupSchedule_API int (GroupSchedule_ref) (lua_State *L, int t);
GroupSchedule_API void (GroupSchedule_unref) (lua_State *L, int t, int ref);

GroupSchedule_API int (GroupSchedule_loadfile) (lua_State *L, const char *filename);
GroupSchedule_API int (GroupSchedule_loadbuffer) (lua_State *L, const char *buff, size_t sz,
                                  const char *name);
GroupSchedule_API int (GroupSchedule_loadstring) (lua_State *L, const char *s);

GroupSchedule_API lua_State *(GroupSchedule_newstate) (void);


GroupSchedule_API const char *(GroupSchedule_gsub) (lua_State *L, const char *s, const char *p,
                                                  const char *r);

GroupSchedule_API const char *(GroupSchedule_findtable) (lua_State *L, int idx,
                                         const char *fname, int szhint);






#define GroupSchedule_argcheck(L, cond,numarg,extramsg)	\
		((void)((cond) || GroupSchedule_argerror(L, (numarg), (extramsg))))
#define GroupSchedule_checkstring(L,n)	(GroupSchedule_checklstring(L, (n), NULL))
#define GroupSchedule_optstring(L,n,d)	(GroupSchedule_optlstring(L, (n), (d), NULL))
#define GroupSchedule_checkint(L,n)	((int)GroupSchedule_checkinteger(L, (n)))
#define GroupSchedule_optint(L,n,d)	((int)GroupSchedule_optinteger(L, (n), (d)))
#define GroupSchedule_checklong(L,n)	((long)GroupSchedule_checkinteger(L, (n)))
#define GroupSchedule_optlong(L,n,d)	((long)GroupSchedule_optinteger(L, (n), (d)))

#define GroupSchedule_typename(L,i)	lua_typename(L, lua_type(L,(i)))

#define GroupSchedule_dofile(L, fn) \
	(GroupSchedule_loadfile(L, fn) || lua_pcall(L, 0, LUA_MULTRET, 0))

#define GroupSchedule_dostring(L, s) \
	(GroupSchedule_loadstring(L, s) || lua_pcall(L, 0, LUA_MULTRET, 0))

#define GroupSchedule_getmetatable(L,n)	(lua_getfield(L, LUA_REGISTRYINDEX, (n)))

#define GroupSchedule_opt(L,f,n,d)	(lua_isnoneornil(L,(n)) ? (d) : f(L,(n)))

/*
** {======================================================
** Generic Buffer manipulation
** =======================================================
*/



typedef struct GroupSchedule_Buffer {
  char *p;			/* current position in buffer */
  int lvl;  /* number of strings in the stack (level) */
  lua_State *L;
  char buffer[GroupSchedule_BUFFERSIZE];
} GroupSchedule_Buffer;

#define GroupSchedule_addchar(B,c) \
  ((void)((B)->p < ((B)->buffer+GroupSchedule_BUFFERSIZE) || GroupSchedule_prepbuffer(B)), \
   (*(B)->p++ = (char)(c)))

/* compatibility only */
#define GroupSchedule_putchar(B,c)	GroupSchedule_addchar(B,c)

#define GroupSchedule_addsize(B,n)	((B)->p += (n))

GroupSchedule_API void (GroupSchedule_buffinit) (lua_State *L, GroupSchedule_Buffer *B);
GroupSchedule_API char *(GroupSchedule_prepbuffer) (GroupSchedule_Buffer *B);
GroupSchedule_API void (GroupSchedule_addlstring) (GroupSchedule_Buffer *B, const char *s, size_t l);
GroupSchedule_API void (GroupSchedule_addstring) (GroupSchedule_Buffer *B, const char *s);
GroupSchedule_API void (GroupSchedule_addvalue) (GroupSchedule_Buffer *B);
GroupSchedule_API void (GroupSchedule_pushresult) (GroupSchedule_Buffer *B);





