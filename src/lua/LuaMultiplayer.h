#include "LuaScriptHelper.h"
#define LuaMultiplayerSrc "\
--Cracker64's Autorun Script Manager\n\
--The autorun to end all autoruns\n\
--Version 3.1\n\
\n\
--TODO:\n\
--manual file addition (that can be anywhere and any extension)\n\
--Moving window (because why not)\n\
--some more API functions\n\
--prettier, organize code\n\
\n\
--CHANGES:\n\
--Version 3.1: Organize scripts less randomly, fix scripts being run twice, fix other bugs\n\
--central script / update server at starcatcher.us / delete local scripts / lots of other things by jacob1 v3.0\n\
--Scan all subdirectories in scripts folder! v2.25\n\
--Remove step hooks, v87 fixes them\n\
--Socket is now default in v87+ , horray, everyone can now use update features without extra downloads.\n\
--Handles up to 50 extra step functions, up from the default 5 (not including the manager's step) v2.1\n\
--Other various nice API functions\n\
--Scripts can store/retrieve settings through the manager, see comments below v2.0\n\
--Small fillrect change for v85, boxes can have backgrounds v1.92\n\
--Support spaces in exe names v1.91\n\
--Auto-update for OTHER scripts now works, is a bit messy, will fix later, but nothing should change for users to use this\n\
--  Place a line '--VER num UPDATE link' in one of the first four lines of the file, see my above example\n\
--  The link at top downloads a file that contains ONLY version,full link,and prints the rest(changelog). See my link for example\n\
\n\
if not socket then error(\"TPT version not supported\") end\n\
if tpt.version.jacob1s_mod then return end\n\
\n\
local VERSION = \"3.1\"\n\
local scriptversion = 2\n\
local TPT_LUA_PATH = 'scripts'\n\
local PATH_SEP = '\\\\'\n\
local WINDOWS=true\n\
local EXE_NAME = \"Powder.exe\"\n\
local CHECKUPDATE = false\n\
if os.getenv('HOME') then\n\
    PATH_SEP = '/'\n\
    EXE_NAME = \"powder\"\n\
    WINDOWS=false\n\
end\n\
local filenames = {}\n\
local num_files = 0 --downloaded scripts aren't stored in filenames\n\
local localscripts = {}\n\
local onlinescripts = {}\n\
local running = {}\n\
local requiresrestart=false\n\
local hidden_mode=true\n\
local online = false\n\
local first_online = true\n\
local updatetable --temporarily holds info on script manager updates\n\
local gen_buttons\n\
local sidebutton\n\
local download_file\n\
local settings = {}\n\
math.randomseed(os.time()) math.random() math.random() math.random() --some filler randoms\n\
\n\
--get line that can be saved into scriptinfo file\n\
local function scriptInfoString(info)\n\
    --Write table into data format\n\
    if type(info)~=\"table\" then return end\n\
    local t = {}\n\
    for k,v in pairs(info) do\n\
        table.insert(t,k..\":\\\"\"..v..\"\\\"\")\n\
    end\n\
    local rstr = table.concat(t,\",\"):gsub(\"\\n\",\"\\\\n\")\n\
    return rstr\n\
end\n\
\n\
--read a scriptinfo line\n\
local function readScriptInfo(list)\n\
    if not list then return {} end\n\
    local scriptlist = {}\n\
    for i in list:gmatch(\"[^\\n]+\") do\n\
        local t = {}\n\
        local ID = 0\n\
        for k,v in i:gmatch(\"(%w+):\\\"([^\\\"]*)\\\"\") do\n\
            t[k]= tonumber(v) or v:gsub(\"\\\\n\",\"\\n\")\n\
        end\n\
        scriptlist[t.ID] = t\n\
    end\n\
    return scriptlist\n\
end\n\
\n\
--save settings\n\
local function save_last()\n\
    local savestring=\"\"\n\
    for script,v in pairs(running) do\n\
        savestring = savestring..\" \\'\"..script..\"\\'\"\n\
    end\n\
    savestring = \"SAV \"..savestring..\"\\nEXE \"..EXE_NAME..\"\\nDIR \"..TPT_LUA_PATH\n\
    for k,t in pairs(settings) do\n\
	for n,v in pairs(t) do\n\
	    savestring = savestring..\"\\nSET \"..k..\" \"..n..\":'\"..v..\"'\"\n\
	end\n\
    end\n\
    local f\n\
    if TPT_LUA_PATH == \"scripts\" then\n\
        f = io.open(TPT_LUA_PATH..PATH_SEP..\"autorunsettings.txt\", \"w\")\n\
    else\n\
        f = io.open(\"autorunsettings.txt\", \"w\")\n\
    end\n\
    if f then\n\
        f:write(savestring)\n\
        f:close()\n\
    end\n\
\n\
    f = io.open(TPT_LUA_PATH..PATH_SEP..\"downloaded\"..PATH_SEP..\"scriptinfo\", \"w\")\n\
    if f then\n\
        for k,v in pairs(localscripts) do\n\
            f:write(scriptInfoString(v)..\"\\n\")\n\
        end\n\
        f:close()\n\
    end\n\
end\n\
\n\
--load settings before anything else\n\
local function load_last()\n\
    local f = io.open(TPT_LUA_PATH..PATH_SEP..\"autorunsettings.txt\",\"r\")\n\
    if not f then\n\
        f = io.open(\"autorunsettings.txt\",\"r\")\n\
    end\n\
    if f then\n\
        local lines = {}\n\
        local line = f:read(\"*l\")\n\
        while line do\n\
            table.insert(lines,line)\n\
            line = f:read(\"*l\")\n\
        end\n\
        f:close()\n\
        for i=1, #lines do\n\
            local tok=lines[i]:sub(1,3)\n\
            local str=lines[i]:sub(5)\n\
            if tok==\"SAV\" then\n\
                for word in string.gmatch(str, \"\\'(.-)\\'\") do running[word] = true end\n\
            elseif tok==\"EXE\" then\n\
                EXE_NAME=str\n\
            elseif tok==\"DIR\" then\n\
                TPT_LUA_PATH=str\n\
            elseif tok==\"SET\" then\n\
	        local ident,name,val = string.match(str,\"(.-) (.-):\\'(.-)\\'\")\n\
		if settings[ident] then settings[ident][name]=val\n\
		else settings[ident]={[name]=val} end\n\
            end\n\
        end\n\
    end\n\
\n\
    f = io.open(TPT_LUA_PATH..PATH_SEP..\"downloaded\"..PATH_SEP..\"scriptinfo\",\"r\")\n\
    if f then\n\
        local lines = f:read(\"*a\")\n\
        f:close()\n\
        localscripts = readScriptInfo(lines)\n\
        for k,v in pairs(localscripts) do\n\
            if k ~= 1 then\n\
                if not v[\"ID\"] or not v[\"name\"] or not v[\"description\"] or not v[\"path\"] or not v[\"version\"] then\n\
                    localscripts[k] = nil\n\
                elseif not fs.exists(TPT_LUA_PATH..\"/\"..v[\"path\"]:gsub(\"\\\\\",\"/\")) then\n\
                     localscripts[k] = nil\n\
                end\n\
            end\n\
        end\n\
    end\n\
end\n\
load_last()\n\
--get list of files in scripts folder\n\
local function load_filenames()\n\
    filenames = {}\n\
    local function searchRecursive(directory)\n\
        local dirlist = fs.list(directory)\n\
        if not dirlist then return end\n\
        for i,v in ipairs(dirlist) do\n\
            local file = directory..\"/\"..v\n\
            if fs.isDirectory(file) and v ~= \"downloaded\" then\n\
                searchRecursive(file)\n\
            elseif fs.isFile(file) then\n\
                if file:find(\"%.lua$\") then\n\
                    local toinsert = file:sub(#TPT_LUA_PATH+2)\n\
                    if WINDOWS then\n\
                        toinsert = toinsert:gsub(\"/\", \"\\\\\") --not actually required\n\
                    end\n\
                    table.insert(filenames, toinsert)\n\
                end\n\
            end\n\
        end\n\
    end\n\
    searchRecursive(TPT_LUA_PATH)\n\
    table.sort(filenames, function(first,second) return first:lower() < second:lower() end)\n\
end\n\
--ui object stuff\n\
local ui_base local ui_box local ui_line local ui_text local ui_button local ui_scrollbar local ui_tooltip local ui_checkbox local ui_console local ui_window\n\
local tooltip\n\
ui_base = {\n\
new = function()\n\
    local b={}\n\
    b.drawlist = {}\n\
    function b:drawadd(f)\n\
        table.insert(self.drawlist,f)\n\
    end\n\
    function b:draw(...)\n\
        for _,f in ipairs(self.drawlist) do\n\
            if type(f)==\"function\" then\n\
                f(self,unpack(arg))\n\
            end\n\
        end\n\
    end\n\
    b.movelist = {}\n\
    function b:moveadd(f)\n\
        table.insert(self.movelist,f)\n\
    end\n\
    function b:onmove(x,y)\n\
        for _,f in ipairs(self.movelist) do\n\
            if type(f)==\"function\" then\n\
                f(self,x,y)\n\
            end\n\
        end\n\
    end\n\
    return b\n\
end\n\
}\n\
ui_box = {\n\
new = function(x,y,w,h,r,g,b)\n\
    local box=ui_base.new()\n\
    box.x=x box.y=y box.w=w box.h=h box.x2=x+w box.y2=y+h\n\
    box.r=r or 255 box.g=g or 255 box.b=b or 255\n\
    function box:setcolor(r,g,b) self.r=r self.g=g self.b=b end\n\
    function box:setbackground(r,g,b,a) self.br=r self.bg=g self.bb=b self.ba=a end\n\
    box.drawbox=true\n\
    box.drawbackground=false\n\
    box:drawadd(function(self) if self.drawbackground then tpt.fillrect(self.x,self.y,self.w+1,self.h+1,self.br,self.bg,self.bb,self.ba) end\n\
                                if self.drawbox then tpt.drawrect(self.x,self.y,self.w,self.h,self.r,self.g,self.b) end end)\n\
    box:moveadd(function(self,x,y)\n\
        if x then self.x=self.x+x self.x2=self.x2+x end\n\
        if y then self.y=self.y+y self.y2=self.y2+y end\n\
    end)\n\
    return box\n\
end\n\
}\n\
ui_line = {\n\
new=function(x,y,x2,y2,r,g,b)\n\
    local line=ui_box.new(x,y,x2-x,y2-y,r,g,b)\n\
    --Line is essentially a box, but with a different draw\n\
    line.drawlist={}\n\
    line:drawadd(function(self) tpt.drawline(self.x,self.y,self.x2,self.y2,self.r,self.g,self.b) end)\n\
    return line\n\
    end\n\
}\n\
ui_text = {\n\
new = function(text,x,y,r,g,b)\n\
    local txt = ui_base.new()\n\
    txt.text = text\n\
    txt.x=x or 0 txt.y=y or 0 txt.r=r or 255 txt.g=g or 255 txt.b=b or 255\n\
    function txt:setcolor(r,g,b) self.r=r self.g=g self.b=b end\n\
    txt:drawadd(function(self,x,y) tpt.drawtext(x or self.x,y or self.y,self.text,self.r,self.g,self.b) end)\n\
    txt:moveadd(function(self,x,y)\n\
        if x then self.x=self.x+x end\n\
        if y then self.y=self.y+y end\n\
    end)\n\
    function txt:process() return false end\n\
    return txt\n\
end,\n\
--Scrolls while holding mouse over\n\
newscroll = function(text,x,y,vis,r,g,b)\n\
    local txt = ui_text.new(text,x,y,r,g,b)\n\
    if tpt.textwidth(text)<vis then return txt end\n\
    txt.visible=vis\n\
    txt.length=string.len(text)\n\
    txt.start=1\n\
    txt.drawlist={} --reset draw\n\
    txt.timer=socket.gettime()+3\n\
    function txt:cuttext(self)\n\
        local last = self.start+1\n\
        while tpt.textwidth(self.text:sub(self.start,last))<txt.visible and last<=self.length do\n\
            last = last+1\n\
        end\n\
        self.last=last-1\n\
    end\n\
    txt:cuttext(txt)\n\
    txt.minlast=txt.last-1\n\
    txt.ppl=((txt.visible-6)/(txt.length-txt.minlast+1))\n\
    txt:drawadd(function(self,x,y)\n\
        if socket.gettime() > self.timer then\n\
            if self.last >= self.length then\n\
                self.start = 1\n\
                self:cuttext(self)\n\
                self.timer = socket.gettime()+3\n\
            else\n\
                self.start = self.start + 1\n\
                self:cuttext(self)\n\
                if self.last >= self.length then\n\
                    self.timer = socket.gettime()+3\n\
                else\n\
                    self.timer = socket.gettime()+.15\n\
                end\n\
            end\n\
        end\n\
        tpt.drawtext(x or self.x,y or self.y, self.text:sub(self.start,self.last) ,self.r,self.g,self.b)\n\
    end)\n\
    function txt:process(mx,my,button,event,wheel)\n\
        if event==3 then\n\
            local newlast = math.floor((mx-self.x)/self.ppl)+self.minlast\n\
            if newlast<self.minlast then newlast=self.minlast end\n\
            if newlast>0 and newlast~=self.last then\n\
                local newstart=1\n\
                while tpt.textwidth(self.text:sub(newstart,newlast))>= self.visible do\n\
                    newstart=newstart+1\n\
                end\n\
                self.start=newstart self.last=newlast\n\
                self.timer = socket.gettime()+3\n\
            end\n\
        end\n\
    end\n\
    return txt\n\
end\n\
}\n\
ui_scrollbar = {\n\
new = function(x,y,h,t,m)\n\
    local bar = ui_base.new() --use line object as base?\n\
    bar.x=x bar.y=y bar.h=h\n\
    bar.total=t\n\
    bar.numshown=m\n\
    bar.pos=0\n\
    bar.length=math.floor((1/math.ceil(bar.total-bar.numshown+1))*bar.h)\n\
    bar.soffset=math.floor(bar.pos*((bar.h-bar.length)/(bar.total-bar.numshown)))\n\
    function bar:update(total,shown,pos)\n\
        self.pos=pos or 0\n\
        if self.pos<0 then self.pos=0 end\n\
        self.total=total\n\
        self.numshown=shown\n\
        self.length= math.floor((1/math.ceil(self.total-self.numshown+1))*self.h)\n\
        self.soffset= math.floor(self.pos*((self.h-self.length)/(self.total-self.numshown)))\n\
    end\n\
    function bar:move(wheel)\n\
        self.pos = self.pos-wheel\n\
        if self.pos < 0 then self.pos=0 end\n\
        if self.pos > (self.total-self.numshown) then self.pos=(self.total-self.numshown) end\n\
        self.soffset= math.floor(self.pos*((self.h-self.length)/(self.total-self.numshown)))\n\
    end\n\
    bar:drawadd(function(self)\n\
        if self.total > self.numshown then\n\
            tpt.drawline(self.x,self.y+self.soffset,self.x,self.y+self.soffset+self.length)\n\
        end\n\
    end)\n\
    bar:moveadd(function(self,x,y)\n\
        if x then self.x=self.x+x end\n\
        if y then self.y=self.y+y end\n\
    end)\n\
    function bar:process(mx,my,button,event,wheel)\n\
        if wheel~=0 and not hidden_mode then\n\
            if self.total > self.numshown then\n\
                local previous = self.pos\n\
                self:move(wheel)\n\
                if self.pos~=previous then\n\
                    return previous-self.pos\n\
                end\n\
            end\n\
        end\n\
        --possibly click the bar and drag?\n\
        return false\n\
    end\n\
    return bar\n\
end\n\
}\n\
ui_button = {\n\
new = function(x,y,w,h,f,text)\n\
    local b = ui_box.new(x,y,w,h)\n\
    b.f=f\n\
    b.t=ui_text.new(text,x+2,y+2)\n\
    b.drawbox=false\n\
    b.clicked=false\n\
    b.almostselected=false\n\
    b.invert=true\n\
    b:setbackground(127,127,127,125)\n\
    b:drawadd(function(self)\n\
        if self.invert and self.almostselected then\n\
            self.almostselected=false\n\
            tpt.fillrect(self.x,self.y,self.w,self.h)\n\
            local tr=self.t.r local tg=self.t.g local tb=self.t.b\n\
            b.t:setcolor(0,0,0)\n\
            b.t:draw()\n\
            b.t:setcolor(tr,tg,tb)\n\
        else\n\
            if tpt.mousex>=self.x and tpt.mousex<=self.x2 and tpt.mousey>=self.y and tpt.mousey<=self.y2 then\n\
                self.drawbackground=true\n\
            else\n\
                self.drawbackground=false\n\
            end\n\
            b.t:draw()\n\
        end\n\
    end)\n\
    b:moveadd(function(self,x,y)\n\
        self.t:onmove(x,y)\n\
    end)\n\
    function b:process(mx,my,button,event,wheel)\n\
        local clicked = self.clicked\n\
        if event==2 then self.clicked = false end\n\
        if mx<self.x or mx>self.x2 or my<self.y or my>self.y2 then return false end\n\
        if event==1 then\n\
            self.clicked=true\n\
        elseif clicked then\n\
            if event==3 then self.almostselected=true end\n\
            if event==2 then self:f() end\n\
            return true\n\
        end\n\
    end\n\
    return b\n\
end\n\
}\n\
ui_tooltip = {\n\
new = function(x,y,w,text)\n\
    local b = ui_box.new(x,y-1,w,0)\n\
    function b:updatetooltip(tooltip)\n\
        self.tooltip = tooltip\n\
        self.length = #tooltip\n\
        self.lines = 0\n\
        local start,last = 1,2\n\
        while last <= self.length do\n\
            while tpt.textwidth(self.tooltip:sub(start,last)) < w and last <= self.length and self.tooltip:sub(last,last) ~= '\\n' do\n\
                last = last + 1\n\
            end\n\
            if last <= self.length and self.tooltip:sub(last,last) ~= '\\n' then\n\
                self.length = self.length + 1\n\
                self.tooltip = self.tooltip:sub(1,last-1)..\"\\n\"..self.tooltip:sub(last)\n\
            end\n\
            last = last + 1\n\
            start = last\n\
            self.lines = self.lines + 1\n\
	    end\n\
        self.h = self.lines*12+2\n\
        --if self.lines == 1 then self.w = tpt.textwidth(self.tooltip)+3 end\n\
        self.drawbox = tooltip ~= \"\"\n\
        self.drawbackground = tooltip ~= \"\"\n\
    end\n\
    function b:settooltip(tooltip_)\n\
        tooltip:onmove(tpt.mousex+5-tooltip.x, tpt.mousey+5-tooltip.y)\n\
        tooltip:updatetooltip(tooltip_)\n\
    end\n\
    b:updatetooltip(text)\n\
    b:setbackground(0,0,0,255)\n\
    b.drawbackground = true\n\
    b:drawadd(function(self)\n\
        if self.tooltip ~= \"\" then\n\
            tpt.drawtext(self.x+1,self.y+2,self.tooltip)\n\
        end\n\
        self:updatetooltip(\"\")\n\
    end)\n\
    function b:process(mx,my,button,event,wheel) end\n\
    return b\n\
end\n\
}\n\
ui_checkbox = {\n\
up_button = function(x,y,w,h,f,text)\n\
    local b=ui_button.new(x,y,w,h,f,text)\n\
    b.canupdate=false\n\
    return b\n\
end,\n\
new_button = function(x,y,w,h,splitx,f,f2,text)\n\
    local b = ui_box.new(x,y,splitx,h)\n\
    b.f=f b.f2=f2\n\
    b.splitx = splitx\n\
    b.t=ui_text.newscroll(text,x+24,y+2,splitx-24)\n\
    b.clicked=false\n\
    b.selected=false\n\
    b.checkbut=ui_checkbox.up_button(x+splitx+9,y,33,9,ui_button.scriptcheck,\"Update\")\n\
    b.canupdate = false\n\
    b.drawbox=false\n\
    b:setbackground(127,127,127,100)\n\
    b:drawadd(function(self)\n\
        if self.t.text == \"\" then return end\n\
        if tpt.mousex>=self.x and tpt.mousex<self.x2 and tpt.mousey>=self.y and tpt.mousey<self.y2 then\n\
            local script\n\
            if online and onlinescripts[self.ID][\"description\"] then\n\
                script = onlinescripts[self.ID]\n\
            elseif not online and localscripts[self.ID] then\n\
                script = localscripts[self.ID]\n\
            end\n\
            if script then\n\
                tooltip:settooltip(script[\"name\"]..\" by \"..script[\"author\"]..\"\\n\\n\"..script[\"description\"])\n\
            end\n\
            self.drawbackground=true\n\
        else\n\
            if tpt.mousey>=self.y and tpt.mousey<self.y2 and tpt.mousex > self.x then\n\
                if tpt.mousex < self.x2+9 and self.running then\n\
                    tooltip:settooltip(online and \"downloaded\" or \"running\")\n\
                elseif tpt.mousex >= self.x2+9 and tpt.mousex < self.x2+43 and self.canupdate and onlinescripts[self.ID] and onlinescripts[self.ID][\"changelog\"] then\n\
                    tooltip:settooltip(onlinescripts[self.ID][\"changelog\"])\n\
                end\n\
            end\n\
            self.drawbackground=false\n\
        end\n\
        self.t:draw()\n\
        if self.f2 then\n\
            if self.deletealmostselected then\n\
                self.deletealmostselected = false\n\
                tpt.drawtext(self.x+1, self.y+1, \"\\134\", 255, 48, 32, 255)\n\
            else\n\
                tpt.drawtext(self.x+1, self.y+1, \"\\134\", 160, 48, 32, 255)\n\
            end\n\
            tpt.drawtext(self.x+1, self.y+1, \"\\133\", 255, 255, 255, 255)\n\
        end\n\
        tpt.drawrect(self.x+12,self.y+1,8,8)\n\
        if self.almostselected then self.almostselected=false tpt.fillrect(self.x+12,self.y+1,8,8,150,150,150)\n\
        elseif self.selected then tpt.fillrect(self.x+12,self.y+1,8,8) end\n\
        local filepath = self.ID and localscripts[self.ID] and localscripts[self.ID][\"path\"] or self.t.text\n\
        if self.running then tpt.drawtext(self.x+self.splitx+2,self.y+2,online and \"D\" or \"R\") end\n\
        if self.checkbut.canupdate then self.checkbut:draw() end\n\
    end)\n\
    b:moveadd(function(self,x,y)\n\
        self.t:onmove(x,y)\n\
        self.checkbut:onmove(x,y)\n\
    end)\n\
    function b:process(mx,my,button,event,wheel)\n\
        if self.f2 and mx <= self.x+8 then\n\
            if event==1 then\n\
                self.clicked = 1\n\
            elseif self.clicked == 1 then\n\
                if event==3 then self.deletealmostselected = true end\n\
                if event==2 then self:f2() end\n\
            end\n\
        elseif self.f and mx<=self.x+self.splitx then\n\
            if event==1 then\n\
                self.clicked = 2\n\
            elseif self.clicked == 2 then\n\
                if event==3 then self.almostselected=true end\n\
                if event==2 then self:f() end\n\
                self.t:process(mx,my,button,event,wheel)\n\
            end\n\
        else\n\
            if self.checkbut.canupdate then self.checkbut:process(mx,my,button,event,wheel) end\n\
        end\n\
        return true\n\
    end\n\
    return b\n\
end,\n\
new = function(x,y,w,h)\n\
    local box = ui_box.new(x,y,w,h)\n\
    box.list={}\n\
    box.numlist = 0\n\
    box.max_lines = math.floor(box.h/10)-1\n\
    box.max_text_width = math.floor(box.w*0.8)\n\
    box.splitx=x+box.max_text_width\n\
    box.scrollbar = ui_scrollbar.new(box.x2-2,box.y+11,box.h-12,0,box.max_lines)\n\
    box.lines={\n\
        ui_line.new(box.x+1,box.y+10,box.x2-1,box.y+10,170,170,170),\n\
        ui_line.new(box.x+22,box.y+10,box.x+22,box.y2-1,170,170,170),\n\
        ui_line.new(box.splitx,box.y+10,box.splitx,box.y2-1,170,170,170),\n\
        ui_line.new(box.splitx+9,box.y+10,box.splitx+9,box.y2-1,170,170,170),\n\
    }\n\
    function box:updatescroll()\n\
        self.scrollbar:update(self.numlist,self.max_lines)\n\
    end\n\
    function box:clear()\n\
        self.list={}\n\
        self.numlist=0\n\
    end\n\
    function box:add(f,f2,text)\n\
        local but = ui_checkbox.new_button(self.x,self.y+1+((self.numlist+1)*10),tpt.textwidth(text)+4,10,self.max_text_width,f,f2,text)\n\
        table.insert(self.list,but)\n\
        self.numlist = #self.list\n\
        return but\n\
    end\n\
    box:drawadd(function (self)\n\
        tpt.drawtext(self.x+24,self.y+2,\"Files in \"..TPT_LUA_PATH..\" folder\")\n\
        tpt.drawtext(self.splitx+11,self.y+2,\"Update\")\n\
        for i,line in ipairs(self.lines) do\n\
            line:draw()\n\
        end\n\
        self.scrollbar:draw()\n\
        local restart = false\n\
        for i,check in ipairs(self.list) do\n\
            local filepath = check.ID and localscripts[check.ID] and localscripts[check.ID][\"path\"] or check.t.text\n\
            if not check.selected and running[filepath] then\n\
                restart = true\n\
            end\n\
            if i>self.scrollbar.pos and i<=self.scrollbar.pos+self.max_lines then\n\
                check:draw()\n\
            end\n\
        end\n\
        requiresrestart = restart and not online\n\
    end)\n\
    box:moveadd(function(self,x,y)\n\
        for i,line in ipairs(self.lines) do\n\
            line:onmove(x,y)\n\
        end\n\
        for i,check in ipairs(self.list) do\n\
            check:onmove(x,y)\n\
        end\n\
    end)\n\
    function box:scroll(amount)\n\
        local move = amount*10\n\
        if move==0 then return end\n\
        for i,check in ipairs(self.list) do\n\
            check:onmove(0,move)\n\
        end\n\
    end\n\
    function box:process(mx,my,button,event,wheel)\n\
        if mx<self.x or mx>self.x2 or my<self.y or my>self.y2 then return false end\n\
        local scrolled = self.scrollbar:process(mx,my,button,event,wheel)\n\
        if scrolled then self:scroll(scrolled) end\n\
        local which = math.floor((my-self.y-11)/10)+1\n\
        if which>0 and which<=self.numlist then self.list[which+self.scrollbar.pos]:process(mx,my,button,event,wheel) end\n\
        if event == 2 then\n\
            for i,v in ipairs(self.list) do v.clicked = false end\n\
        end\n\
        return true\n\
    end\n\
    return box\n\
end\n\
}\n\
ui_console = {\n\
new = function(x,y,w,h)\n\
    local con = ui_box.new(x,y,w,h)\n\
    con.shown_lines = math.floor(con.h/10)\n\
    con.max_lines = 300\n\
    con.max_width = con.w-4\n\
    con.lines = {}\n\
    con.scrollbar = ui_scrollbar.new(con.x2-2,con.y+1,con.h-2,0,con.shown_lines)\n\
    con:drawadd(function(self)\n\
        self.scrollbar:draw()\n\
        local count=0\n\
        for i,line in ipairs(self.lines) do\n\
            if i>self.scrollbar.pos and i<= self.scrollbar.pos+self.shown_lines then\n\
                line:draw(self.x+3,self.y+3+(count*10))\n\
                count = count+1\n\
            end\n\
        end\n\
    end)\n\
    con:moveadd(function(self,x,y)\n\
        self.scrollbar:onmove(x,y)\n\
    end)\n\
    function con:clear()\n\
        self.lines = {}\n\
        self.scrollbar:update(0,con.shown_lines)\n\
    end\n\
    function con:addstr(str,r,g,b)\n\
        str = tostring(str)\n\
        local nextl = str:find('\\n')\n\
        while nextl do\n\
            local line = str:sub(1,nextl-1)\n\
            self:addline(line,r,g,b)\n\
            str = str:sub(nextl+1)\n\
            nextl = str:find('\\n')\n\
        end\n\
        self:addline(str,r,g,b) --anything leftover\n\
    end\n\
    function con:addline(line,r,g,b)\n\
        if not line or line==\"\" then return end --No blank lines\n\
        table.insert(self.lines,ui_text.newscroll(line,self.x,0,self.max_width,r,g,b))\n\
        if #self.lines>self.max_lines then table.remove(self.lines,1) end\n\
        self.scrollbar:update(#self.lines,self.shown_lines,#self.lines-self.shown_lines)\n\
    end\n\
    function con:process(mx,my,button,event,wheel)\n\
        if mx<self.x or mx>self.x2 or my<self.y or my>self.y2 then return false end\n\
        self.scrollbar:process(mx,my,button,event,wheel)\n\
        local which = math.floor((my-self.y-1)/10)+1\n\
        if which>0 and which<=self.shown_lines and self.lines[which+self.scrollbar.pos] then self.lines[which+self.scrollbar.pos]:process(mx,my,button,event,wheel) end\n\
        return true\n\
    end\n\
    return con\n\
end\n\
}\n\
ui_window = {\n\
new = function(x,y,w,h)\n\
    local w=ui_box.new(x,y,w,h)\n\
    w.sub={}\n\
    function w:add(m,name)\n\
        if name then w[name]=m end\n\
        table.insert(self.sub,m)\n\
    end\n\
    w:drawadd(function(self)\n\
        for i,sub in ipairs(self.sub) do\n\
            sub:draw()\n\
        end\n\
    end)\n\
    w:moveadd(function(self,x,y)\n\
        for i,sub in ipairs(self.sub) do\n\
            sub:onmove(x,y)\n\
        end\n\
    end)\n\
    function w:process(mx,my,button,event,wheel)\n\
        if mx<self.x or mx>self.x2 or my<self.y or my>self.y2 then ui_button.sidepressed() return true end\n\
        local ret\n\
        for i,sub in ipairs(self.sub) do\n\
            if sub:process(mx,my,button,event,wheel) then ret = true end\n\
        end\n\
        return ret\n\
    end\n\
    return w\n\
end\n\
}\n\
--Main window with everything!\n\
local mainwindow = ui_window.new(50,50,525,300)\n\
mainwindow:setbackground(10,10,10,235) mainwindow.drawbackground=true\n\
mainwindow:add(ui_console.new(275,148,300,189),\"menuconsole\")\n\
mainwindow:add(ui_checkbox.new(50,80,225,257),\"checkbox\")\n\
tooltip = ui_tooltip.new(0,1,250,\"\")\n\
\n\
--Some API functions you can call from other scripts\n\
--put 'using_manager=MANAGER_EXISTS' or similar in your scripts, using_manager will be true if the manager is active\n\
MANAGER_EXISTS = true\n\
--Print a message to the manager console, can be colored\n\
function MANAGER_PRINT(msg,...)\n\
    mainwindow.menuconsole:addstr(msg,unpack(arg))\n\
end\n\
--send version, for some compatibility\n\
function MANAGER_VERSION()\n\
    return VERSION\n\
end\n\
--downloads and returns a file, so you can do whatever...\n\
function MANAGER_DOWNLOAD(url)\n\
    return download_file(url)\n\
end\n\
--Get various info about the system (if on windows, script directory, path seperator, if socket is loaded)\n\
function MANAGER_SYSINFO()\n\
    return {[\"isWindows\"]=WINDOWS,[\"scriptDir\"]=TPT_LUA_PATH,[\"pathSep\"]=PATH_SEP,[\"socket\"]=true}\n\
end\n\
--Get whether or not the manager window is hidden or not\n\
function MANAGER_HIDDEN()\n\
    return hidden_mode\n\
end\n\
--Save a setting in the autorun settings file, ident should be your script name no one else would use.\n\
--Name is variable name, val is the value which will be saved/returned as a string\n\
function MANAGER_SAVESETTING(ident,name,val)\n\
    ident = tostring(ident)\n\
    name = tostring(name)\n\
    val = tostring(val)\n\
    if settings[ident] then settings[ident][name]=val\n\
    else settings[ident]={[name]=val} end\n\
    save_last()\n\
end\n\
--Get a previously saved value, if it has one\n\
function MANAGER_GETSETTING(ident,name)\n\
    if settings[ident] then return settings[ident][name] end\n\
    return nil\n\
end\n\
--delete a setting, leave name nil to delete all of ident\n\
function MANAGER_DELSETTING(ident,name)\n\
    if settings[ident] then\n\
	if name then settings[ident][name]=nil\n\
	else settings[ident]=nil end\n\
	save_last()\n\
    end\n\
end\n\
--some other API functions here..\n\
\n\
--mniip's download thing (mostly)\n\
local pattern = \"http:\/\/w*%.?(.-)(/.*)\"\n\
local function download_file(url)\n\
    local _,_,host,rest = url:find(pattern)\n\
    if not host or not rest then MANAGER_PRINT(\"Bad link\") return end\n\
    local conn=socket.tcp()\n\
    if not conn then return end\n\
    local succ=pcall(conn.connect,conn,host,80)\n\
    conn:settimeout(5)\n\
    if not succ then return end\n\
    local userAgent = \"PowderToy/\"..tpt.version.major..\".\"..tpt.version.minor..\".\"..tpt.version.build..\" (\"..(WINDOWS and \"WIN; \" or \"LIN; \")..(tpt.version.jacob1s_mod and \"M1\" or \"M0\")..\") SCRIPT/\"..VERSION\n\
    succ,resp,something=pcall(conn.send,conn,\"GET \"..rest..\" HTTP/1.1\\r\\nHost: \"..host..\"\\r\\nConnection: close\\r\\nUser-Agent: \"..userAgent..\"\\r\\n\\n\")\n\
    if not succ then return end\n\
    local data=\"\"\n\
    local c=\"\"\n\
    while c do\n\
        c=conn:receive(\"*l\")\n\
        if c then\n\
            data=data..\"\\n\"..c\n\
        end\n\
    end\n\
    if data==\"\" then MANAGER_PRINT(\"no data\") return end\n\
    local first,last,code = data:find(\"HTTP/1%.1 (.-) .-\\n\")\n\
    while last do\n\
        data = data:sub(last+1)\n\
        first,last,header = data:find(\"^([^\\n]-:.-)\\n\")\n\
        --read something from headers?\n\
        if header then\n\
            if tonumber(code)==302 then\n\
                local _,_,new = header:find(\"^Location: (.*)\")\n\
                if new then return download_file(new) end\n\
            end\n\
        end\n\
    end\n\
    if host:find(\"pastebin.com\") then --pastebin adds some weird numbers\n\
        _,_,data=data:find(\"\\n[^\\n]*\\n(.*)\\n.+\\n$\")\n\
    end\n\
    return data\n\
end\n\
--Downloads to a location\n\
local function download_script(ID,location)\n\
    local file = download_file(\"http:\/\/starcatcher.us/scripts/main.lua?get=\"..ID)\n\
    if file then\n\
        f=io.open(location,\"w\")\n\
        f:write(file)\n\
        f:close()\n\
        return true\n\
    end\n\
    return false\n\
end\n\
--Restart exe (if named correctly)\n\
local function do_restart()\n\
    save_last()\n\
    if WINDOWS then\n\
        os.execute(\"TASKKILL /IM \\\"\"..EXE_NAME..\"\\\" /F &&START .\\\\\\\"\"..EXE_NAME..\"\\\"\")\n\
    else\n\
        os.execute(\"killall -s KILL \\\"\"..EXE_NAME..\"\\\" && ./\\\"\"..EXE_NAME..\"\\\"\")\n\
    end\n\
    MANAGER_PRINT(\"Restart failed, do you have the exe name right?\",255,0,0)\n\
end\n\
--TPT interface\n\
local function step()\n\
    if tpt.version.jacob1s_mod then\n\
        tpt.fillrect(0,0,gfx.WIDTH,gfx.HEIGHT,0,0,0,150)\n\
    else\n\
        tpt.fillrect(-1,-1,gfx.WIDTH,gfx.HEIGHT,0,0,0,150)\n\
    end\n\
    mainwindow:draw()\n\
    tpt.drawtext(280,140,\"Console Output:\")\n\
    if requiresrestart then\n\
        tpt.drawtext(280,88,\"Disabling a script requires a restart for effect!\",255,50,50)\n\
    end\n\
    tpt.drawtext(55,55,\"Click a script to toggle, hit DONE when finished\")\n\
    tpt.drawtext(474,55,\"Script Manager v\"..VERSION)--479 for simple versions\n\
    tooltip:draw()\n\
end\n\
local function mouseclick(mousex,mousey,button,event,wheel)\n\
    sidebutton:process(mousex,mousey,button,event,wheel)\n\
    if hidden_mode then return true end\n\
\n\
    if mousex>612 or mousey>384 then return false end\n\
    mainwindow:process(mousex,mousey,button,event,wheel)\n\
    return false\n\
end\n\
local function keypress(key,nkey,modifier,event)\n\
    if nkey==27 and not hidden_mode then hidden_mode=true return false end\n\
    if not hidden_mode then return false end\n\
end\n\
--small button on right to bring up main menu\n\
local menusAdded = 1\n\
local WHITE = {255,255,255,255}\n\
local BLACK = {0,0,0,255}\n\
local sidecoords = {612,134 - menusAdded * 16,16,17}\n\
local ICON = math.random(2) --pick a random icon\n\
local lua_letters= {{{615,136,615,141},{615,141,617,141},{619,141,619,145},{619,145,621,145},{621,141,621,145},{623,145,625,145},{623,145,623,149},{624,147,624,147},{625,145,625,149},},\n\
    {{615,137,615,147},{615,148,620,148},{617,137,617,146},{617,146,620,146},{620,137,620,146},{622,137,625,137},{622,137,622,148},{623,142,624,142},{625,137,625,148},}}\n\
local function smallstep()\n\
    tpt.drawrect(613,135 - menusAdded * 16,14,15,200,200,200)\n\
    local color=WHITE\n\
    if not hidden_mode then\n\
		step()\n\
		tpt.fillrect(unpack(sidecoords))\n\
		color=BLACK\n\
	end\n\
    for i,dline in ipairs(lua_letters[ICON]) do\n\
        tpt.drawline(dline[1],dline[2] - menusAdded * 16,dline[3],dline[4] - menusAdded * 16,color[1],color[2],color[3])\n\
    end\n\
end\n\
--button functions on click\n\
function ui_button.reloadpressed(self)\n\
    load_filenames()\n\
    gen_buttons()\n\
    mainwindow.checkbox:updatescroll()\n\
    if num_files == 0 then\n\
        MANAGER_PRINT(\"No scripts found in '\"..TPT_LUA_PATH..\"' folder\",255,255,0)\n\
        fs.makeDirectory(TPT_LUA_PATH)\n\
    else\n\
        MANAGER_PRINT(\"Reloaded file list, found \"..num_files..\" scripts\")\n\
    end\n\
end\n\
function ui_button.selectnone(self)\n\
    for i,but in ipairs(mainwindow.checkbox.list) do\n\
        but.selected = false\n\
    end\n\
end\n\
function ui_button.consoleclear(self)\n\
    mainwindow.menuconsole:clear()\n\
end\n\
function ui_button.changeexename(self)\n\
    local last = EXE_NAME\n\
    local new = tpt.input(\"Change exe name\",\"Enter the exact name of powder toy executable\",EXE_NAME,EXE_NAME)\n\
    if new~=last and new~=\"\" then\n\
        MANAGER_PRINT(\"Executable name changed to \"..new,255,255,0)\n\
        EXE_NAME = new\n\
    end\n\
    save_last()\n\
end\n\
function ui_button.changedir(self)\n\
    local last = TPT_LUA_PATH\n\
    local new = tpt.input(\"Change search directory\",\"Enter the folder where your scripts are\",TPT_LUA_PATH,TPT_LUA_PATH)\n\
    if new~=last and new~=\"\" then\n\
        MANAGER_PRINT(\"Directory changed to \"..new,255,255,0)\n\
        TPT_LUA_PATH = new\n\
    end\n\
    ui_button.reloadpressed()\n\
    save_last()\n\
end\n\
function ui_button.uploadscript(self)\n\
    local command = WINDOWS and \"start\" or \"xdg-open\"\n\
    os.execute(command..\" http:\/\/starcatcher.us/scripts/paste.lua\")\n\
end\n\
local lastpaused\n\
function ui_button.sidepressed(self)\n\
    hidden_mode = not hidden_mode\n\
    ui_button.localview()\n\
    if not hidden_mode then\n\
        lastpaused = tpt.set_pause()\n\
        tpt.set_pause(1)\n\
        ui_button.reloadpressed()\n\
    else\n\
        tpt.set_pause(lastpaused)\n\
    end\n\
end\n\
local donebutton\n\
function ui_button.donepressed(self)\n\
    hidden_mode = true\n\
    for i,but in ipairs(mainwindow.checkbox.list) do\n\
        local filepath = but.ID and localscripts[but.ID][\"path\"] or but.t.text\n\
        if but.selected then\n\
            if requiresrestart then\n\
                running[filepath] = true\n\
            else\n\
                if not running[filepath] then\n\
                    local status,err = pcall(dofile,TPT_LUA_PATH..PATH_SEP..filepath)\n\
                    if not status then\n\
                        MANAGER_PRINT(err,255,0,0)\n\
                        print(err)\n\
                        but.selected = false\n\
                    else\n\
                        MANAGER_PRINT(\"Started \"..filepath)\n\
                        running[filepath] = true\n\
                    end\n\
                end\n\
            end\n\
        elseif running[filepath] then\n\
            running[filepath] = nil\n\
        end\n\
    end\n\
    if requiresrestart then do_restart() return end\n\
    save_last()\n\
end\n\
function ui_button.downloadpressed(self)\n\
    for i,but in ipairs(mainwindow.checkbox.list) do\n\
        if but.selected then\n\
            --maybe do better display names later\n\
            local displayName\n\
            local function get_script(butt)\n\
                local script = download_file(\"http:\/\/starcatcher.us/scripts/main.lua?get=\"..butt.ID)\n\
                displayName = \"downloaded\"..PATH_SEP..butt.ID..\" \"..onlinescripts[butt.ID].author..\"-\"..onlinescripts[butt.ID].name..\".lua\"\n\
                local name = TPT_LUA_PATH..PATH_SEP..displayName\n\
                if not fs.exists(TPT_LUA_PATH..PATH_SEP..\"downloaded\") then\n\
                    fs.makeDirectory(TPT_LUA_PATH..PATH_SEP..\"downloaded\")\n\
                end\n\
                local file = io.open(name, \"w\")\n\
                if not file then error(\"could not open \"..name) end\n\
                file:write(script)\n\
                file:close()\n\
                localscripts[butt.ID] = onlinescripts[butt.ID]\n\
                localscripts[butt.ID][\"path\"] = displayName\n\
                dofile(name)\n\
            end\n\
            local status,err = pcall(get_script, but)\n\
            if not status then\n\
                MANAGER_PRINT(err,255,0,0)\n\
                print(err)\n\
                but.selected = false\n\
            else\n\
                MANAGER_PRINT(\"Downloaded and started \"..but.t.text)\n\
                running[displayName] = true\n\
            end\n\
        end\n\
    end\n\
    hidden_mode=true\n\
    ui_button.localview()\n\
    save_last()\n\
end\n\
\n\
function ui_button.pressed(self)\n\
    self.selected = not self.selected\n\
end\n\
function ui_button.delete(self)\n\
    --there is no tpt.confirm() yet\n\
    if tpt.input(\"Delete File\", \"Delete \"..self.t.text..\"?\", \"yes\", \"no\") == \"yes\" then\n\
        local filepath = self.ID and localscripts[self.ID][\"path\"] or self.t.text\n\
        fs.removeFile(TPT_LUA_PATH..\"/\"..filepath:gsub(\"\\\\\",\"/\"))\n\
        if running[filepath] then running[filepath] = nil end\n\
        if localscripts[self.ID] then localscripts[self.ID] = nil end\n\
        save_last()\n\
        ui_button.localview()\n\
        load_filenames()\n\
        gen_buttons()\n\
    end\n\
end\n\
function ui_button.scriptcheck(self)\n\
    if download_script(self.ID,TPT_LUA_PATH..PATH_SEP..localscripts[self.ID][\"path\"]) then\n\
        self.canupdate = false\n\
        localscripts[self.ID][\"version\"] = onlinescripts[self.ID][\"version\"]\n\
        if running[localscripts[self.ID][\"path\"]] then\n\
            do_restart()\n\
        end\n\
    end\n\
end\n\
function ui_button.doupdate(self)\n\
    fileSystem.move(\"autorun.lua\", \"autorunold.lua\")\n\
    download_script(1, 'autorun.lua')\n\
    localscripts[1] = updatetable[1]\n\
    do_restart()\n\
end\n\
function ui_button.localview(self)\n\
    if online then\n\
        online = false\n\
        gen_buttons()\n\
        donebutton.t.text = \"DONE\"\n\
        donebutton.w = 29 donebutton.x2 = donebutton.x + donebutton.w\n\
        donebutton.f = ui_button.donepressed\n\
    end\n\
end\n\
function ui_button.onlineview(self)\n\
    if not online then\n\
        online = true\n\
        gen_buttons()\n\
        donebutton.t.text = \"DOWNLOAD\"\n\
        donebutton.w = 55 donebutton.x2 = donebutton.x + donebutton.w\n\
        donebutton.f = ui_button.downloadpressed\n\
    end\n\
end\n\
--add buttons to window\n\
donebutton = ui_button.new(55,339,29,10,ui_button.donepressed,\"DONE\")\n\
mainwindow:add(donebutton)\n\
mainwindow:add(ui_button.new(134,339,40,10,ui_button.sidepressed,\"CANCEL\"))\n\
--mainwindow:add(ui_button.new(152,339,29,10,ui_button.selectnone,\"NONE\"))\n\
local nonebutton = ui_button.new(62,81,8,8,ui_button.selectnone,\"\")\n\
nonebutton.drawbox = true\n\
mainwindow:add(nonebutton)\n\
mainwindow:add(ui_button.new(538,339,33,10,ui_button.consoleclear,\"CLEAR\"))\n\
mainwindow:add(ui_button.new(278,67,40,10,ui_button.reloadpressed,\"RELOAD\"))\n\
mainwindow:add(ui_button.new(333,67,80,10,ui_button.changeexename,\"Change exe name\"))\n\
mainwindow:add(ui_button.new(428,67,51,10,ui_button.changedir,\"Change dir\"))\n\
mainwindow:add(ui_button.new(493,67,68,10,ui_button.uploadscript,\"Upload Script\"))\n\
local tempbutton = ui_button.new(60, 65, 30, 10, ui_button.localview, \"Local\")\n\
tempbutton.drawbox = true\n\
mainwindow:add(tempbutton)\n\
tempbutton = ui_button.new(100, 65, 35, 10, ui_button.onlineview, \"Online\")\n\
tempbutton.drawbox = true\n\
mainwindow:add(tempbutton)\n\
sidebutton = ui_button.new(613,134,14,15,ui_button.sidepressed,'')\n\
\n\
local function gen_buttons_local()\n\
    local count = 0\n\
    local sorted = {}\n\
    for k,v in pairs(localscripts) do if v.ID ~= 1 then table.insert(sorted, v) end end\n\
    table.sort(sorted, function(first,second) return first.name:lower() < second.name:lower() end)\n\
    for i,v in ipairs(sorted) do\n\
        local check = mainwindow.checkbox:add(ui_button.pressed,ui_button.delete,v.name)\n\
        check.ID = v.ID\n\
        if running[v.path] then\n\
            check.running = true\n\
            check.selected = true\n\
        end\n\
        count = count + 1\n\
    end\n\
    if #sorted >= 5 and #filenames >= 5 then\n\
        mainwindow.checkbox:add(nil, nil, \"\") --empty space to separate things\n\
    end\n\
    for i=1,#filenames do\n\
        local check = mainwindow.checkbox:add(ui_button.pressed,ui_button.delete,filenames[i])\n\
        if running[filenames[i]] then\n\
            check.running = true\n\
            check.selected = true\n\
        end\n\
    end\n\
    num_files = count + #filenames\n\
end\n\
local function gen_buttons_online()\n\
    local list = download_file(\"http:\/\/starcatcher.us/scripts/main.lua\")\n\
    onlinescripts = readScriptInfo(list)\n\
    local sorted = {}\n\
    for k,v in pairs(onlinescripts) do table.insert(sorted, v) end\n\
    table.sort(sorted, function(first,second) return first.ID < second.ID end)\n\
    for k,v in pairs(sorted) do\n\
        local check = mainwindow.checkbox:add(ui_button.pressed, nil, v.name)\n\
        check.ID = v.ID\n\
        check.checkbut.ID = v.ID\n\
        if localscripts[v.ID] then\n\
            check.running = true\n\
            if tonumber(v.version) > tonumber(localscripts[check.ID].version) then\n\
                check.checkbut.canupdate = true\n\
            end\n\
        end\n\
    end\n\
    if first_online then\n\
        first_online = false\n\
        local updateinfo = download_file(\"http:\/\/starcatcher.us/scripts/main.lua?info=1\")\n\
        updatetable = readScriptInfo(updateinfo)\n\
        if not updatetable[1] then return end\n\
        if tonumber(updatetable[1].version) > scriptversion then\n\
            local updatebutton = ui_button.new(278,127,40,10,ui_button.doupdate,\"UPDATE\")\n\
            updatebutton.t:setcolor(25,255,25)\n\
            mainwindow:add(updatebutton)\n\
            MANAGER_PRINT(\"A script manager update is available! Click UPDATE\",25,255,55)\n\
            MANAGER_PRINT(updatetable[1].changelog,25,255,55)\n\
        end\n\
    end\n\
end\n\
gen_buttons = function()\n\
    mainwindow.checkbox:clear()\n\
    if online then\n\
        gen_buttons_online()\n\
    else\n\
        gen_buttons_local()\n\
    end\n\
    mainwindow.checkbox:updatescroll()\n\
end\n\
gen_buttons()\n\
\n\
--register manager first\n\
tpt.register_step(smallstep)\n\
--load previously running scripts\n\
local started = \"\"\n\
for prev,v in pairs(running) do\n\
    local status,err = pcall(dofile,TPT_LUA_PATH..PATH_SEP..prev)\n\
    if not status then\n\
        MANAGER_PRINT(err,255,0,0)\n\
        running[prev] = nil\n\
    else\n\
        started=started..\" \"..prev\n\
        local newbut = mainwindow.checkbox:add(ui_button.pressed,prev)\n\
        newbut.selected=true\n\
    end\n\
end\n\
save_last()\n\
if started~=\"\" then\n\
    MANAGER_PRINT(\"Auto started\"..started)\n\
end\n\
tpt.register_mouseevent(mouseclick)\n\
tpt.register_keypress(keypress)\n\
"
