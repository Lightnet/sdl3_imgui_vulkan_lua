-- window combo
-- bug flags
local combo_selection = 1
local list_selection = 1
local combo_items = {"Option A", "Option B", "Option C"}

function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        -- local combo_changed, new_combo_selection = ImGui.Combo("Combo Box", combo_selection, combo_items, ImGui.PopupAlignLeft)
        local combo_changed, new_combo_selection = ImGui.Combo("Combo Box", combo_selection, combo_items)
        if combo_changed then
            combo_selection = new_combo_selection
            -- print("Combo selected: " .. combo_items[combo_selection])
        end
    end
    ImGui.End()
end