#include "simulation/Elements.h"

//#TPT-Directive ElementClass Element_FRCT PT_FRCT 183
Element_FRCT::Element_FRCT()
{
	Identifier = "DEFAULT_PT_FRCT";
	Name = "FRCT";
	Colour = PIXPACK(0x8000FF);
	MenuVisible = 1;
	MenuSection = SC_FEYNMAN;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.00f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;

	Weight = 100;

	Temperature = R_TEMP+0.0f +273.15f;
	HeatConduct = 0;
	Description = "Fractals!";

	State = ST_SOLID;
	Properties = TYPE_SOLID;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Graphics = &Element_FRCT::graphics;

}


//#TPT-Directive ElementHeader Element_FRCT static int graphics(GRAPHICS_FUNC_ARGS)
int Element_FRCT::graphics(GRAPHICS_FUNC_ARGS)

{
	*colr = (int)(cpart->x) % 255;
	*colg = (int)(cpart->y) % 255;
	*colb = (int)(cpart->x + cpart->y) % 255;
	return 0;
}


Element_FRCT::~Element_FRCT() {}
