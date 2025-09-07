-- window blank


function draw()
    local open = ImGui.Begin("Texture Manager 2", true)
    if open then
        ImGui.Text("Dynamic Load/Unload Textures")
        ImGui.Separator()

        -- Load button (e.g., load a second image)
        if ImGui.Button("Load New Texture (icon.png)") then
            local tex_id = ImGui.LoadTexture("icon.png", "icon")  -- Path relative to exe, name "icon"
            print("tex_id hex:", string.format("%p", tex_id or "nil"))  -- Debug address
            if tex_id then
                -- print("tex_id: ", tex_id)
                print("Loaded icon.png as ImGui.textures.icon")
            else
                print("Failed to load icon.png")
            end
        end
        ImGui.Spacing()

        -- Display loaded textures (fixed count)
        local textures = ImGui.textures or {}
        print(ImGui.textures)
        local texture_count = 0
        for _ in pairs(textures) do
            texture_count = texture_count + 1
            print("pairs")
        end
        ImGui.Text("Loaded count: " .. texture_count)  -- Now accurate (e.g., 2 after loading icon)

        -- -- Display loaded textures
        -- local textures = ImGui.textures or {}

        -- if textures.character then
        --     ImGui.Image(textures.character, 150, 150)
        --     ImGui.SameLine()
        --     if ImGui.Button("Unload Character") then
        --         if ImGui.UnloadTexture("character") then
        --             print("Unloaded character")
        --         end
        --     end
        -- end


        -- if textures.icon then
        --     ImGui.Image(textures.icon, 150, 150)
        --     ImGui.SameLine()
        --     if ImGui.Button("Unload Icon") then
        --         if ImGui.UnloadTexture("icon") then
        --             print("Unloaded icon")
        --         end
        --     end
        -- end

        ImGui.Text("Loaded textures: " .. #textures .. " (check console for details)")


    end
    ImGui.End()
end