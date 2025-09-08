-- window theme


function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        ImGui.Text("Hello from Lua!")
        if ImGui.Button("Dark theme ") then
            print("Button clicked in Lua!")
            -- Apply dark theme (call once, e.g., at start)
            ImGui.StyleDark()
        end
        if ImGui.Button("Light theme") then
            print("Button clicked in Lua!")
            -- Or light
            ImGui.StyleLight()
        end
        if ImGui.Button("Style Classic Theme") then
            print("Button clicked in Lua!")
            -- Or classic/default
            ImGui.StyleClassic()
        end
    end
    ImGui.End()
end