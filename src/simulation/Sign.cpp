#include "Sign.h"
#include "graphics/Graphics.h"
#include "simulation/Simulation.h"
#include "Misc.h"
#include "font.h"

sign::sign(std::string text_, int x_, int y_, Justification justification_):
	text(text_),
	x(x_),
	y(y_),
	ju(justification_)
{
}

std::string sign::getText(Simulation *sim)
{
	char buff[512];
	char signText[512];
	sprintf(signText, "%s", text.substr(0, 255).c_str());

	if(signText[0] && signText[0] == '{')
	{
		if (!strcmp(signText,"{p}"))
		{
			float pressure = 0.0f;
			if (x>=0 && x<XRES && y>=0 && y<YRES)
				pressure = sim->pv[y/CELL][x/CELL];
			sprintf(buff, "Pressure: %3.2f", pressure);  //...pressure
		}
		else if (!strcmp(signText,"{t}"))
		{
			if (x>=0 && x<XRES && y>=0 && y<YRES && sim->pmap[y][x])
				sprintf(buff, "Temp: %4.2f", sim->parts[sim->pmap[y][x]>>8].temp-273.15);  //...temperature
			else
				sprintf(buff, "Temp: 0.00");  //...temperature
		}
		else if (!strcmp(signText,"{life}"))
		{
			if (x>=0 && x<XRES && y>=0 && y<YRES && sim->pmap[y][x])
				sprintf(buff, "Life: %d", sim->parts[sim->pmap[y][x]>>8].life);  //...life
			else
				sprintf(buff, "Life: 0");  //...life
		}
		else if (!strcmp(signText,"{tmp}"))
		{
			if (x>=0 && x<XRES && y>=0 && y<YRES && sim->pmap[y][x])
				sprintf(buff, "Tmp: %d", sim->parts[sim->pmap[y][x]>>8].tmp);  //...tmp
			else
				sprintf(buff, "Tmp: 0");  //...tmp
		}
		else if (!strcmp(signText,"{tmp2}"))
		{
			if (x>=0 && x<XRES && y>=0 && y<YRES && sim->pmap[y][x])
				sprintf(buff, "Tmp2: %d", sim->parts[sim->pmap[y][x]>>8].tmp2);  //...tmp2
			else
				sprintf(buff, "Tmp2: 0");  //...tmp2
		}
		else if (!strcmp(signText,"{type}"))
		{
			if (x>=0 && x<XRES && y>=0 && y<YRES && sim->pmap[y][x])
				sprintf(buff, "Type: %d", sim->parts[sim->pmap[y][x]>>8].type);  //...type
			else
				sprintf(buff, "Type: 0");  //...type
		}
		else if (!strcmp(signText,"{ctype}"))
		{
			if (x>=0 && x<XRES && y>=0 && y<YRES && sim->pmap[y][x])
				sprintf(buff, "Ctype: 0x%x", sim->parts[sim->pmap[y][x]>>8].ctype);  //...ctype
			else
				sprintf(buff, "Ctype: 0x0");  //...ctype
		}
		else
		{
			int pos=splitsign(signText);
			if (pos)
			{
				strcpy(buff, signText+pos+1);
				buff[strlen(signText)-pos-2]=0;
			}
			else
				strcpy(buff, signText);
		}
	}
	else
	{
		strcpy(buff, signText);
	}

	return std::string(buff);
}

void sign::pos(std::string signText, int & x0, int & y0, int & w, int & h)
{
	w = Graphics::textwidth(signText.c_str()) + 5;
	h = 15;
	x0 = (ju == 2) ? x - w :
	      (ju == 1) ? x - w/2 : x;
	y0 = (y > 18) ? y - 18 : y + 4;
}

int sign::height(std::string signText)
{
	/*
	char *t = new char[signText.length() + 1];
	std::strcpy(t, signText.c_str());
	t[signText.size()] = '\0';
	*/
	int h = FONT_H;
	int wordlen = signText.size() - 1;
	int found = 0;
	for (int n = 0; n < signText.size(); n++)
	{
		if (signText.substr(n, 1) == "\n")
		{
			h+=FONT_H+3;
			found = 1;
		}
	}
	if (found == 0) h+=5;
	return h;
}
