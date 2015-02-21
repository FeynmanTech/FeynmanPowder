if not arg[1] then
	print("Usage:")
	print("file2c.lua [file_in] [file_out]")
	print()
	print("Parses file file_in and converts it to a .h file usable in Powder Toy mods.")
	print("If file_out is given, the converted data is written to that file.")
	print("Otherwise, it is written to foo.h (assuming the input file was foo.lua)")
	print()
	print("Press ENTER to exit...")
	io.read()
	return
end

local f = io.open(arg[1], "r")

if not f then
	print("Unable to open " .. arg[1] .. " for reading.")
	return
else
	print("Opened " .. arg[1] .. ".")
	print("Reading...")
end

local s = f:read("*all")
f:close()

local formatted = '#ifdef LUACONSOLE\n\nconst char* ' .. (arg[3] or 'multiplayer_luac') .. ' = ""\n'

if not(s) then
	print("Unable to read from " .. arg[1] .. ".")
	return
else
	print("File read.")
	print("Parsing...")
end

print("Escaping characters...")
--s = ("%q"):format(s)
s = s:gsub("\\", "\\\\")
s = s:gsub("\\\\(%d%d%d)", "\\%1")
s = s:gsub("\"", "\\\"")

print("Formatting lines...")
for line in s:gmatch("([^\n]+)\n") do
	--line = line:sub(1, -2)
	formatted = formatted .. ("\"" .. line .. "\\n\"") .. "\n"
end

--formatted = formatted:gsub("\\\\n\"\n", "\\n\"\n")
formatted = formatted .. ';\n#endif'

print("Writing...")
if arg[2] then
	local f = io.open(arg[2], "w")
	f:write(formatted)
	f:close()
else
	local f
	if io.open((arg[1]):gsub(".lua", ".h"), "r") then
		io.write("\n" .. (arg[1]):gsub(".lua", ".h") .. " already exists. Overwrite? (y/n) > ")
		local r = io.read()
		if r.lower() == 'y' then
			f = io.open((arg[1]):gsub(".lua", ".h"), "w")
		else
			io.write("\nFile to write to: ")
			f = io.open(io.read(), "w")
		end
	else
		f = io.open((arg[1]):gsub(".lua", ".h"), "w")
	end
	f:write(formatted)
	f:close()
end
