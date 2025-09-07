-- window blank


function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        local texture = ImGui.my_texture  -- This is the light userdata
        if texture then
            -- Basic image display (200x200 size, default UV/tint/border)
            ImGui.Image(texture, 200, 200)

            -- Or with custom params (e.g., full UV, white tint, no border)
            -- ImGui.Image(texture, 300, 300, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0)

            ImGui.Spacing()
            if ImGui.IsItemHovered() then
                ImGui.SetTooltip("Texture hovered! (character.png)")
            end
        else
            ImGui.Text("Texture not loaded (check C++ side)")
        end
    end
    ImGui.End()
end