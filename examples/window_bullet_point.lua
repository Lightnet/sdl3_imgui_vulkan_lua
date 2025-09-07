-- window blank
local slider_value = 0.5

function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        ImGui.Text("Vertical Slider and Bullet Points")
        ImGui.Separator()

        -- -- Vertical slider (e.g., 20px wide, 150px tall)
        -- local vslider_changed, new_vslider_value = ImGui.VSliderFloat("Vertical Volume", 20, 150, slider_value, 0.0, 1.0, "%.2f")
        -- if vslider_changed then
        --     slider_value = new_vslider_value  -- Update the value
        --     print("Vertical slider value: " .. slider_value)
        -- end
        -- if ImGui.IsItemHovered() then
        --     ImGui.SetTooltip("Vertical slider for volume (0-1)")
        -- end
        -- ImGui.Spacing()

        -- Bullet points (list)
        ImGui.BulletText("Bullet point 1: Basic text")
        ImGui.BulletText("Bullet point 2: " .. string.format("Formatted with value %.2f", slider_value))
        ImGui.BulletText("Bullet point 3: Multi-line\nwith a newline")


    end
    ImGui.End()
end