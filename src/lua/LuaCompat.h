#ifndef LUAINC_H
#define LUAINC_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef LUA_INC
#include "lua5.1/lua.h"
#include "lua5.1/lauxlib.h"
#include "lua5.1/lualib.h"
#endif
#ifndef LUAJIT
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#else
#include "luajit.h"
#include "lauxlib.h"
#include "lualib.h"
#endif
#ifdef LUAJIT_INC
#include "luajit-2.0/lua.h"
#include "luajit-2.0/luaxlib.h"
#include "luajit-2.0/lualib.h"
#endif

#if LUA_VERSION_NUM >= 502
#define luaL_getn(L,i)		lua_rawlen(L, (i))
#define LUA_GLOBALSINDEX	LUA_RIDX_GLOBALS

LUALIB_API int (luaL_typerror) (lua_State *L, int narg, const char *tname);
#endif

#ifdef __cplusplus
}
#endif

#endif