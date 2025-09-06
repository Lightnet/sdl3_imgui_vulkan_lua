-- script.lua
local text = "Initial text"
local multi_text = "Initial\nmultiline\ntext"
local slider_value = 0.5
local checkbox_state = false
local radio_selection = 1
local color_r, color_g, color_b = 1.0, 0.5, 0.0
local progress = 0.5
local combo_selection = 1
local list_selection = 1
local combo_items = {"Option A", "Option B", "Option C"}

function draw()
    local open = ImGui.Begin("Lua Test Window", true, ImGui.MenuBar)
    -- local open = ImGui.Begin("Lua Test Window", true)
    if open then
        if ImGui.BeginMenuBar() then
            if ImGui.BeginMenu("File") then
                if ImGui.MenuItem("New") then
                    print("Menu: New selected")
                end
                if ImGui.MenuItem("Open", "Ctrl+O", false, true) then
                    print("Menu: Open selected")
                end
                ImGui.Separator()
                if ImGui.MenuItem("Save") then
                    print("Menu: Save selected")
                end
                ImGui.EndMenu()
            end
            if ImGui.BeginMenu("Edit") then
                if ImGui.MenuItem("Cut") then
                    print("Menu: Cut selected")
                end
                if ImGui.MenuItem("Copy") then
                    print("Menu: Copy selected")
                end
                if ImGui.MenuItem("Paste", nil, false, checkbox_state) then
                    print("Menu: Paste selected")
                end
                ImGui.EndMenu()
            end
            ImGui.EndMenuBar()
        end

        -- Tab bar
        if ImGui.BeginTabBar("MyTabBar") then
            -- Tab 1: Input
            -- if ImGui.BeginTabItem("Input", true, ImGui.SetSelected) then -- this will lock tab
            if ImGui.BeginTabItem("Input", true) then
                ImGui.Text("Input Controls")
                ImGui.Separator()

                -- Single-line text input
                local changed, new_text = ImGui.InputText("Text Input", text, ImGui.EnterReturnsTrue)
                if changed then
                    text = new_text
                    print("Text changed to: " .. text)
                end
                ImGui.Spacing()

                -- Multiline text input
                local multi_changed, new_multi_text = ImGui.InputTextMultiline("Multiline Input", multi_text, 0, 100)
                if multi_changed then
                    multi_text = new_multi_text
                    print("Multiline text changed to: " .. multi_text)
                end
                if ImGui.IsItemHovered() then
                    print("Multiline input hovered")
                end
                ImGui.Spacing()

                -- Slider
                local slider_changed, new_value = ImGui.SliderFloat("Slider", slider_value, 0.0, 1.0)
                if slider_changed then
                    slider_value = new_value
                    progress = new_value
                    print("Slider value: " .. slider_value)
                end

                ImGui.EndTabItem()
            end

            -- Tab 2: Selection
            if ImGui.BeginTabItem("Selection") then
                ImGui.Text("Selection Controls")
                ImGui.Separator()

                -- Checkbox
                local check_changed, new_check_state = ImGui.Checkbox("Enable", checkbox_state)
                if check_changed then
                    checkbox_state = new_check_state
                    print("Checkbox state: " .. tostring(checkbox_state))
                end
                ImGui.Spacing()

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
                ImGui.Spacing()

                -- Combo box
                local combo_changed, new_combo_selection = ImGui.Combo("Combo Box", combo_selection, combo_items, ImGui.PopupAlignLeft)
                if combo_changed then
                    combo_selection = new_combo_selection
                    print("Combo selected: " .. combo_items[combo_selection])
                end
                ImGui.Spacing()

                -- List box
                local list_changed, new_list_selection = ImGui.ListBox("List Box", list_selection, combo_items)
                if list_changed then
                    list_selection = new_list_selection
                    print("List selected: " .. combo_items[list_selection])
                end
                if ImGui.IsItemHovered() then
                    print("List box hovered")
                end
                ImGui.Spacing()

                -- Color picker
                local color_changed, new_r, new_g, new_b = ImGui.ColorEdit3("Color Picker", color_r, color_g, color_b, ImGui.NoAlpha)
                if color_changed then
                    color_r, color_g, color_b = new_r, new_g, new_b
                    print(string.format("Color changed to: R=%.2f, G=%.2f, B=%.2f", color_r, color_g, color_b))
                end

                ImGui.EndTabItem()
            end

            ImGui.EndTabBar()
        end
        ImGui.Separator()

        -- Progress bar
        ImGui.ProgressBar(progress, 0, 0, string.format("%.0f%%", progress * 100))
        ImGui.Spacing()

        -- Tree node
        if ImGui.TreeNode("Tree Node", ImGui.DefaultOpen) then
            ImGui.Text("Nested content")
            ImGui.Button("Nested Button")
            if ImGui.IsItemClicked() then
                print("Nested button clicked")
            end
            ImGui.TreePop()
        end
        ImGui.Spacing()

        -- Button
        if ImGui.Button("Click Me") then
            print("Button clicked in Lua!")
        end
        if ImGui.IsItemHovered() then
            print("Button hovered")
        end

        -- Window dimensions
        local width = ImGui.GetWindowWidth()
        local height = ImGui.GetWindowHeight()
        ImGui.Text("Window size: " .. width .. "x" .. height)
    end
    ImGui.End()
end