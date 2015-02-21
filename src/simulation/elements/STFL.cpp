#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_STFL PT_STFL 180
// Pretty much just a stronger and shorter NPLM
Element_STFL::Element_STFL()
{
	Identifier = "FMOD_PT_STFL";
	Name = "STFL";
	Colour = PIXPACK(0xFFC090);
	MenuVisible = 1;
	MenuSection = SC_FEYNMAN;
	Enabled = 1;

	Advection = 0.4f;
	AirDrag = 0.04f * CFDS;
	AirLoss = 0.94f;
	Loss = 0.95f;
	Collision = -0.1f;
	Gravity = 0.3f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 2;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 2;

	Weight = 90;

	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 150;
	Description = "Starter fluid. Burns extremely hot. Can be ignited by fire, plasma, or temperatures above 1000 degrees.";

	State = ST_SOLID;
	Properties = TYPE_PART;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &Element_STFL::update;
}

//#TPT-Directive ElementHeader Element_STFL static int update(UPDATE_FUNC_ARGS)
int Element_STFL::update(UPDATE_FUNC_ARGS)
 {
	int r, rx, ry, trade, temp, t = parts[i].type;

	for (int cx = -1; cx <= 1; cx ++)
	{
		for (int cy = -1; cy <= 1; cy++)
		{
			if ((parts[pmap[y + cy][x + cx]>>8].type == PT_FIRE || parts[pmap[y + cy][x + cx]>>8].type == PT_PLSM || parts[i].temp >= 1273.15) && parts[i].tmp2 != 1)
			{
				parts[i].life = 100;
				parts[i].tmp2 = 1;
			}
		}
	}
	if (parts[i].life<=0 && parts[i].tmp2 == 1) {
		sim->create_part(i, x, y, PT_PLSM);
		return 1;
	} else if (parts[i].life > 0 && parts[i].tmp2 == 1) {
		parts[i].life--;
		sim->create_part(-1, x+rand()%3-1, y+rand()%3-1, PT_PLSM);
	}

	return 0;
}

Element_STFL::~Element_STFL() {}
