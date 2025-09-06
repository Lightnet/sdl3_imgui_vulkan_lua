
local slider_float = 0.1

function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        ImGui.Text("Hello from Lua!")
    end
    ImGui.End()
end