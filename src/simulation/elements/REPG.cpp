#include "simulation/Elements.h"
#include <iostream>
//#TPT-Directive ElementClass Element_REPG PT_REPG 185
Element_REPG::Element_REPG()
{
	Identifier = "DEFAULT_PT_REPG";
	Name = "REPG";
	Colour = PIXPACK(0xFFA000);
	MenuVisible = 0;
	MenuSection = SC_FEYNMAN;
	Enabled = 1;

	Advection = 1.0f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.95f;
	Loss = 0.30f;
	Collision = -0.1f;
	Gravity = 0.0f;
	Diffusion = 0.9f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 1000;
	Explosive = 1;
	Meltable = 50;
	Hardness = 1;

	Weight = 100;

	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 240;
	Description = "Replicating Powder. Melts at 500 degrees.";

	State = ST_GAS;
	Properties = TYPE_GAS|PROP_LIFE_DEC|PROP_LIFE_KILL;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = 273.0f + 500.0f;
	LowTemperatureTransition = PT_REP;
	HighTemperature = ITL;
	HighTemperatureTransition = NT;

	Update = &Element_REPG::update;

}

//#TPT-Directive ElementHeader Element_REPG static int update(UPDATE_FUNC_ARGS)
int Element_REPG::update(UPDATE_FUNC_ARGS)
 {
	int r, nxx, nyy, docontinue, nxi, nyi, rx, ry, nr, ry1, rx1;
	if (parts[i].life == 1) {
		for (int mx = -1; mx <= 1; mx++)
		{
			for (int my = -1; my <= 1; my++)
			{
				sim->create_part(-1, x + mx, y + my, PT_REPG);
			}
		}
	}
	return 0;
}

Element_REPG::~Element_REPG() {}
