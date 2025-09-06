-- window blank


function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        -- Columns
        ImGui.Columns(2, "SampleColumns", true)
        ImGui.SetColumnWidth(0, 150)
        ImGui.Text("Column 1: Text")
        ImGui.NextColumn()
        ImGui.Text("Column 2: Button")
        ImGui.Button("Column Button")
        if ImGui.IsItemClicked() then
            print("Column button clicked")
        end
        ImGui.Columns(1) -- Reset to single column
    end
    ImGui.End()
end