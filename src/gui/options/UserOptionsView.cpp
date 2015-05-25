#include <stdio.h>
#include <iostream>
#include <algorithm>
#ifdef WIN
	#include <direct.h>
	#define getcwd _getcwd
#else
	#include <unistd.h>
#endif

#include "UserOptionsView.h"
#include "gui/Style.h"
#include "gui/interface/Button.h"
#include "gui/interface/Label.h"
#include "gui/interface/DropDown.h"
#include "gui/dialogues/ErrorMessage.h"

#define NUM_USER_OPTIONS 256

UserOptionsView::UserOptionsView():

	ui::Window(ui::Point(-1, -1), ui::Point(300, 370)){

	//std::cout << "Window initialized\n";

	ui::Label * tempLabel = new ui::Label(ui::Point(4, 5), ui::Point(Size.X-8, 14), "User Options");
	tempLabel->SetTextColour(style::Colour::InformationTitle);
	tempLabel->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;	tempLabel->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
	AddComponent(tempLabel);

	//std::cout << "Label initialized\n";

	ui::Component * options[NUM_USER_OPTIONS];

	//std::cout << "Options initialized\n";

	int currentPosition = 0;

	class CloseAction: public ui::ButtonAction
	{
	public:
		UserOptionsView * v;
		CloseAction(UserOptionsView * v_) { v = v_; }
		void ActionCallback(ui::Button * sender)
		{
			ui::Engine::Ref().CloseWindow();
		}
	};

	//std::cout << "Close button initialized\n";

	ui::Button * tempButton = new ui::Button(ui::Point(0, Size.Y-16), ui::Point(Size.X, 16), "OK");
	tempButton->SetActionCallback(new CloseAction(this));
	AddComponent(tempButton);
	SetCancelButton(tempButton);
	SetOkayButton(tempButton);

	//std::cout << "All initialized\n";
}

int UserOptionsView::AddOption(char * header, char * description)
{
	if (currentPosition > NUM_USER_OPTIONS)
		currentPosition = 0;
	options[currentPosition] = new ui::Checkbox(ui::Point(8, 23 + currentPosition*30), ui::Point(Size.X-6, 16), header, "");
	options[currentPosition]->SetActionCallback(new GenericAction(this, currentPosition));
	AddComponent(options[currentPosition]);
	tempLabel = new ui::Label(ui::Point(22, options[currentPosition]->Position.Y+14), ui::Point(Size.X-28, 16), description);
	tempLabel->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;	tempLabel->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
	AddComponent(tempLabel);
	currentPosition++;
	return currentPosition - 1;
}

int UserOptionsView::GetOptionState(int index)
{
	if (index > currentPosition - 1)
		return 0;
	return options[index]->GetChecked();
}

void UserOptionsView::NotifySettingsChanged(OptionsModel * sender)
{
	
}

void UserOptionsView::AttachController(OptionsController * c_)
{
	c = c_;
}

void UserOptionsView::OnDraw()
{
	Graphics * g = ui::Engine::Ref().g;
	g->clearrect(Position.X-2, Position.Y-2, Size.X+3, Size.Y+3);
	g->drawrect(Position.X, Position.Y, Size.X, Size.Y, 255, 255, 255, 255);
	//g->draw_line(Position.X+1, Position.Y+scale->Position.Y-4, Position.X+Size.X-1, Position.Y+scale->Position.Y-4, 255, 255, 255, 180);
}

void UserOptionsView::OnTryExit(ExitMethod method)
{
	ui::Engine::Ref().CloseWindow();
}


UserOptionsView::~UserOptionsView() {
}

