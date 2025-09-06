-- window input
local text = "Initial text"

function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        local changed, new_text = ImGui.InputText("Text Input", text, ImGui.EnterReturnsTrue)
        if changed then
            text = new_text
            print("Text changed to: " .. text)
        end
    end
    ImGui.End()
end