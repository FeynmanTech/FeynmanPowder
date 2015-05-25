if arg[1] == 'make' then
	os.execute("luajit -b comp.lua comp.out")
else
	os.execute("python generator.py")
	os.execute('lua "src/Lua Scripts/file2c.lua" "src/Lua Scripts/multiplayer.lua" "src/Lua Scripts/multiplayer.h"')
	os.execute('lua "src/Lua Scripts/file2c.lua" "src/Lua Scripts/texter.lua" "src/Lua Scripts/texter.h" "texter_luac"')
	--os.execute('lua "src/Lua Scripts/file2c.lua" "src/Lua Scripts/scriptmanager.lua" "src/Lua Scripts/scriptmanager.h" "scriptmanager_luac"')
	local release, win, tptnet, mac
	local flags = {}
	local params = ""
	local b32
	for i, v in ipairs(arg) do
		params = ""
		if v:find("release") then
			release = true
		end
		if v:find("win") then
			win = true
		elseif v:find("mac") then
			mac = true
		end
		if v:find("net") or v:find("tptnet") then
			tptnet = true
		end
		if v:find("flag:.+") or v:find("flag=.+") then
			table.insert(flags, v:sub(6, -1))
		end
		if v:find("32") then
			b32 = true
		elseif v:find("64") then
			b64 = true
		end
		if v:sub(1, 2) == "--" then
			params = params .. v .. " "
			print("Parameter: " .. v)
		end
	end
	os.execute('scons -j8 --static --output=build/FeynmanPowder' .. (b32 and '_x86' or '') .. (mac and '_mac' or '') .. (win and '.exe' or '') .. --[[' --builddir="./build/" ' ..]] (release and ' --release' or '') .. (win and ' --win' or '') .. (mac and ' --mac' or '') .. (tptnet and ' --tptnet' or '') .. (b32 and ' --32bit' or (b64 and ' --64bit' or '')) .. ' ' .. table.concat(flags, ' ') .. ' ' .. params)
end