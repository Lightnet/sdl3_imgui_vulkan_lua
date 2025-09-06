-- script.lua
-- test window menu
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
                if ImGui.MenuItem("Paste") then
                    print("Menu: Paste selected")
                end
                if ImGui.MenuItem("Pastess") then
                    print("Menu: Paste selected")
                end
                ImGui.EndMenu()
            end
            ImGui.EndMenuBar()
        end

        -- Window content (matching C++ test)
        ImGui.Text("This is the window content.")

        -- Window dimensions (optional, for debugging)
        local width = ImGui.GetWindowWidth()
        local height = ImGui.GetWindowHeight()
        ImGui.Text("Window size: " .. width .. "x" .. height)
    end
    ImGui.End()
end