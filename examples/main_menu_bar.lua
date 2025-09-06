-- test main menu top level
local checkbox_state = false
function draw()
    local open = ImGui.Begin("Lua Test Window", true, ImGui.MenuBar)
    if open then
        if ImGui.BeginMenuBar() then
            if ImGui.BeginMenu("File") then
                if ImGui.MenuItem("New") then
                    print("Menu: New selected")
                end
                if ImGui.MenuItem("Open", "Ctrl+O", false, true) then
                    print("Menu: Open selected")
                end
                ImGui.Separator()
                if ImGui.MenuItem("Save") then
                    print("Menu: Save selected")
                end
                ImGui.EndMenu()
            end
            if ImGui.BeginMenu("Edit") then
                if ImGui.MenuItem("Cut") then
                    print("Menu: Cut selected")
                end
                if ImGui.MenuItem("Copy") then
                    print("Menu: Copy selected")
                end
                if ImGui.MenuItem("Paste", nil, false, checkbox_state) then
                    print("Menu: Paste selected")
                end
                ImGui.EndMenu()
            end
            ImGui.EndMenuBar()
        end
    end
    ImGui.End()

end