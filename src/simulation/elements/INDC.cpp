#include "simulation/Elements.h"
#include "simulation/Air.h"
//#TPT-Directive ElementClass Element_INDC PT_INDC 181
Element_INDC::Element_INDC()
{
	Identifier = "DEFAULT_PT_INDC";
	Name = "INDC";
	Colour = PIXPACK(0x606060);
	MenuVisible = 1;
	MenuSection = SC_ELEC;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.0f;
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

	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 0;
	Description = "Indestructible conductor. Blocks all heat and pressure.";

	State = ST_SOLID;
	Properties = TYPE_SOLID|PROP_CONDUCTS|PROP_LIFE_DEC;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &Element_INDC::update;
}

//#TPT-Directive ElementHeader Element_INDC static int update(UPDATE_FUNC_ARGS)
int Element_INDC::update(UPDATE_FUNC_ARGS)
 {
	sim->air->bmap_blockair[y/CELL][x/CELL] = 1;
	sim->air->bmap_blockairh[y/CELL][x/CELL] = 0x8;
	return 0;
}

Element_INDC::~Element_INDC() {}
