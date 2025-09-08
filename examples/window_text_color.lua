-- window blank


function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        -- Simple red text
        ImGui.TextColored(1.0, 0.0, 0.0, 1.0, "This is red text!")

        -- Green text with alpha (semi-transparent)
        ImGui.TextColored(0.0, 1.0, 0.0, 0.7, "This is semi-transparent green text!")

        -- Dynamic: Use current Text style color but make it brighter
        local text_r, text_g, text_b, text_a = ImGui.GetStyleColor(ImGui.Col_Text)
        ImGui.TextColored(text_r * 1.2, text_g * 1.2, text_b * 1.2, text_a, "Dynamic colored text (brighter than default)!")

        -- Example with variables
        local my_r, my_g, my_b, my_a = 0.0, 0.5, 1.0, 1.0  -- Blue-ish
        ImGui.TextColored(my_r, my_g, my_b, my_a, "Blue text from variables")
    end
    ImGui.End()
end