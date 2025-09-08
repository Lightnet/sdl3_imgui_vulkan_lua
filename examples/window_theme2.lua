-- window theme

function set_custom_theme()
    -- Example: Custom dark theme with rounded windows (5.0 pixel rounding)
    local custom_dark = {
        [ImGui.Col_WindowBg] = {r=0.1, g=0.1, b=0.1, a=0.95},  -- Dark gray window background
        [ImGui.Col_FrameBg] = {r=0.2, g=0.2, b=0.2, a=0.54},    -- Darker frames
        [ImGui.Col_Button] = {r=0.26, g=0.59, b=0.98, a=0.40},   -- Blue-ish buttons
        [ImGui.Col_Text] = {r=1.0, g=1.0, b=1.0, a=1.0},        -- White text
        [ImGui.Col_TextDisabled] = {r=0.5, g=0.5, b=0.5, a=1.0}, -- Gray disabled text
        -- Add more colors as needed (up to ImGui.Col_COUNT - 1)
        -- If you omit a color, it keeps the previous/default value
    }

    local style_tweaks = {
        WindowRounding = 5.0,    -- Rounded window corners
        FrameRounding = 3.0,     -- Rounded frames/buttons
        ChildRounding = 3.0,     -- Rounded child windows
        PopupRounding = 3.0,     -- Rounded popups/tooltips
        GrabRounding = 3.0,      -- Rounded slider grabs
        -- Add more from ImGuiStyle (e.g., ScrollbarSize = 16.0)
    }

    -- Apply the custom theme (call once, e.g., here at the top of script.lua)
    ImGui.StyleCustom(custom_dark, style_tweaks)
end

function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        ImGui.Text("Hello from Lua!")
        if ImGui.Button("Dark theme ") then
            print("Button clicked in Lua!")
            -- Apply dark theme (call once, e.g., at start)
            ImGui.StyleDark()
        end
        if ImGui.Button("Light theme") then
            print("Button clicked in Lua!")
            -- Or light
            ImGui.StyleLight()
        end
        if ImGui.Button("Style Classic Theme") then
            print("Button clicked in Lua!")
            -- Or classic/default
            ImGui.StyleClassic()
        end

        if ImGui.Button("Style Custom Theme") then
            print("Button clicked in Lua!")
            -- Or classic/default
            set_custom_theme()
        end
    end
    ImGui.End()
end