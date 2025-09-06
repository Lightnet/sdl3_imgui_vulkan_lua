-- window color picker

local color_r, color_g, color_b = 1.0, 0.5, 0.0

function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        -- Color picker
        local color_changed, new_r, new_g, new_b = ImGui.ColorEdit3("Color Picker", color_r, color_g, color_b, ImGui.NoAlpha)
        if color_changed then
            color_r, color_g, color_b = new_r, new_g, new_b
            print(string.format("Color changed to: R=%.2f, G=%.2f, B=%.2f", color_r, color_g, color_b))
        end
    end
    ImGui.End()
end