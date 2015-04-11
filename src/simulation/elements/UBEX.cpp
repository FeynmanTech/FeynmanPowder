#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_UBEX PT_UBEX 188
Element_UBEX::Element_UBEX()
{
	Identifier = "DEFAULT_PT_UBEX";
	Name = "UBEX";
	Colour = PIXPACK(0x5010B0);
	MenuVisible = 1;
	MenuSection = SC_EXPLOSIVE;
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
	HeatConduct = 88;
	Description = "Uber explosive. Extremely volatile, creates lots of pressure on detonation.";
	
	State = ST_SOLID;
	Properties = TYPE_SOLID | PROP_NEUTPENETRATE;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
	
	Update = &Element_UBEX::update;
	
}

//#TPT-Directive ElementHeader Element_UBEX static int update(UPDATE_FUNC_ARGS)
int Element_UBEX::update(UPDATE_FUNC_ARGS)
 {
	int r, rx, ry, nb;
	if(parts[i].tmp==0)
	{
		if(parts[i].temp>=673.0f)
			parts[i].tmp = 1;
		else
			for (rx=-1; rx<2; rx++)
				for (ry=-1; ry<2; ry++)
					if (BOUNDS_CHECK && (rx || ry))
					{
						r = pmap[y+ry][x+rx];
						if (!r)
							continue;
						if (
							(r&0xFF)==PT_FIRE || (r&0xFF)==PT_PLSM || (r&0xFF)==PT_SPRK || (r&0xFF)==PT_LIGH || 
							sim->pv[y/CELL][x/CELL] > 3.f
						)
						{
							parts[i].tmp = 1;
						}
					}
	
	}
	else if(parts[i].tmp==1)
	{
		if ((pmap[y][x]>>8 == i))
		{
			PropertyValue value;
			value.Integer = 2;
			sim->flood_prop(x, y, offsetof(Particle, tmp), value, StructProperty::Integer);
		}
		parts[i].tmp = 2;
	}
	else if(parts[i].tmp==2)
	{
		parts[i].tmp = 3;
	}
	else
	{
		float otemp = parts[i].temp-273.15f;
		//Explode!!
		sim->pv[y/CELL][x/CELL] += 10.f;
		parts[i].tmp = 0;
		
		for (int cx = -1; cx <= 1; cx+=2)
		{
			for (int cy = -1; cy <= 1; cy+=2)
			{
				if(!(rand()%5))
				{
					sim->create_part(-3, x+cx, y+cy, PT_PLSM);
				}
				else
				{
					sim->create_part(-3, x+cx, y+cy, PT_FIRE);
					parts[i].life = rand()%50+500;
				}
				sim->create_part(i, x, y, PT_PLSM);
			}
		}
		if (rand()%10)
		{
			if(!(rand()%15))
			{
				sim->create_part(i, x, y, PT_EMBR);
				parts[i].tmp = 0;
				parts[i].life = 50;
				parts[i].temp = restrict_flt((MAX_TEMP)+otemp, MIN_TEMP, MAX_TEMP);
				parts[i].vx = rand()%20-10;
				parts[i].vy = rand()%20-10;
			}
			else
			{
				sim->kill_part(i);
			}
		}
		return 1;
	}
	return 0;
}


Element_UBEX::~Element_UBEX() {}
