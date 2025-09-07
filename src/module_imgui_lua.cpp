// module_imgui_lua.cpp
#include "module_imgui_lua.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
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
// static lua_State* L = nullptr;
lua_State* L = nullptr;  // Now it's a global definition with external linkage

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
// ImGuiWindowFlags_MenuBar
// Lua binding for ImGui::Begin
static int lua_imgui_begin(lua_State* L) {
    const char* name = luaL_checkstring(L, 1);
    bool open = true;
    if (lua_isboolean(L, 2)) {
        open = lua_toboolean(L, 2);
    }
    int flags = 0;
    if (lua_gettop(L) >= 3) {
        if (lua_istable(L, 3)) {
            // Handle flags as a table
            int len = luaL_len(L, 3);
            for (int i = 1; i <= len; ++i) {
                lua_rawgeti(L, 3, i);
                if (lua_isnumber(L, -1)) {
                    flags |= lua_tointeger(L, -1);
                }
                lua_pop(L, 1);
            }
            // printf("ImGui::Begin flags (table): %d\n", flags); // Debug output
        } else if (lua_isnumber(L, 3)) {
            // Handle single integer flag
            flags = luaL_optinteger(L, 3, 0);
            // printf("ImGui::Begin flags (single): %d\n", flags); // Debug output
        }
    }
    // printf("default ImGui::Begin flags (single): %d\n", flags); // Debug output
    bool result = ImGui::Begin(name, &open, flags);
    lua_pushboolean(L, result);
    lua_pushboolean(L, open);
    return 2;
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
    int flags = luaL_optinteger(L, 3, 0);
    char buffer[256] = {0};
    strncpy(buffer, initial_text, sizeof(buffer) - 1);
    bool changed = ImGui::InputText(label, buffer, sizeof(buffer), flags);
    lua_pushboolean(L, changed);
    lua_pushstring(L, buffer);
    return 2;
}

// Lua binding for ImGui::InputTextMultiline
static int lua_imgui_input_text_multiline(lua_State* L) {
    const char* label = luaL_checkstring(L, 1);
    const char* initial_text = luaL_checkstring(L, 2);
    float width = luaL_optnumber(L, 3, 0.0f);
    float height = luaL_optnumber(L, 4, 0.0f);
    int flags = luaL_optinteger(L, 5, 0);
    char buffer[1024] = {0};
    strncpy(buffer, initial_text, sizeof(buffer) - 1);
    bool changed = ImGui::InputTextMultiline(label, buffer, sizeof(buffer), ImVec2(width, height), flags);
    lua_pushboolean(L, changed);
    lua_pushstring(L, buffer);
    return 2;
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
    return 2;
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
    int flags = luaL_optinteger(L, 5, 0);
    float color[3] = {r, g, b};
    bool changed = ImGui::ColorEdit3(label, color, flags);
    lua_pushboolean(L, changed);
    lua_pushnumber(L, color[0]);
    lua_pushnumber(L, color[1]);
    lua_pushnumber(L, color[2]);
    return 4;
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
    luaL_checktype(L, 3, LUA_TTABLE);
    int flags = luaL_optinteger(L, 4, 0);
    int item_count = luaL_len(L, 3);
    std::vector<std::string> items(item_count);
    for (int i = 0; i < item_count; ++i) {
        lua_rawgeti(L, 3, i + 1);
        items[i] = luaL_checkstring(L, -1);
        lua_pop(L, 1);
    }
    std::vector<const char*> c_items(item_count);
    for (int i = 0; i < item_count; ++i) {
        c_items[i] = items[i].c_str();
    }
    bool changed = ImGui::Combo(label, &current_item, c_items.data(), item_count, flags);
    lua_pushboolean(L, changed);
    lua_pushinteger(L, current_item);
    return 2;
}

