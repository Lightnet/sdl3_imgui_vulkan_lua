# sdl3_imgui_vulkan_lua

# License: MIT

# Status:
- work in progress.

# Packages:
- sdl 3.2.22
- KhronosGroup/Vulkan-Headers
- KhronosGroup/Vulkan-Loader
- imgui 1.92
- lua 5.4.8

## Notes:
- There are other packages are needed to install depend on the OS.

# Goal:
  Convert imgui c++ to lua for easy create widgets. As for later use for other project.

# Information:
  This is c++ project to use CMake build. For SDL, imgui, vulkan and lua script.

  Using the lua script to build imgui widgets. The reason is not hard code imgui in c++ but in lua for easy access and change. Well be hard code back once refine UI.

  This project use github repo to build this project for easy compile.

# CMake:
  Use msys64 windows 64 bit.

  Using the SDL as base for easy cross platform inputs and others. As for the gpu is vullkan. But use vulkan header and vulkan loader to build vulkan for better update.

# Features:
- [ ] Variable Flags
  - [ ] work in progress
## window:
- [x] Begin
- [x] End
- [x] GetWindowHeight
- [x] GetWindowWidth
- [x] window child
## widgets:
- [x] Text
- [x] Button
- [x] InputText
- [x] InputTextMultiline
- [x] SliderFloat
- [x] Checkbox
- [x] RadioButton
- [x] ColorEdit3
- [x] ProgressBar
- [x] Combo
- [x] ListBox
- [x] TreeNode
- [x] TreePop
- [x] BeginTabBar
- [x] EndTabBar
- [x] BeginTabItem
- [x] EndTabItem
- [x] BeginTooltip
- [x] EndTooltip
### layout:
- [x] SameLine
- [x] Separator
- [x] Spacing
## events
- [x] IsItemHovered
- [x] IsItemActive
- [x] IsItemClicked
## menu:
- [x] BeginMenuBar
- [x] EndMenuBar
- [x] BeginMenu
- [x] EndMenu
- [x] MenuItem
- [x] BeginMainMenuBar
##
- [ ] 
- [ ] 

# imgui lua:
  The design will be simple to match function but need to translate to c++ to lua which work differently. Still work the same some degree but to lua to c++ will take some time to translate correctly.

  Using the lua script to create imgui widgets is easy without need to compile all and save time depend on the project.

## flags:
 Note this is work in progress test.
```lua
local bit = require("bit32")
local flags = bit.bor(ImGui.ImGuiComboFlags_PopupAlignLeft, ImGui.ImGuiComboFlags_HeightSmall)
```


# Credits:
- https://github.com/ocornut/imgui
- https://github.com/sonoro1234/LuaJIT-ImGui
- https://github.com/ocornut/imgui/wiki/Bindings
- https://github.com/ThePhD/sol2
- 
- 
