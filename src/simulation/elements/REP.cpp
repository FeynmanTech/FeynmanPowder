#include "simulation/Elements.h"
#include <iostream>
//#TPT-Directive ElementClass Element_REP PT_REP 184
Element_REP::Element_REP()
{
	Identifier = "DEFAULT_PT_REP";
	Name = "REP";
	Colour = PIXPACK(0xFFA000);
	MenuVisible = 1;
	MenuSection = SC_FEYNMAN;
	Enabled = 1;

	Advection = 0.5f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.95f;
	Loss = 0.95f;
	Collision = 0.9f;
	Gravity = 0.5f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 2;

	Flammable = 1000;
	Explosive = 1;
	Meltable = 50;
	Hardness = 1;

	Weight = 100;

	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 240;
	Description = "Replicating Powder. Melts at 500 degrees.";

	State = ST_SOLID;
	Properties = PROP_LIFE_DEC|PROP_LIFE_KILL;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &Element_REP::update;

}

//#TPT-Directive ElementHeader Element_REP static int update(UPDATE_FUNC_ARGS)
int Element_REP::update(UPDATE_FUNC_ARGS)
 {
	int r, nxx, nyy, docontinue, nxi, nyi, rx, ry, nr, ry1, rx1;
	if (parts[i].life == 1) {
		/**
		for (int mx = -1; mx <= 1; mx++)
		{
			for (int my = -1; my <= 1; my++)
			{
				sim->create_part(-1, x + mx, y + my, PT_REP);
			}
		}
		**/
		for (int n = 0; n < parts[i].tmp; n++)
		{
			sim->create_part(-1, x + rand()%7-3, y + rand()%7-3, PT_REP);
		}
		sim->delete_part(parts[i].x, parts[i].y);
	}
	return 0;
}

Element_REP::~Element_REP() {}
