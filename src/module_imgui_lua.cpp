// module_imgui_lua.cpp
#include "module_imgui_lua.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imgui.h"
#include <vector>
#include <string>


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

// Lua binding for ImGui::InputText
static int lua_imgui_input_text(lua_State* L) {
    const char* label = luaL_checkstring(L, 1);
    const char* initial_text = luaL_checkstring(L, 2);
    char buffer[256] = {0};
    strncpy(buffer, initial_text, sizeof(buffer) - 1);
    bool changed = ImGui::InputText(label, buffer, sizeof(buffer));
    lua_pushboolean(L, changed);
    lua_pushstring(L, buffer);
    return 2; // Return changed flag and new text
}

// Lua binding for ImGui::InputTextMultiline
static int lua_imgui_input_text_multiline(lua_State* L) {
    const char* label = luaL_checkstring(L, 1);
    const char* initial_text = luaL_checkstring(L, 2);
    float width = luaL_optnumber(L, 3, 0.0f);
    float height = luaL_optnumber(L, 4, 0.0f);
    char buffer[1024] = {0};
    strncpy(buffer, initial_text, sizeof(buffer) - 1);
    bool changed = ImGui::InputTextMultiline(label, buffer, sizeof(buffer), ImVec2(width, height));
    lua_pushboolean(L, changed);
    lua_pushstring(L, buffer);
    return 2; // Return changed flag and new text
}

// Lua binding for ImGui::SliderFloat
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

// Lua binding for ImGui::IsItemHovered
static int lua_imgui_is_item_hovered(lua_State* L) {
    bool hovered = ImGui::IsItemHovered();
    lua_pushboolean(L, hovered);
    return 1;
}

// Lua binding for ImGui::IsItemActive
static int lua_imgui_is_item_active(lua_State* L) {
    bool active = ImGui::IsItemActive();
    lua_pushboolean(L, active);
    return 1;
}

// Lua binding for ImGui::IsItemClicked
static int lua_imgui_is_item_clicked(lua_State* L) {
    bool clicked = ImGui::IsItemClicked();
    lua_pushboolean(L, clicked);
    return 1;
}

// Lua binding for ImGui::GetWindowHeight
static int lua_imgui_get_window_height(lua_State* L) {
    float height = ImGui::GetWindowHeight();
    lua_pushnumber(L, height);
    return 1;
}

// Lua binding for ImGui::GetWindowWidth
static int lua_imgui_get_window_width(lua_State* L) {
    float width = ImGui::GetWindowWidth();
    lua_pushnumber(L, width);
    return 1;
}

// Lua binding for ImGui::Checkbox
static int lua_imgui_checkbox(lua_State* L) {
    const char* label = luaL_checkstring(L, 1);
    bool checked = lua_toboolean(L, 2);
    bool changed = ImGui::Checkbox(label, &checked);
    lua_pushboolean(L, changed);
    lua_pushboolean(L, checked);
    return 2; // Return changed flag and new state
}

// Lua binding for ImGui::RadioButton
static int lua_imgui_radio_button(lua_State* L) {
    const char* label = luaL_checkstring(L, 1);
    bool active = lua_toboolean(L, 2);
    bool clicked = ImGui::RadioButton(label, active);
    lua_pushboolean(L, clicked);
    return 1;
}

// Lua binding for ImGui::ColorEdit3
static int lua_imgui_color_edit3(lua_State* L) {
    const char* label = luaL_checkstring(L, 1);
    float r = (float)luaL_checknumber(L, 2);
    float g = (float)luaL_checknumber(L, 3);
    float b = (float)luaL_checknumber(L, 4);
    float color[3] = {r, g, b};
    bool changed = ImGui::ColorEdit3(label, color);
    lua_pushboolean(L, changed);
    lua_pushnumber(L, color[0]);
    lua_pushnumber(L, color[1]);
    lua_pushnumber(L, color[2]);
    return 4; // Return changed flag and RGB values
}

// Lua binding for ImGui::SameLine
static int lua_imgui_same_line(lua_State* L) {
    float offset = luaL_optnumber(L, 1, 0.0f);
    float spacing = luaL_optnumber(L, 2, -1.0f);
    ImGui::SameLine(offset, spacing);
    return 0;
}

