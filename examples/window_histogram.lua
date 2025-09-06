-- window Histogram

local hist_data = {0.2, 0.5, 0.8, 0.3, 0.9, 0.4, 0.7, 0.6, 0.1, 0.5} -- Random values
function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        -- Histogram
        ImGui.PlotHistogram("Histogram", hist_data, "Random Values", 0.0, 1.0, 300, 100)
        
    end
    ImGui.End()
end