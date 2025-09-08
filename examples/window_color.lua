-- window color

local my_r, my_g, my_b, my_a = 1.0, 0.5, 0.0, 1.0
local picker_r, picker_g, picker_b = 0.0, 1.0, 0.0
local button_clicked = false

function draw()
    local open = ImGui.Begin("Color Tools", true)
    if open then
        -- ColorEdit4: RGBA editor (label, r, g, b, a, [flags])
        local changed, r, g, b, a = ImGui.ColorEdit4("My Color RGBA", my_r, my_g, my_b, my_a, ImGui.ColorEditFlags_NoAlpha)
        if changed then
            my_r, my_g, my_b, my_a = r, g, b, a
            print("RGBA changed to: " .. r .. ", " .. g .. ", " .. b .. ", " .. a)
        end

        -- ColorPicker3: RGB picker (label, r, g, b, [flags])
        local picker_changed, pr, pg, pb = ImGui.ColorPicker3("My Picker RGB", picker_r, picker_g, picker_b, {ImGui.ColorEditFlags_HDR, ImGui.ColorEditFlags_PickerHueWheel})
        if picker_changed then
            picker_r, picker_g, picker_b = pr, pg, pb
            print("RGB picked: " .. pr .. ", " .. pg .. ", " .. pb)
        end

        -- ColorPicker4: RGBA picker (label, r, g, b, a, [flags])
        local picker4_changed, p4r, p4g, p4b, p4a = ImGui.ColorPicker4("My Picker RGBA", my_r, my_g, my_b, my_a)
        if picker4_changed then
            my_r, my_g, my_b, my_a = p4r, p4g, p4b, p4a
        end

        -- ColorButton: Color swatch button (id, r, g, b, [a=1.0], [size_x=0, size_y=0], [flags])
        button_clicked = ImGui.ColorButton("ColorBtn##1", my_r, my_g, my_b, my_a, 50.0, 20.0, ImGui.ColorEditFlags_AlphaPreview)
        if button_clicked then
            print("Color button clicked! Current color: " .. my_r .. ", " .. my_g .. ", " .. my_b .. ", " .. my_a)
        end
    end
    ImGui.End()
end