// Lua binding for ImGui::ListBox
static int lua_imgui_list_box(lua_State* L) {
    const char* label = luaL_checkstring(L, 1);
    int current_item = (int)luaL_checkinteger(L, 2);
    luaL_checktype(L, 3, LUA_TTABLE);
    int item_count = luaL_len(L, 3);
    std::vector<std::string> items(item_count);
    for (int i = 0; i < item_count; ++i) {
        lua_rawgeti(L, 3, i + 1);
        items[i] = luaL_checkstring(L, -1);
        lua_pop(L, 1);
    }
    std::vector<const char*> c_items(item_count);
    for (int i = 0; i < item_count; ++i) {
        c_items[i] = items[i].c_str();
    }
    bool changed = ImGui::ListBox(label, &current_item, c_items.data(), item_count, -1);
    lua_pushboolean(L, changed);
    lua_pushinteger(L, current_item);
    return 2;
}

// Lua binding for ImGui::TreeNode
static int lua_imgui_tree_node(lua_State* L) {
    const char* label = luaL_checkstring(L, 1);
    int flags = luaL_optinteger(L, 2, 0);
    bool open = ImGui::TreeNodeEx(label, flags);
    lua_pushboolean(L, open);
    return 1;
}

// Lua binding for ImGui::TreePop
static int lua_imgui_tree_pop(lua_State* L) {
    ImGui::TreePop();
    return 0;
}

// Lua binding for ImGui::BeginTabBar
static int lua_imgui_begin_tab_bar(lua_State* L) {
    const char* str_id = luaL_checkstring(L, 1);
    int flags = luaL_optinteger(L, 2, 0);
    bool open = ImGui::BeginTabBar(str_id, flags);
    lua_pushboolean(L, open);
    return 1;
}

// Lua binding for ImGui::EndTabBar
static int lua_imgui_end_tab_bar(lua_State* L) {
    ImGui::EndTabBar();
    return 0;
}

// Lua binding for ImGui::BeginTabItem
static int lua_imgui_begin_tab_item(lua_State* L) {
    const char* label = luaL_checkstring(L, 1);
    bool open = true;
    if (lua_isboolean(L, 2)) {
        open = lua_toboolean(L, 2);
    }
    int flags = luaL_optinteger(L, 3, 0);
    bool selected = ImGui::BeginTabItem(label, &open, flags);
    lua_pushboolean(L, selected);
    lua_pushboolean(L, open);
    return 2;
}

// Lua binding for ImGui::EndTabItem
static int lua_imgui_end_tab_item(lua_State* L) {
    ImGui::EndTabItem();
    return 0;
}

// Lua binding for ImGui::BeginMenuBar
static int lua_imgui_begin_menu_bar(lua_State* L) {
    bool open = ImGui::BeginMenuBar();
    lua_pushboolean(L, open);
    return 1;
}

// Lua binding for ImGui::EndMenuBar
static int lua_imgui_end_menu_bar(lua_State* L) {
    ImGui::EndMenuBar();
    return 0;
}

// Lua binding for ImGui::BeginMainMenuBar
static int lua_imgui_begin_main_menu_bar(lua_State* L) {
    bool open = ImGui::BeginMainMenuBar();
    lua_pushboolean(L, open);
    return 1;
}

// Lua binding for ImGui::EndMainMenuBar
static int lua_imgui_end_main_menu_bar(lua_State* L) {
    ImGui::EndMainMenuBar();
    return 0;
}

// Lua binding for ImGui::BeginMenu
static int lua_imgui_begin_menu(lua_State* L) {
    const char* label = luaL_checkstring(L, 1);
    bool enabled = true;
    if (lua_gettop(L) >= 2 && !lua_isnil(L, 2)) {
        enabled = lua_toboolean(L, 2);
    }
    bool open = ImGui::BeginMenu(label, enabled);
    lua_pushboolean(L, open);
    return 1;
}

// Lua binding for ImGui::EndMenu
static int lua_imgui_end_menu(lua_State* L) {
    ImGui::EndMenu();
    return 0;
}

