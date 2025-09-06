-- window tool tip

function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        -- Button with tooltip
        if ImGui.Button("Click Me") then
            print("Button clicked in Lua!")
        end
        if ImGui.IsItemHovered() then
            ImGui.BeginTooltip()
            ImGui.Text("Click this button to print a message!")
            ImGui.EndTooltip()
            print("Button hovered")
        end
    end
    ImGui.End()
end