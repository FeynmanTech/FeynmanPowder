#ifndef USEROPTIONSVIEW_H_
#define USEROPTIONSVIEW_H_

#include "gui/interface/Window.h"
#include "OptionsController.h"
#include "gui/interface/Checkbox.h"
#include "gui/interface/DropDown.h"
#include "gui/interface/Label.h"
#include "OptionsModel.h"
#include "simulation/Simulation.h"
#include "lua/LuaScriptInterface.h"
#include "lua/LuaScriptHelper.h"

class GameModel;
class Simulation;

class OptionsModel;
class OptionsController;

class UserOptionsView: public ui::Window {
	ui::Checkbox * heatSimulation;
	ui::Checkbox * ambientHeatSimulation;
	ui::Checkbox * newtonianGravity;
	ui::Checkbox * waterEqualisation;
	ui::DropDown * airMode;
	ui::DropDown * gravityMode;
	ui::DropDown * edgeMode;
	ui::Checkbox * scale;
	ui::Checkbox * cur;
	ui::Checkbox * server;
	ui::Checkbox * fullscreen;
	ui::Checkbox * fastquit;
	ui::Checkbox * showAvatars;
	int currentPosition;
	ui::Checkbox * options[128];
	ui::Label * tempLabel;
public:
	OptionsController * c;
	UserOptionsView();
	int AddOption(char * header, char * description);
	int GetOptionState(int index);
	void NotifySettingsChanged(OptionsModel * sender);
	void AttachController(OptionsController * c_);
	void OnDraw();
	void OnTryExit(ExitMethod method);
	virtual ~UserOptionsView();
};

class GenericAction: public ui::CheckboxAction
{
	UserOptionsView * v;
	int index;
	lua_State * l;
public:
	GenericAction(UserOptionsView * v_, int _index){	v = v_;	index = _index;}
	virtual void ActionCallback(ui::Checkbox * sender){
		l = luacon_ci->l;
		lua_getglobal(l, "tpt");
		if (lua_istable(l, lua_gettop(l)))
		{
			lua_getfield(l, lua_gettop(l), "USER_OPTIONS");
			int uo_index = lua_gettop(l);
			lua_pushinteger(l, index);
			lua_gettable(l, uo_index);
			if (lua_isfunction(l, lua_gettop(l)))
			{
				if (sender->GetChecked()) lua_pushinteger(l, 1); else lua_pushinteger(l, 0);
				lua_call(l, 1, 1);
				if (!lua_isnil(l, lua_gettop(l)))
					sender->SetChecked(lua_toboolean(l, lua_gettop(l)));
			}
		} else
		{
			std::cout << "Cannot find tpt!\n";
		}
	}
};

#endif /* OPTIONSVIEW_H_ */
