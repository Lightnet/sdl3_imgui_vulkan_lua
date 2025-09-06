-- window tree node

function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        if ImGui.TreeNode("Tree Node", ImGui.DefaultOpen) then
            ImGui.Text("Nested content")
            ImGui.Button("Nested Button")
            if ImGui.IsItemClicked() then
                print("Nested button clicked")
            end
            ImGui.TreePop()
        end
    end
    ImGui.End()
end