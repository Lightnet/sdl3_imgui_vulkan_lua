
local slider_float = 0.1

function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        ImGui.Text("Hello from Lua!")
        local changed, value = ImGui.SliderFloat("Slider", slider_float, 0.0, 1.0)
        if changed then
            print("Slider value: " .. value)
            slider_float = value
        end
        if ImGui.Button("Click Me") then
            print("Button clicked in Lua!")
        end
    end
    ImGui.End()
end