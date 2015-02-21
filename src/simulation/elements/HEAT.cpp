#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_HEAT PT_HEAT 186
Element_HEAT::Element_HEAT()
{
	Identifier = "DEFAULT_PT_HEAT";
	Name = "HEAT";
	Colour = PIXPACK(0xFF4040);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;
	
	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.90f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;
	
	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;
	
	Weight = 100;
	
	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 0;
	Description = "Heater/Cooler. If TMP=1, heats surrounding particles by its temperature, otherwise cools them.";
	
	State = ST_NONE;
	Properties = TYPE_SOLID;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
	
	Update = &Element_HEAT::update;
	
}

//#TPT-Directive ElementHeader Element_HEAT static int update(UPDATE_FUNC_ARGS)
int Element_HEAT::update(UPDATE_FUNC_ARGS)
 {
	int r, rx, ry;
	for (rx=-1; rx<2; rx++)
	{
		for (ry=-1; ry<2; ry++)
		{
			if (x+rx>=0 && y+ry>=0 && x+rx<XRES && y+ry<YRES)
			{
				r = sim->photons[y+ry][x+rx];
				if (!r)
					r = pmap[y+ry][x+rx];
				if (!r)
					continue;
				if((r&0xFF)!=PT_HEAT)
				{

					parts[r>>8].temp += parts[i].temp * (parts[i].tmp*2-1);
				}
			}
		}
	}
	return 0;
}


Element_HEAT::~Element_HEAT() {}
