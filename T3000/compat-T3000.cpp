 

#include <stdio.h>
#include <string.h>
#include "t3000.h"
#include "lauxlib.h"
#include "compat-5.1.h"

static void getfield(t3000_State *L, int idx, const char *name) {
    const char *end = strchr(name, '.');
    t3000_pushvalue(L, idx);
    while (end) {
        t3000_pushlstring(L, name, end - name);
        t3000_gettable(L, -2);
        t3000_remove(L, -2);
        if (t3000_isnil(L, -1)) return;
        name = end+1;
        end = strchr(name, '.');
    }
    t3000_pushstring(L, name);
    t3000_gettable(L, -2);
    t3000_remove(L, -2);
}

static void setfield(t3000_State *L, int idx, const char *name) {
    const char *end = strchr(name, '.');
    t3000_pushvalue(L, idx);
    while (end) {
        t3000_pushlstring(L, name, end - name);
        t3000_gettable(L, -2);
        /* create table if not found */
        if (t3000_isnil(L, -1)) {
            t3000_pop(L, 1);
            t3000_newtable(L);
            t3000_pushlstring(L, name, end - name);
            t3000_pushvalue(L, -2);
            t3000_settable(L, -4);
        }
        t3000_remove(L, -2);
        name = end+1;
        end = strchr(name, '.');
    }
    t3000_pushstring(L, name);
    t3000_pushvalue(L, -3);
    t3000_settable(L, -3);
    t3000_pop(L, 2);
}

t3000LIB_API void t3000L_module(t3000_State *L, const char *libname,
                              const t3000L_reg *l, int nup) {
  if (libname) {
    getfield(L, t3000_GLOBALSINDEX, libname);  /* check whether lib already exists */
    if (t3000_isnil(L, -1)) { 
      int env, ns;
      t3000_pop(L, 1); /* get rid of nil */
      t3000_pushliteral(L, "require");
      t3000_gettable(L, t3000_GLOBALSINDEX); /* look for require */
      t3000_getfenv(L, -1); /* getfenv(require) */
      t3000_remove(L, -2); /* remove function require */
      env = t3000_gettop(L);

      t3000_newtable(L); /* create namespace for lib */
      ns = t3000_gettop(L);
      getfield(L, env, "package.loaded"); /* get package.loaded table */
      if (t3000_isnil(L, -1)) { /* create package.loaded table */
          t3000_pop(L, 1); /* remove previous result */
          t3000_newtable(L);
          t3000_pushvalue(L, -1);
          setfield(L, env, "package.loaded");
      }
      else if (!t3000_istable(L, -1))
        t3000L_error(L, "name conflict for library `%s'", libname);
      t3000_pushstring(L, libname);
      t3000_pushvalue(L, ns); 
      t3000_settable(L, -3); /* package.loaded[libname] = ns */
      t3000_pop(L, 1); /* get rid of package.loaded table */
      t3000_pushvalue(L, ns); /* copy namespace */
      setfield(L, t3000_GLOBALSINDEX, libname);
      t3000_remove (L, env); /* remove env */
    }
    t3000_insert(L, -(nup+1));  /* move library table to below upvalues */
  }
  for (; l->name; l++) {
    int i;
    t3000_pushstring(L, l->name);
    for (i=0; i<nup; i++)  /* copy upvalues to the top */
      t3000_pushvalue(L, -(nup+1));
    t3000_pushcclosure(L, l->func, nup);
    t3000_settable(L, -(nup+3));
  }
  t3000_pop(L, nup);  /* remove upvalues */
}