// Lua binding for ImGui::MenuItem
static int lua_imgui_menu_item(lua_State* L) {
    const char* label = luaL_checkstring(L, 1);
    const char* shortcut = luaL_optstring(L, 2, nullptr);
    bool selected = false;
    if (lua_gettop(L) >= 3 && !lua_isnil(L, 3)) {
        selected = lua_toboolean(L, 3);
    }
    bool enabled = true;
    if (lua_gettop(L) >= 4 && !lua_isnil(L, 4)) {
        enabled = lua_toboolean(L, 4);
    }
    bool activated = ImGui::MenuItem(label, shortcut, selected, enabled);
    lua_pushboolean(L, activated);
    return 1;
}

// Lua binding for ImGui::BeginTooltip
static int lua_imgui_begin_tooltip(lua_State* L) {
    bool result = ImGui::BeginTooltip();
    lua_pushboolean(L, result);
    return 1;
}

// Lua binding for ImGui::EndTooltip
static int lua_imgui_end_tooltip(lua_State* L) {
    ImGui::EndTooltip();
    return 0;
}

// Lua binding for ImGui::BeginChild (supports string ID)
static int lua_imgui_begin_child(lua_State* L) {
    const char* str_id = luaL_checkstring(L, 1);
    float size_x = luaL_optnumber(L, 2, 0.0f);
    float size_y = luaL_optnumber(L, 3, 0.0f);
    bool border = lua_toboolean(L, 4);
    int flags = 0;
    if (lua_gettop(L) >= 5) {
        if (lua_istable(L, 5)) {
            int len = luaL_len(L, 5);
            for (int i = 1; i <= len; ++i) {
                lua_rawgeti(L, 5, i);
                if (lua_isnumber(L, -1)) {
                    flags |= lua_tointeger(L, -1);
                }
                lua_pop(L, 1);
            }
            // printf("ImGui::BeginChild flags (table): %d\n", flags);
        } else if (lua_isnumber(L, 5)) {
            flags = luaL_optinteger(L, 5, 0);
            // printf("ImGui::BeginChild flags (single): %d\n", flags);
        }
    }
    bool result = ImGui::BeginChild(str_id, ImVec2(size_x, size_y), border, flags);
    lua_pushboolean(L, result);
    return 1;
}

// Lua binding for ImGui::EndChild
static int lua_imgui_end_child(lua_State* L) {
    ImGui::EndChild();
    return 0;
}

// Lua binding for ImGui::PlotLines
static int lua_imgui_plot_lines(lua_State* L) {
    const char* label = luaL_checkstring(L, 1);
    luaL_checktype(L, 2, LUA_TTABLE);
    const char* overlay_text = luaL_optstring(L, 3, nullptr);
    float scale_min = luaL_optnumber(L, 4, FLT_MAX);
    float scale_max = luaL_optnumber(L, 5, FLT_MAX);
    float graph_width = luaL_optnumber(L, 6, 0.0f);
    float graph_height = luaL_optnumber(L, 7, 0.0f);

    // Get values from Lua table
    int value_count = luaL_len(L, 2);
    std::vector<float> values(value_count);
    for (int i = 0; i < value_count; ++i) {
        lua_rawgeti(L, 2, i + 1);
        if (lua_isnumber(L, -1)) {
            values[i] = (float)lua_tonumber(L, -1);
        } else {
            values[i] = 0.0f;
        }
        lua_pop(L, 1);
    }

    ImGui::PlotLines(label, values.data(), value_count, 0, overlay_text, scale_min, scale_max, ImVec2(graph_width, graph_height));
    return 0;
}

