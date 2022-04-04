#pragma once

#include "../BaseGraphics/Canvas.h"

class SDLCanvas :
	public Canvas<Color>
{
public:
	SDLCanvas(SDL_Window*);

	void DrawPoint(int x, int y, const Color& color);

	void DrawLine(int x0, int y0, int x1, int y1, const Color& color);

	void DrawFilledRect(int x0, int y0, int w, int h, const Color& color);

	void DrawRect(int x0, int y0, int w, int h, const Color& color);

	void DrawText(int x, int y, int w, int h, const char* text, const Color& color);

	inline int Height() const
	{
		return _h;
	}

	inline int Witdth() const
	{
		return _w;
	}

	void Clear(const Color& color);

	void Clear();

	void Update();

	virtual ~SDLCanvas();

private:
	int _w, _h;

	SDL_Renderer* _sdl;

	const Color _background;
};

