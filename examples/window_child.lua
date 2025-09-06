-- window child
local multi_text = "Initial\nmultiline\ntext"

function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        ImGui.Text("Input Controls")
        -- Child window for multiline input
        if ImGui.BeginChild("MultiTextChild", 200, 100, true, ImGui.ChildBorder) then
            local multi_changed, new_multi_text = ImGui.InputTextMultiline("Multiline Input", multi_text, 0, 80)
            if multi_changed then
                multi_text = new_multi_text
                print("Multiline text changed to: " .. multi_text)
            end
            if ImGui.IsItemHovered() then
                print("Multiline input hovered")
            end
        end
        ImGui.EndChild()
    end
    ImGui.End()
end