// module_imgui_lua.hpp
#ifndef MODULE_IMGUI_LUA_HPP
#define MODULE_IMGUI_LUA_HPP

#ifdef __cplusplus
extern "C" {
#endif
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#ifdef __cplusplus
}
#endif

bool InitLua(const char* script_file = nullptr);
void RunLuaDraw();
void CleanupLua();

#endif