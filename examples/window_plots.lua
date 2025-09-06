-- window blank
-- Sample data for plots
local plot_data = {0.0, 0.84, 0.91, 0.14, -0.76, -0.96, -0.28, 0.66, 0.99, 0.41} -- Sine-like values
function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        -- Line plot
        ImGui.PlotLines("Line Plot", plot_data, "Sine Wave", -1.0, 1.0, 300, 100)
        if ImGui.IsItemHovered() then
            ImGui.BeginTooltip()
            ImGui.Text("Sine wave line plot")
            ImGui.EndTooltip()
        end
    end
    ImGui.End()
end