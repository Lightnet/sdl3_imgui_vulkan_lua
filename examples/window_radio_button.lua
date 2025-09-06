-- window Radio Button
local radio_selection = 1

function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        -- Radio buttons
        if ImGui.RadioButton("Option 1", radio_selection == 1) then
            radio_selection = 1
            print("Selected Option 1")
        end
        ImGui.SameLine()
        if ImGui.RadioButton("Option 2", radio_selection == 2) then
            radio_selection = 2
            print("Selected Option 2")
        end
    end
    ImGui.End()
end