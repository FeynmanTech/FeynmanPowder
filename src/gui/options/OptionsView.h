#ifndef OPTIONSVIEW_H_
#define OPTIONSVIEW_H_

#include "gui/interface/Window.h"
#include "OptionsController.h"
#include "gui/interface/Checkbox.h"
#include "gui/interface/DropDown.h"
#include "OptionsModel.h"
#include "simulation/Simulation.h"

class GameModel;
class Simulation;

class OptionsModel;
class OptionsController;
class OptionsView: public ui::Window {
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
public:
	OptionsController * c;
	OptionsView();
	void NotifySettingsChanged(OptionsModel * sender);
	void AttachController(OptionsController * c_);
	void OnDraw();
	void OnTryExit(ExitMethod method);
	virtual ~OptionsView();
};

#endif /* OPTIONSVIEW_H_ */
