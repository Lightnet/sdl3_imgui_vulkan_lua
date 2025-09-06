-- window progress bar
local progress = 0.5
local slider_value = 0.5

function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        -- Slider
        local slider_changed, new_value = ImGui.SliderFloat("Slider", slider_value, 0.0, 1.0)
        if slider_changed then
            slider_value = new_value
            progress = new_value
            print("Slider value: " .. slider_value)
        end
        -- progress bar
        ImGui.ProgressBar(progress, 0, 0, string.format("%.0f%%", progress * 100))
    end
    ImGui.End()
end