// Lua binding for ImGui::Separator
static int lua_imgui_separator(lua_State* L) {
    ImGui::Separator();
    return 0;
}

// Lua binding for ImGui::Spacing
static int lua_imgui_spacing(lua_State* L) {
    ImGui::Spacing();
    return 0;
}

// Lua binding for ImGui::ProgressBar
static int lua_imgui_progress_bar(lua_State* L) {
    float fraction = (float)luaL_checknumber(L, 1);
    float width = luaL_optnumber(L, 2, 0.0f);
    float height = luaL_optnumber(L, 3, 0.0f);
    const char* overlay = luaL_optstring(L, 4, nullptr);
    ImGui::ProgressBar(fraction, ImVec2(width, height), overlay);
    return 0;
}

// Lua binding for ImGui::Combo
static int lua_imgui_combo(lua_State* L) {
    const char* label = luaL_checkstring(L, 1);
    int current_item = (int)luaL_checkinteger(L, 2);
    luaL_checktype(L, 3, LUA_TTABLE); // Items must be a table
    // Get table length
    int item_count = luaL_len(L, 3);
    // Build items array
    std::vector<std::string> items(item_count);
    for (int i = 0; i < item_count; ++i) {
        lua_rawgeti(L, 3, i + 1); // Lua tables are 1-indexed
        items[i] = luaL_checkstring(L, -1);
        lua_pop(L, 1);
    }
    // Convert to C-style array of const char*
    std::vector<const char*> c_items(item_count);
    for (int i = 0; i < item_count; ++i) {
        c_items[i] = items[i].c_str();
    }
    bool changed = ImGui::Combo(label, &current_item, c_items.data(), item_count);
    lua_pushboolean(L, changed);
    lua_pushinteger(L, current_item);
    return 2; // Return changed flag and selected item index
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
    lua_pushcfunction(L, lua_imgui_input_text);
    lua_setfield(L, -2, "InputText");
    lua_pushcfunction(L, lua_imgui_input_text_multiline);
    lua_setfield(L, -2, "InputTextMultiline");
    lua_pushcfunction(L, lua_imgui_slider_float);
    lua_setfield(L, -2, "SliderFloat");
    lua_pushcfunction(L, lua_imgui_is_item_hovered);
    lua_setfield(L, -2, "IsItemHovered");
    lua_pushcfunction(L, lua_imgui_is_item_active);
    lua_setfield(L, -2, "IsItemActive");
    lua_pushcfunction(L, lua_imgui_is_item_clicked);
    lua_setfield(L, -2, "IsItemClicked");
    lua_pushcfunction(L, lua_imgui_get_window_height);
    lua_setfield(L, -2, "GetWindowHeight");
    lua_pushcfunction(L, lua_imgui_get_window_width);
    lua_setfield(L, -2, "GetWindowWidth");
    lua_pushcfunction(L, lua_imgui_checkbox);
    lua_setfield(L, -2, "Checkbox");
    lua_pushcfunction(L, lua_imgui_radio_button);
    lua_setfield(L, -2, "RadioButton");
    lua_pushcfunction(L, lua_imgui_color_edit3);
    lua_setfield(L, -2, "ColorEdit3");
    lua_pushcfunction(L, lua_imgui_same_line);
    lua_setfield(L, -2, "SameLine");
    lua_pushcfunction(L, lua_imgui_separator);
    lua_setfield(L, -2, "Separator");
    lua_pushcfunction(L, lua_imgui_spacing);
    lua_setfield(L, -2, "Spacing");
    lua_pushcfunction(L, lua_imgui_progress_bar);
    lua_setfield(L, -2, "ProgressBar");
    lua_pushcfunction(L, lua_imgui_combo);
    lua_setfield(L, -2, "Combo");
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
        return false;
    }
    lua_pop(L, 1);
    return true;
}

// Call Lua draw function each frame
void RunLuaDraw() {
    if (!L) return;
    lua_getglobal(L, "draw");
    if (!lua_isfunction(L, -1)) {
        fprintf(stderr, "draw is not a function (type: %d)\n", lua_type(L, -1));
        lua_pop(L, 1);
        return;
    }
    lua_pushcfunction(L, lua_error_handler);
    lua_insert(L, -2);
    if (lua_pcall(L, 0, 0, -2) != LUA_OK) {
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