// Lua binding for ImGui::PlotHistogram
static int lua_imgui_plot_histogram(lua_State* L) {
    const char* label = luaL_checkstring(L, 1);
    luaL_checktype(L, 2, LUA_TTABLE);
    const char* overlay_text = luaL_optstring(L, 3, nullptr);
    float scale_min = luaL_optnumber(L, 4, FLT_MAX);
    float scale_max = luaL_optnumber(L, 5, FLT_MAX);
    float graph_width = luaL_optnumber(L, 6, 0.0f);
    float graph_height = luaL_optnumber(L, 7, 0.0f);

    // Get values from Lua table
    int value_count = luaL_len(L, 2);
    std::vector<float> values(value_count);
    for (int i = 0; i < value_count; ++i) {
        lua_rawgeti(L, 2, i + 1);
        if (lua_isnumber(L, -1)) {
            values[i] = (float)lua_tonumber(L, -1);
        } else {
            values[i] = 0.0f;
        }
        lua_pop(L, 1);
    }

    ImGui::PlotHistogram(label, values.data(), value_count, 0, overlay_text, scale_min, scale_max, ImVec2(graph_width, graph_height));
    return 0;
}

// Lua binding for ImGui::BeginTable
static int lua_imgui_begin_table(lua_State* L) {
    const char* str_id = luaL_checkstring(L, 1);
    int column = luaL_checkinteger(L, 2);
    int flags = 0;
    if (lua_gettop(L) >= 3) {
        if (lua_istable(L, 3)) {
            int len = luaL_len(L, 3);
            for (int i = 1; i <= len; ++i) {
                lua_rawgeti(L, 3, i);
                if (lua_isnumber(L, -1)) {
                    flags |= lua_tointeger(L, -1);
                }
                lua_pop(L, 1);
            }
            printf("ImGui::BeginTable flags (table): %d\n", flags);
        } else if (lua_isnumber(L, 3)) {
            flags = luaL_optinteger(L, 3, 0);
            printf("ImGui::BeginTable flags (single): %d\n", flags);
        }
    }
    float outer_size_x = luaL_optnumber(L, 4, 0.0f);
    float outer_size_y = luaL_optnumber(L, 5, 0.0f);
    bool result = ImGui::BeginTable(str_id, column, flags, ImVec2(outer_size_x, outer_size_y));
    lua_pushboolean(L, result);
    return 1;
}

// Lua binding for ImGui::EndTable
static int lua_imgui_end_table(lua_State* L) {
    ImGui::EndTable();
    return 0;
}

// Lua binding for ImGui::TableNextRow
static int lua_imgui_table_next_row(lua_State* L) {
    float row_min_height = luaL_optnumber(L, 1, 0.0f);
    ImGui::TableNextRow(0, row_min_height);
    return 0;
}

// Lua binding for ImGui::TableNextColumn
static int lua_imgui_table_next_column(lua_State* L) {
    bool result = ImGui::TableNextColumn();
    lua_pushboolean(L, result);
    return 1;
}

// Lua binding for ImGui::TableSetColumnIndex
static int lua_imgui_table_set_column_index(lua_State* L) {
    int column_n = luaL_checkinteger(L, 1);
    bool result = ImGui::TableSetColumnIndex(column_n);
    lua_pushboolean(L, result);
    return 1;
}

// Lua binding for ImGui::Columns
static int lua_imgui_columns(lua_State* L) {
    int count = luaL_checkinteger(L, 1);
    const char* str_id = luaL_optstring(L, 2, nullptr);
    bool border = lua_toboolean(L, 3);
    ImGui::Columns(count, str_id, border);
    return 0;
}

// Lua binding for ImGui::NextColumn
static int lua_imgui_next_column(lua_State* L) {
    ImGui::NextColumn();
    return 0;
}

// Lua binding for ImGui::SetColumnWidth
static int lua_imgui_set_column_width(lua_State* L) {
    int column_index = luaL_checkinteger(L, 1);
    float width = luaL_checknumber(L, 2);
    ImGui::SetColumnWidth(column_index, width);
    return 0;
}

// Lua binding for ImGui::SetColumnOffset
static int lua_imgui_set_column_offset(lua_State* L) {
    int column_index = luaL_checkinteger(L, 1);
    float offset_x = luaL_checknumber(L, 2);
    ImGui::SetColumnOffset(column_index, offset_x);
    return 0;
}

