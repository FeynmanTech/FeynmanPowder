#include "Graphics.h"
#include "simulation/Simulation.h"
#include "Config.h"
#include "Format.h"
#include "gui/game/GameModel.h"
#include "gui/game/Tool.h"
#include "Misc.h"
#include "PowderToy.h"
#include "ElementClasses.h"
#include <fstream>
#include <assert.h>

Simulation * sim;

#ifndef OGLI

Graphics::Graphics():
sdl_scale(1)
{
	vid = (pixel *)malloc(PIXELSIZE * (WINDOWW * WINDOWH));

}

void Graphics::Acquire()
{

}

void Graphics::Release()
{

}

Graphics::~Graphics()
{
	free(vid);
}

void Graphics::Clear()
{
	memset(vid, 0, PIXELSIZE * (WINDOWW * WINDOWH));
}

void Graphics::Finalise()
{

}

#define VIDXRES WINDOWW
#define VIDYRES WINDOWH
#define PIXELMETHODS_CLASS Graphics
#include "RasterDrawMethods.inl"
#undef VIDYRES
#undef VIDXRES
#undef PIXELMETHODS_CLASS

#endif
