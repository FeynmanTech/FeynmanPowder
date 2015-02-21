#include "simulation/Elements.h"
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <bitset>
#include <string>

#include "lua/LuaScriptInterface.h"
#include "lua/TPTScriptInterface.h"
#include "lua/LuaScriptHelper.h"
//#TPT-Directive ElementClass Element_BDST PT_BDST 182
Element_BDST::Element_BDST()
{
	Identifier = "DEFAULT_PT_BDST";
	Name = "BDST";
	Colour = PIXPACK(0xD0D0D0);
	MenuVisible = 1;
	MenuSection = SC_ELEC;
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

	Temperature = R_TEMP+0.0f +273.15f;
	HeatConduct = 0;
	Description = "Binary Data Storage";

	State = ST_SOLID;
	Properties = TYPE_SOLID|PROP_LIFE_DEC;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &Element_BDST::update;

}

//#TPT-Directive ElementHeader Element_BDST static int update(UPDATE_FUNC_ARGS)
int Element_BDST::update(UPDATE_FUNC_ARGS)
 {
	int r, nxx, nyy, docontinue, nxi, nyi, rx, ry, nr, ry1, rx1;
	if (parts[i].life==0) {
		for (rx=-1; rx<2; rx++)
			for (ry=-1; ry<2; ry++)
				if (BOUNDS_CHECK && (rx || ry))
				{
					r = pmap[y+ry][x+rx];
					if (!r)
						continue;
					if (rx && ry) // Don't spark from diagonals
							continue;
					if ((r&0xFF)==PT_SPRK && parts[r>>8].life==3 && parts[r>>8].ctype==PT_NSCN) {
						/*
						bool isBlackDeco = false;
						int destroy = (parts[r>>8].ctype==PT_PSCN)?1:0;
						int nostop = (parts[r>>8].ctype==PT_INST)?1:0;
						int colored = 0;
						*/
						//(2^parts[i].tmp)
						int storageData[512] = {};
						int stored = 0;
						int toStore = pow(2., parts[i].tmp);
						//toStore = 2 ^ toStore;
						for (docontinue = 1, nxx = 0, nyy = 0, nxi = rx*-1, nyi = ry*-1; docontinue; nyy+=nyi, nxx+=nxi) {
							if (!(x+nxi+nxx<XRES && y+nyi+nyy<YRES && x+nxi+nxx >= 0 && y+nyi+nyy >= 0)) {
								break;
							}
							r = pmap[y+nyi+nyy][x+nxi+nxx];
							if (r) {
								//int nr = sim->create_part(-1, x+nxi+nxx, y+nyi+nyy, PT_BRAY);
								storageData[stored] = 1;
							} else {
								storageData[stored] = 0;
							}
							stored++;
							if (stored >= toStore)
							{
								int partsData = 0;
								for (int d = 0; d < toStore; d++) {
									if (storageData[d] == 1)
										partsData += pow(2, d);
								}
								parts[i].ctype = partsData;
								return 1;
							}
						}
					} else {
						if ((r&0xFF)==PT_SPRK && parts[r>>8].life==3 && parts[r>>8].ctype==PT_PSCN) {
							std::string bin = std::bitset<sizeof(parts[i].ctype)>(parts[i].ctype).to_string();
							for (int p = 0; p < sizeof(parts[i].ctype); p++)
							{
								if (bin.substr(p, 1) == "1")
								{
									sim->wireless[(int)(parts[i].temp / 100) + p + 1][1] = 1;
									sim->ISWIRE = 2;
								}
							}
						}
					}
					//parts[i].life = 4;
				}
	}
	return 0;
}


Element_BDST::~Element_BDST() {}