// Lua binding for ImGui::Image
static int lua_imgui_image(lua_State* L) {
    // Get texture_id as light userdata (pointer to VkDescriptorSet)
    ImTextureID texture_id = (ImTextureID)lua_touserdata(L, 1);  // Cast to ImTextureID
    if (!texture_id) {
        luaL_error(L, "Invalid texture_id (must be userdata/pointer)");
        return 0;
    }

    float size_x = (float)luaL_checknumber(L, 2);
    float size_y = (float)luaL_checknumber(L, 3);

    // Optional UV0 (defaults: 0,0)
    float uv0_x = luaL_optnumber(L, 4, 0.0f);
    float uv0_y = luaL_optnumber(L, 5, 0.0f);

    // Optional UV1 (defaults: 1,1)
    float uv1_x = luaL_optnumber(L, 6, 1.0f);
    float uv1_y = luaL_optnumber(L, 7, 1.0f);

    // Optional tint_col (ImVec4, defaults: 1,1,1,1 white)
    float tint_r = luaL_optnumber(L, 8, 1.0f);
    float tint_g = luaL_optnumber(L, 9, 1.0f);
    float tint_b = luaL_optnumber(L, 10, 1.0f);
    float tint_a = luaL_optnumber(L, 11, 1.0f);

    // Optional border_col (ImVec4, defaults: 0,0,0,0 transparent)
    float border_r = luaL_optnumber(L, 12, 0.0f);
    float border_g = luaL_optnumber(L, 13, 0.0f);
    float border_b = luaL_optnumber(L, 14, 0.0f);
    float border_a = luaL_optnumber(L, 15, 0.0f);

    ImGui::Image(texture_id, ImVec2(size_x, size_y),
                 ImVec2(uv0_x, uv0_y), ImVec2(uv1_x, uv1_y),
                 ImVec4(tint_r, tint_g, tint_b, tint_a),
                 ImVec4(border_r, border_g, border_b, border_a));
    return 0;
}

// Lua binding for ImGui::SetTooltip (converts args to strings and concatenates)
static int lua_imgui_set_tooltip(lua_State* L) {
    // First argument: base format/message (string)
    const char* base = luaL_checkstring(L, 1);
    
    // Get number of additional args
    int n = lua_gettop(L) - 1;
    
    // Build tooltip string by concatenating base + converted args
    std::string tooltip = std::string(base);  // Start with base
    for (int i = 2; i <= n + 1; ++i) {  // Args start at index 2
        // Convert arg to string (handles number, bool, nil, etc.)
        const char* arg_str = luaL_tolstring(L, i, nullptr);  // Pushes converted string to stack temporarily
        if (arg_str) {
            tooltip += " ";  // Simple separator (customize if needed)
            tooltip += arg_str;
            lua_pop(L, 1);  // Pop the temporary string from stack
        }
    }
    
    // Call ImGui::SetTooltip with the concatenated string
    ImGui::SetTooltip("%s", tooltip.c_str());
    
    return 0;
}

// Lua binding for ImGui::VSliderFloat (modern API with flags support)
static int lua_imgui_vslider_float(lua_State* L) {
    const char* label = luaL_checkstring(L, 1);
    float size_x = (float)luaL_checknumber(L, 2);
    float size_y = (float)luaL_checknumber(L, 3);
    float value = (float)luaL_checknumber(L, 4);
    float min = (float)luaL_checknumber(L, 5);
    float max = (float)luaL_checknumber(L, 6);
    const char* format = luaL_optstring(L, 7, "%.3f");
    
    // Handle flags as optional table (like Begin binding)
    ImGuiSliderFlags flags = 0;
    if (lua_gettop(L) >= 8) {
        if (lua_istable(L, 8)) {
            // Parse table of flags (e.g., {ImGuiSliderFlags_Logarithmic})
            int len = luaL_len(L, 8);
            for (int i = 1; i <= len; ++i) {
                lua_rawgeti(L, 8, i);
                if (lua_isnumber(L, -1)) {
                    flags |= (ImGuiSliderFlags)lua_tointeger(L, -1);
                }
                lua_pop(L, 1);
            }
        } else if (lua_isnumber(L, 8)) {
            // Single integer flag
            flags = (ImGuiSliderFlags)luaL_optinteger(L, 8, 0);
        }
    }
    
    bool changed = ImGui::VSliderFloat(label, ImVec2(size_x, size_y), &value, min, max, format, flags);
    lua_pushboolean(L, changed);
    lua_pushnumber(L, value);
    return 2;
}

