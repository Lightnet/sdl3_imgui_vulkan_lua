// module_imgui_lua.cpp
#include "module_imgui_lua.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imgui.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#ifdef __cplusplus
}
#endif

// Lua state
static lua_State* L = nullptr;

// Function to check if a file exists
static bool FileExists(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}

// Lua error handler
static int lua_error_handler(lua_State* L) {
    const char* error = lua_tostring(L, -1);
    fprintf(stderr, "Lua error: %s\n", error ? error : "unknown error");
    lua_pop(L, 1); // Remove error message from stack
    return 0;
}

// Lua binding for ImGui::Begin
static int lua_imgui_begin(lua_State* L) {
    const char* name = luaL_checkstring(L, 1);
    bool open = true;
    if (lua_isboolean(L, 2)) {
        open = lua_toboolean(L, 2);
    }
    bool result = ImGui::Begin(name, &open);
    lua_pushboolean(L, result);
    lua_pushboolean(L, open);
    return 2; // Return result and open state
}

// Lua binding for ImGui::End
static int lua_imgui_end(lua_State* L) {
    ImGui::End();
    return 0;
}

// Lua binding for ImGui::Text
static int lua_imgui_text(lua_State* L) {
    const char* text = luaL_checkstring(L, 1);
    ImGui::Text("%s", text);
    return 0;
}

// Lua binding for ImGui::Button
static int lua_imgui_button(lua_State* L) {
    const char* label = luaL_checkstring(L, 1);
    bool clicked = ImGui::Button(label);
    lua_pushboolean(L, clicked);
    return 1;
}

static int lua_imgui_slider_float(lua_State* L) {
    const char* label = luaL_checkstring(L, 1);
    float value = (float)luaL_checknumber(L, 2);
    float min = (float)luaL_checknumber(L, 3);
    float max = (float)luaL_checknumber(L, 4);
    bool changed = ImGui::SliderFloat(label, &value, min, max);
    lua_pushboolean(L, changed);
    lua_pushnumber(L, value);
    return 2;
}

// Initialize Lua and load script.lua
bool InitLua(const char* script_file) {
    L = luaL_newstate();
    if (!L) {
        fprintf(stderr, "Failed to create Lua state\n");
        return false;
    }
    luaL_openlibs(L); // Open standard Lua libraries

    // Register ImGui functions
    lua_newtable(L);
    lua_pushcfunction(L, lua_imgui_begin);
    lua_setfield(L, -2, "Begin");
    lua_pushcfunction(L, lua_imgui_end);
    lua_setfield(L, -2, "End");
    lua_pushcfunction(L, lua_imgui_text);
    lua_setfield(L, -2, "Text");
    lua_pushcfunction(L, lua_imgui_button);
    lua_setfield(L, -2, "Button");
    lua_pushcfunction(L, lua_imgui_slider_float);
    lua_setfield(L, -2, "SliderFloat");

    lua_setglobal(L, "ImGui");

    // Check for script file (default to script.lua if nullptr)
    const char* filename = script_file ? script_file : "script.lua";
    if (FileExists(filename)) {
        if (luaL_dofile(L, filename) != LUA_OK) {
            fprintf(stderr, "Error loading %s: %s\n", filename, lua_tostring(L, -1));
            lua_pop(L, 1);
            return false;
        }
        printf("Loaded %s successfully\n", filename);
    } else {
        printf("%s not found, skipping\n", filename);
    }

    // Check for Lua draw function
    lua_getglobal(L, "draw");
    if (!lua_isfunction(L, -1)) {
        fprintf(stderr, "No 'draw' function found in %s\n", filename);
        lua_pop(L, 1);
        return false; // Return false to indicate no valid draw function
    }
    lua_pop(L, 1);
    return true;
}

// Call Lua draw function each frame
void RunLuaDraw() {
    if (!L) return;
    lua_getglobal(L, "draw");
    if (!lua_isfunction(L, -1)) {
        lua_pop(L, 1);
        return; // Skip if draw is not a function
    }
    // Push error handler
    lua_pushcfunction(L, lua_error_handler);
    lua_insert(L, -2); // Move error handler below the draw function
    if (lua_pcall(L, 0, 0, -2) != LUA_OK) {
        // Error is handled by lua_error_handler, no need to log here
        lua_pop(L, 1); // Remove error handler
        return;
    }
    lua_pop(L, 1); // Remove error handler
}

// Cleanup Lua
void CleanupLua() {
    if (L) {
        lua_close(L);
        L = nullptr;
    }
}