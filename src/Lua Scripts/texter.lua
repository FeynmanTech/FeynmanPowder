function __GDTEXT(text, r, g, b)
	local r, g, b = r, g, b
	function __drawFontRect()
		local x, y = sim.adjustCoords(tpt.mousex, tpt.mousey)
		tpt.drawtext(x, y, text, r, g, b)
	end
	tpt.register_step(__drawFontRect)
	function __stopFontRect(x, y, _b, e)
		if e == 2 then
			local x, y = sim.adjustCoords(tpt.mousex, tpt.mousey)
			sim.plottext(x, y, text, r, g, b)
			tpt.unregister_step(__drawFontRect)
			tpt.unregister_mouseclick(__stopFontRect)
		end
		return false
	end
	tpt.register_mouseclick(__stopFontRect)
end

local toPlot, toPlot2, r, g, b = 'Hello world!', '', 255, 255, 255

local winW, winH = 525, 170

__gdtexter_key_test = function(a, _b, c, d)
	if a == 't' and c == 64 then
		local cmx, cmy = tpt.mousex, tpt.mousey
		local texterWindow = Window:new(305 - winW/2, 190 - winH/2, winW, winH)

		local currentY = 10

		local toPlotIn = Textbox:new(10, currentY, (select(1, texterWindow:size()))-20, 16, toPlot, "[place text here]")
		toPlotIn:onTextChanged(
			function(sender)
				toPlot = toPlotIn:text()
			end
		)

		currentY = currentY + 20

		--[[
		local toPlot2In = Textbox:new(10, currentY, (select(1, texterWindow:size()))-20, 16, toPlot2, "[and here, if you need more space]")
		toPlot2In:onTextChanged(
			function(sender)
				toPlot2 = toPlot2In:text();
			end
		)

		currentY = currentY + 20
		--]]

		local rl = Label:new(14, currentY, tpt.textwidth("Red"), 16, "Red")
		currentY = currentY + 15

		local rIn = Slider:new(10, currentY, 255, 16, 255)
		rIn:onValueChanged(
			function(sender)
				r = rIn:value();
			end
		)
		rIn:value(255)

		currentY = currentY + 20

		local gl = Label:new(14, currentY, tpt.textwidth("Green"), 16, "Green")
		currentY = currentY + 15

		local gIn = Slider:new(10, currentY, 255, 16, 255)
		gIn:onValueChanged(
			function(sender)
				g = gIn:value();
			end
		)
		gIn:value(255)

		currentY = currentY + 20

		local bl = Label:new(14, currentY, tpt.textwidth("Blue"), 16, "Blue")
		currentY = currentY + 15

		local bIn = Slider:new(10, currentY, 255, 16, 255)
		bIn:onValueChanged(
			function(sender)
				b = bIn:value();
			end
		)
		bIn:value(255)

		local closeButton = Button:new(10, select(2, texterWindow:size())-26, 100, 16, "Draw text")

		closeButton:action(function()
			interface.closeWindow(texterWindow)
			__GDTEXT(string.gsub(toPlot, '\\n', '\n'), r, g, b)
		end
		)

		texterWindow:onTryExit(function() interface.closeWindow(texterWindow) end) -- Allow the default exit events

		texterWindow:addComponent(toPlotIn)
		if toPlot2In then
			texterWindow:addComponent(toPlot2In)
		end
		texterWindow:addComponent(rIn)
		texterWindow:addComponent(gIn)
		texterWindow:addComponent(bIn)
		texterWindow:addComponent(rl)
		texterWindow:addComponent(gl)
		texterWindow:addComponent(bl)
		texterWindow:addComponent(closeButton)

		interface.showWindow(texterWindow)
	end
end

tpt.register_keypress(__gdtexter_key_test)