// Lua binding for ImGui::BulletText (simple string; use string.format in Lua for advanced fmt)
static int lua_imgui_bullet_text(lua_State* L) {
    const char* text = luaL_checkstring(L, 1);
    ImGui::BulletText("%s", text);
    return 0;
}


// Lua binding for ImGui::ImageButton (corrected with str_id first)
static int lua_imgui_image_button(lua_State* L) {
    // First: str_id (required string for ImGui ID)
    const char* str_id = luaL_checkstring(L, 1);

    // Second: texture_id as light userdata (pointer to VkDescriptorSet / ImTextureID)
    ImTextureID texture_id = (ImTextureID)lua_touserdata(L, 2);
    if (!texture_id) {
        luaL_error(L, "Invalid texture_id (must be userdata/pointer at arg 2)");
        return 0;
    }

    // Third/Fourth: size_x, size_y (image_size)
    float size_x = (float)luaL_checknumber(L, 3);
    float size_y = (float)luaL_checknumber(L, 4);

    // Optional UV0 (defaults: 0,0)
    float uv0_x = luaL_optnumber(L, 5, 0.0f);
    float uv0_y = luaL_optnumber(L, 6, 0.0f);

    // Optional UV1 (defaults: 1,1)
    float uv1_x = luaL_optnumber(L, 7, 1.0f);
    float uv1_y = luaL_optnumber(L, 8, 1.0f);

    // Optional bg_col (ImVec4, defaults: transparent 0,0,0,0)
    float bg_r = luaL_optnumber(L, 9, 0.0f);
    float bg_g = luaL_optnumber(L, 10, 0.0f);
    float bg_b = luaL_optnumber(L, 11, 0.0f);
    float bg_a = luaL_optnumber(L, 12, 0.0f);

    // Optional tint_col (ImVec4, defaults: white 1,1,1,1)
    float tint_r = luaL_optnumber(L, 13, 1.0f);
    float tint_g = luaL_optnumber(L, 14, 1.0f);
    float tint_b = luaL_optnumber(L, 15, 1.0f);
    float tint_a = luaL_optnumber(L, 16, 1.0f);

    bool clicked = ImGui::ImageButton(str_id, texture_id, ImVec2(size_x, size_y),
                                      ImVec2(uv0_x, uv0_y), ImVec2(uv1_x, uv1_y),
                                      ImVec4(bg_r, bg_g, bg_b, bg_a),
                                      ImVec4(tint_r, tint_g, tint_b, tint_a));
    lua_pushboolean(L, clicked);
    return 1;
}


