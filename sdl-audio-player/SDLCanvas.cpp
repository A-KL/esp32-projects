#include <SDL.h>

#include "../lib/Color3D.h"
#include "../lib/Canvas.h"

#include "SDLCanvas.h"

SDLCanvas::SDLCanvas(SDL_Window* window, int bpp)
{
	SDL_GetWindowSize(window, &_w, &_h);

	_bpp = bpp;

	_sdl = SDL_CreateRenderer(window, -1, 0);
}

SDLCanvas::SDLCanvas(SDL_Window* window)
	: SDLCanvas(window, 32)
{ }

void SDLCanvas::DrawPoint(int x, int y, const Color3D& color)
{
	SDL_SetRenderDrawColor(_sdl, color.RToByte(), color.GToByte(), color.BToByte(), 0);

	SDL_RenderDrawPoint(_sdl, x, y);
}

void SDLCanvas::DrawLine(int x0, int y0, int x1, int y1, const Color3D& color)
{
	SDL_SetRenderDrawColor(_sdl, color.RToByte(), color.GToByte(), color.BToByte(), 0);

	SDL_RenderDrawLine(_sdl, x0, y0, x1, y1);
}

void SDLCanvas::Clear(const Color3D& color)
{
	SDL_SetRenderDrawColor(_sdl, color.RToByte(), color.GToByte(), color.BToByte(), 0);

	SDL_RenderClear(_sdl);
}

void SDLCanvas::Clear()
{
	Clear(_background);
}

void SDLCanvas::Update()
{
	SDL_RenderPresent(_sdl);
}

SDLCanvas::~SDLCanvas()
{
	SDL_DestroyRenderer(_sdl);
}
