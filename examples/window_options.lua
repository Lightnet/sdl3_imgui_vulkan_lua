-- window blank


function draw()
    local open = ImGui.Begin("Lua Test Window", true, {ImGui.MenuBar, ImGui.NoResize, ImGui.NoMove})
    if open then

    end
    ImGui.End()
end