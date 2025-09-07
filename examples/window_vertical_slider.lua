-- window blank
local slider_value = 0.5

function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        ImGui.Text("Vertical Slider and Bullet Points")
        ImGui.Separator()

        -- Linear vertical slider (default, omit flags)
        local vslider_changed, new_vslider_value = ImGui.VSliderFloat("Vertical Volume (Linear)", 20, 150, slider_value, 0.0, 1.0, "%.2f")
        if vslider_changed then
            slider_value = new_vslider_value
            print("Linear vertical slider value: " .. slider_value)
        end
        if ImGui.IsItemHovered() then
            ImGui.SetTooltip("Linear vertical slider (0-1)")
        end
        ImGui.Spacing()
        -- if ImGui.IsItemHovered() then
        --     ImGui.SetTooltip("Vertical slider for volume (0-1)")
        -- end
        -- ImGui.Spacing()

        -- Bullet points (list)
        ImGui.BulletText("Bullet point 2: " .. string.format("Formatted with value %.2f", slider_value))


    end
    ImGui.End()
end