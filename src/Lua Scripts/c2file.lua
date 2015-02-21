--for i, v in pairs(arg) do print(i, v) end

local f = io.open(arg[1], "r")
local s = f:read("*all")
f:close()

print("\"(.-)\\n\"\n -> %1\n")
s = s:gsub("\"(.-)\\n\"\n", "%1\n")

print("\\\\ -> \\")
s = s:gsub("\\\\", "\\")
print("\\\" -> \"")
s = s:gsub("\\\"", "\"")

f = io.open(arg[2], "w")
f:write(s)
f:close()
