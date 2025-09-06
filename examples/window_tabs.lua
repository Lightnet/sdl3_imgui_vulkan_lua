-- window blank


function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        if ImGui.BeginTabBar("MyTabBar") then
            if ImGui.BeginTabItem("Input", true) then
                ImGui.Text("Input Controls")
                ImGui.EndTabItem()
            end
            if ImGui.BeginTabItem("Selection") then
                ImGui.Text("Selection Controls")
                ImGui.EndTabItem()
            end
            ImGui.EndTabBar()
        end
    end
    ImGui.End()
end