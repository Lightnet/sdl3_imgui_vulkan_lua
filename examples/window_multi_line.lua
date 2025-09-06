-- window Multiline text input
local multi_text = "Initial\nmultiline\ntext"

function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        local multi_changed, new_multi_text = ImGui.InputTextMultiline("Multiline Input", multi_text, 0, 100)
        if multi_changed then
            multi_text = new_multi_text
            print("Multiline text changed to: " .. multi_text)
        end
        if ImGui.IsItemHovered() then
            print("Multiline input hovered")
        end
    end
    ImGui.End()
end