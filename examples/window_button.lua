-- test button

function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        ImGui.Text("Hello from Lua!")
        if ImGui.Button("Click Me") then
            print("Button clicked in Lua!")
        end
    end
    ImGui.End()
end