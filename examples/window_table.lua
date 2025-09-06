-- window blank


function draw()
    local open = ImGui.Begin("Lua Test Window", true)
    if open then
        -- Table
        if ImGui.BeginTable("SampleTable", 3, {ImGui.TableResizable, ImGui.TableBorders}) then
            -- Header
            ImGui.TableNextRow()
            ImGui.TableSetColumnIndex(0)
            ImGui.Text("ID")
            ImGui.TableSetColumnIndex(1)
            ImGui.Text("Name")
            ImGui.TableSetColumnIndex(2)
            ImGui.Text("Value")

            -- Rows
            for i = 1, 3 do
                ImGui.TableNextRow()
                ImGui.TableSetColumnIndex(0)
                ImGui.Text(tostring(i))
                ImGui.TableSetColumnIndex(1)
                ImGui.Text("Item " .. i)
                ImGui.TableSetColumnIndex(2)
                ImGui.Text(string.format("%.2f", i * 0.5))
            end
            ImGui.EndTable()
        end
    end
    ImGui.End()
end