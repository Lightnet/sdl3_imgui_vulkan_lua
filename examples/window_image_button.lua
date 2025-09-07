-- window blank


function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        ImGui.Text("ImageButton Demo")
        ImGui.Separator()
        local texture = ImGui.my_texture
        if texture then
            -- ImageButton (now with str_id first, texture second)
            local clicked = ImGui.ImageButton("##imagebtn", texture, 100, 100)
            if clicked then
                print("ImageButton clicked! (e.g., toggle action)")
                -- Example: Toggle a state
                checkbox_state = not checkbox_state
            end
            if ImGui.IsItemHovered() then
                ImGui.SetTooltip("Clickable image button (hover for info)")
            end
            if ImGui.IsItemActive() then
                ImGui.SetTooltip("Dragging the button...")
            end
        end
        

    end
    ImGui.End()
end