-- script.lua
local text = "Initial text"
local multi_text = "Initial\nmultiline\ntext"
local slider_value = 0.5

function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        ImGui.Text("Hello from Lua!")
        
        -- Single-line text input
        local changed, new_text = ImGui.InputText("Text Input", text)
        if changed then
            text = new_text
            print("Text changed to: " .. text)
        end

        -- Multiline text input
        local multi_changed, new_multi_text = ImGui.InputTextMultiline("Multiline Input", multi_text, 0, 100)
        if multi_changed then
            multi_text = new_multi_text
            print("Multiline text changed to: " .. multi_text)
        end
        if ImGui.IsItemHovered() then
            print("Multiline input hovered")
        end
        if ImGui.IsItemActive() then
            print("Multiline input active")
        end
        if ImGui.IsItemClicked() then
            print("Multiline input clicked")
        end

        -- Slider
        local slider_changed, new_value = ImGui.SliderFloat("Slider", slider_value, 0.0, 1.0)
        if slider_changed then
            slider_value = new_value
            print("Slider value: " .. slider_value)
        end

        -- Button
        if ImGui.Button("Click Me") then
            print("Button clicked in Lua!")
        end
        if ImGui.IsItemActive() then
            print("Button active")
        end
        if ImGui.IsItemHovered() then
            print("Button hovered")
        end
        
        if ImGui.IsItemClicked() then
            print("Button clicked (via IsItemClicked)")
        end

        -- Window dimensions
        local width = ImGui.GetWindowWidth()
        local height = ImGui.GetWindowHeight()
        ImGui.Text("Window size: " .. width .. "x" .. height)

    end
    ImGui.End()
end