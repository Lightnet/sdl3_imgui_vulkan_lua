
local slider_float = 0.1

function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    -- header title bar
    -- if ImGui.IsItemHovered() then
    --     print("window hover")
    -- end
    if ImGui.IsItemClicked() then
        print("window IsItemClicked")
    end
    -- draw window 
    if ImGui.IsItemActive() then
        print("window IsItemActive")
    end
    if open then
        ImGui.Text("Hello from Lua!")
    end
    ImGui.End()
end