-- window get color theme
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

        -- Button to apply a tweak based on current color (e.g., darken if too bright)
        if ImGui.Button("Tweak WindowBg (darken if bright)") then
            if current_r + current_g + current_b > 1.5 then  -- Rough "brightness" check
                local darker_r, darker_g, darker_b = current_r * 0.8, current_g * 0.8, current_b * 0.8
                -- To set it back, you'd use StyleCustom or similar (not implemented here)
                print("Would darken WindowBg to: " .. darker_r .. ", " .. darker_g .. ", " .. darker_b)
            end
        end
    end
    ImGui.End()
end