// Initialize Lua and load script.lua
bool InitLua(const char* script_file) {
    L = luaL_newstate();
    if (!L) {
        fprintf(stderr, "Failed to create Lua state\n");
        return false;
    }
    luaL_openlibs(L); // Open standard Lua libraries

    // Create ImGui table
    lua_newtable(L);

    // Register ImGui functions
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
    lua_pushcfunction(L, lua_imgui_list_box);
    lua_setfield(L, -2, "ListBox");
    lua_pushcfunction(L, lua_imgui_tree_node);
    lua_setfield(L, -2, "TreeNode");
    lua_pushcfunction(L, lua_imgui_tree_pop);
    lua_setfield(L, -2, "TreePop");
    lua_pushcfunction(L, lua_imgui_begin_tab_bar);
    lua_setfield(L, -2, "BeginTabBar");
    lua_pushcfunction(L, lua_imgui_end_tab_bar);
    lua_setfield(L, -2, "EndTabBar");
    lua_pushcfunction(L, lua_imgui_begin_tab_item);
    lua_setfield(L, -2, "BeginTabItem");
    lua_pushcfunction(L, lua_imgui_end_tab_item);
    lua_setfield(L, -2, "EndTabItem");
    lua_pushcfunction(L, lua_imgui_begin_menu_bar);
    lua_setfield(L, -2, "BeginMenuBar");
    lua_pushcfunction(L, lua_imgui_end_menu_bar);
    lua_setfield(L, -2, "EndMenuBar");
    lua_pushcfunction(L, lua_imgui_begin_main_menu_bar);
    lua_setfield(L, -2, "BeginMainMenuBar");
    lua_pushcfunction(L, lua_imgui_end_main_menu_bar);
    lua_setfield(L, -2, "EndMainMenuBar");
    lua_pushcfunction(L, lua_imgui_begin_menu);
    lua_setfield(L, -2, "BeginMenu");
    lua_pushcfunction(L, lua_imgui_end_menu);
    lua_setfield(L, -2, "EndMenu");
    lua_pushcfunction(L, lua_imgui_menu_item);
    lua_setfield(L, -2, "MenuItem");
    lua_pushcfunction(L, lua_imgui_begin_tooltip);
    lua_setfield(L, -2, "BeginTooltip");
    lua_pushcfunction(L, lua_imgui_end_tooltip);
    lua_setfield(L, -2, "EndTooltip");
    lua_pushcfunction(L, lua_imgui_begin_child);
    lua_setfield(L, -2, "BeginChild");
    lua_pushcfunction(L, lua_imgui_end_child);
    lua_setfield(L, -2, "EndChild");
    lua_pushcfunction(L, lua_imgui_plot_lines);
    lua_setfield(L, -2, "PlotLines");
    lua_pushcfunction(L, lua_imgui_plot_histogram);
    lua_setfield(L, -2, "PlotHistogram");
    lua_pushcfunction(L, lua_imgui_begin_table);
    lua_setfield(L, -2, "BeginTable");
    lua_pushcfunction(L, lua_imgui_end_table);
    lua_setfield(L, -2, "EndTable");
    lua_pushcfunction(L, lua_imgui_table_next_row);
    lua_setfield(L, -2, "TableNextRow");
    lua_pushcfunction(L, lua_imgui_table_next_column);
    lua_setfield(L, -2, "TableNextColumn");
    lua_pushcfunction(L, lua_imgui_table_set_column_index);
    lua_setfield(L, -2, "TableSetColumnIndex");
    lua_pushcfunction(L, lua_imgui_columns);
    lua_setfield(L, -2, "Columns");
    lua_pushcfunction(L, lua_imgui_next_column);
    lua_setfield(L, -2, "NextColumn");
    lua_pushcfunction(L, lua_imgui_set_column_width);
    lua_setfield(L, -2, "SetColumnWidth");
    lua_pushcfunction(L, lua_imgui_set_column_offset);
    lua_setfield(L, -2, "SetColumnOffset");
    lua_pushcfunction(L, lua_imgui_image);
    lua_setfield(L, -2, "Image");
    lua_pushcfunction(L, lua_imgui_set_tooltip);
    lua_setfield(L, -2, "SetTooltip");
    lua_pushcfunction(L, lua_imgui_vslider_float);
    lua_setfield(L, -2, "VSliderFloat");
    lua_pushcfunction(L, lua_imgui_bullet_text);
    lua_setfield(L, -2, "BulletText");

    lua_pushcfunction(L, lua_imgui_image_button);
    lua_setfield(L, -2, "ImageButton");

    // Register ImGuiInputTextFlags
    lua_pushinteger(L, ImGuiInputTextFlags_EnterReturnsTrue);
    lua_setfield(L, -2, "EnterReturnsTrue");
    lua_pushinteger(L, ImGuiInputTextFlags_ReadOnly);
    lua_setfield(L, -2, "ReadOnly");

    // Register ImGuiColorEditFlags
    lua_pushinteger(L, ImGuiColorEditFlags_NoAlpha);
    lua_setfield(L, -2, "NoAlpha");
    lua_pushinteger(L, ImGuiColorEditFlags_HDR);
    lua_setfield(L, -2, "HDR");

    // Register ImGuiComboFlags
    lua_pushinteger(L, ImGuiComboFlags_PopupAlignLeft);
    lua_setfield(L, -2, "PopupAlignLeft");
    lua_pushinteger(L, ImGuiComboFlags_HeightSmall);
    lua_setfield(L, -2, "HeightSmall");

    // Register ImGuiTreeNodeFlags
    lua_pushinteger(L, ImGuiTreeNodeFlags_DefaultOpen);
    lua_setfield(L, -2, "DefaultOpen");
    lua_pushinteger(L, ImGuiTreeNodeFlags_Selected);
    lua_setfield(L, -2, "Selected");

    // Register ImGuiTabBarFlags
    lua_pushinteger(L, ImGuiTabBarFlags_Reorderable);
    lua_setfield(L, -2, "Reorderable");

    // Register ImGuiTabItemFlags
    lua_pushinteger(L, ImGuiTabItemFlags_SetSelected);
    lua_setfield(L, -2, "SetSelected");

    // Register ImGuiWindowFlags
    lua_pushinteger(L, ImGuiWindowFlags_NoTitleBar);
    lua_setfield(L, -2, "NoTitleBar");
    lua_pushinteger(L, ImGuiWindowFlags_NoResize);
    lua_setfield(L, -2, "NoResize");
    lua_pushinteger(L, ImGuiWindowFlags_NoMove);
    lua_setfield(L, -2, "NoMove");
    lua_pushinteger(L, ImGuiWindowFlags_MenuBar);
    lua_setfield(L, -2, "MenuBar");
    lua_pushinteger(L, ImGuiWindowFlags_NoCollapse);
    lua_setfield(L, -2, "NoCollapse");
    lua_pushinteger(L, ImGuiWindowFlags_NoScrollbar);
    lua_setfield(L, -2, "NoScrollbar");
    lua_pushinteger(L, ImGuiWindowFlags_AlwaysAutoResize);
    lua_setfield(L, -2, "AlwaysAutoResize");

    // Register ImGuiChildFlags
    lua_pushinteger(L, ImGuiChildFlags_Border);
    lua_setfield(L, -2, "ChildBorder");
    lua_pushinteger(L, ImGuiChildFlags_AlwaysAutoResize);
    lua_setfield(L, -2, "ChildAlwaysAutoResize");

    // Register ImGuiTableFlags
    lua_pushinteger(L, ImGuiTableFlags_Resizable);
    lua_setfield(L, -2, "TableResizable");
    lua_pushinteger(L, ImGuiTableFlags_Borders);
    lua_setfield(L, -2, "TableBorders");
    lua_pushinteger(L, ImGuiTableFlags_SizingStretchSame);
    lua_setfield(L, -2, "TableSizingStretchSame");

    // Register ImGuiSliderFlags
    // lua_pushinteger(L, ImGuiSliderFlags_AlwaysExpose); //does not exist
    // lua_setfield(L, -2, "SliderFlags_AlwaysExpose");
    lua_pushinteger(L, ImGuiSliderFlags_NoRoundToFormat);
    lua_setfield(L, -2, "SliderFlags_NoRoundToFormat");
    lua_pushinteger(L, ImGuiSliderFlags_NoInput);
    lua_setfield(L, -2, "SliderFlags_NoInput");
    lua_pushinteger(L, ImGuiSliderFlags_InvalidMask_);  // For reference (internal mask)
    lua_setfield(L, -2, "SliderFlags_InvalidMask");
    lua_pushinteger(L, ImGuiSliderFlags_Logarithmic);
    lua_setfield(L, -2, "SliderFlags_Logarithmic");  // Key for logarithmic (replaces old 'power')


    // Set ImGui table as global
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