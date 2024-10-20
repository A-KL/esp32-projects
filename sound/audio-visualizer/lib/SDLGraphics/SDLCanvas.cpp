#include <SDL.h>
#include <SDL_ttf.h>

#include "../BaseGraphics/Color.h"
#include "../BaseGraphics/Canvas.h"

#include "SDLCanvas.h"

SDLCanvas::SDLCanvas(SDL_Window* window)
	: _background(0, 0, 0)
{
	SDL_GetWindowSize(window, &_w, &_h);

	_sdl = SDL_CreateRenderer(window, -1, 0);
}

void SDLCanvas::DrawPoint(int x, int y, const Color& color)
{
	SDL_SetRenderDrawColor(_sdl, color._r, color._g, color._b, color._a);

	SDL_RenderDrawPoint(_sdl, x, y);
}

void SDLCanvas::DrawLine(int x0, int y0, int x1, int y1, const Color& color)
{
	SDL_SetRenderDrawColor(_sdl, color._r, color._g, color._b, color._a);

	SDL_RenderDrawLine(_sdl, x0, y0, x1, y1);
}

void SDLCanvas::DrawFilledRect(int x0, int y0, int w, int h, const Color& color)
{
	SDL_SetRenderDrawColor(_sdl, color._r, color._g, color._b, color._a);

	SDL_Rect rect { x0, y0, w, h };
	SDL_RenderFillRect(_sdl, &rect);
}

void SDLCanvas::DrawRect(int x0, int y0, int w, int h, const Color& color)
{
	SDL_SetRenderDrawColor(_sdl, color._r, color._g, color._b, color._a);

	SDL_Rect rect { x0, y0, w, h };
	SDL_RenderDrawRect(_sdl, &rect);
}

void SDLCanvas::DrawText(int x, int y, int w, int h, const char* text, const Color& color)
{
	auto fontName = "C:/Windows/Fonts/Calibril.ttf";

	auto font = TTF_OpenFont(fontName, 16);

	SDL_Surface* surfaceMessage =
		TTF_RenderText_Solid(font, text, *(SDL_Color*)&color);

	SDL_Texture* Message =
		SDL_CreateTextureFromSurface(_sdl, surfaceMessage);

	SDL_Rect text_rect{ x, y, surfaceMessage->w, surfaceMessage->h };

	SDL_RenderCopy(_sdl, Message, NULL, &text_rect);

	SDL_FreeSurface(surfaceMessage);

	SDL_DestroyTexture(Message);
}

void SDLCanvas::SetFont(int index, unsigned char size)
{

}

void SDLCanvas::Clear(const Color& color)
{
	SDL_SetRenderDrawColor(_sdl, color._r, color._g, color._b, color._a);

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
