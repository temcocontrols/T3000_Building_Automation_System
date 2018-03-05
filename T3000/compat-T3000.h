 

#ifndef COMPAT_H

T3000LIB_API void T3000L_module(T3000_State *L, const char *libname,
                                       const luaL_reg *l, int nup);
#define T3000L_openlib T3000L_module

#endif
