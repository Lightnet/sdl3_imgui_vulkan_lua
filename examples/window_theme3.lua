-- window get color theme
-- testing text color change.
local current_r, current_g, current_b, current_a = 0.0, 0.0, 0.0, 0.0

function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        -- Get current WindowBg color (ImGui.Col_WindowBg == 2)
        current_r, current_g, current_b, current_a = ImGui.GetStyleColor(ImGui.Col_WindowBg)
        ImGui.Text("Current WindowBg: R=" .. string.format("%.2f", current_r) .. 
                " G=" .. string.format("%.2f", current_g) .. 
                " B=" .. string.format("%.2f", current_b) .. 
                " A=" .. string.format("%.2f", current_a))

        -- Example: Get and use another color, e.g., Text color
        local text_r, text_g, text_b, text_a = ImGui.GetStyleColor(ImGui.Col_Text)
        ImGui.TextColored(text_r, text_g, text_b, text_a, "This uses current Text color!")

        local color_changed, new_r, new_g, new_b = ImGui.ColorEdit3("Color Picker", text_r, text_g, text_b, ImGui.NoAlpha)
        if color_changed then
            text_r, text_g, text_b = new_r, new_g, new_b
            local custom_theme = {
                [ImGui.Col_Text] = {r=text_r, g=text_g, b=text_b}
            }
            ImGui.StyleCustom(custom_theme)
        end

        -- Button to apply a tweak based on current color (e.g., darken if too bright)
        if ImGui.Button("Tweak WindowBg (darken if bright)") then
            if current_r + current_g + current_b > 1.5 then  -- Rough "brightness" check
                local darker_r, darker_g, darker_b = current_r * 0.8, current_g * 0.8, current_b * 0.8
                -- To set it back, you'd use StyleCustom or similar (not implemented here)
                print("Would darken WindowBg to: " .. darker_r .. ", " .. darker_g .. ", " .. darker_b)
                local custom_theme = {
                    [ImGui.Col_Text] = {r=darker_r, g=darker_g, b=darker_b}
                }
                ImGui.StyleCustom(custom_theme)
            end
        end
    end
    ImGui.End()
end