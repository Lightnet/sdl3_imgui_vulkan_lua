-- window list box
-- error here
local list_selection = 1
local combo_items = {"Option A", "Option B", "Option C"}

function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        -- List box
        local list_changed, new_list_selection = ImGui.ListBox("List Box", list_selection, combo_items)
        if list_changed then
            list_selection = new_list_selection
            print("list_selection: ".. list_selection)
            -- error here need to fixed
            -- print("List selected: " .. combo_items[list_selection])
        end
        -- if ImGui.IsItemHovered() then
        --     print("List box hovered")
        -- end
    end
    ImGui.End()
end