#pragma once

#include "Color.h"
#include "Canvas.h"

class SDLCanvas :
	public Canvas<>
{
public:
	SDLCanvas(SDL_Window*, int);

	SDLCanvas(SDL_Window*);

	void DrawPoint(int x, int y, const Color& color);

	void DrawLine(int x0, int y0, int x1, int y1, const Color& color);

	inline int Height() const
	{
		return _h;
	}

	inline int Witdth() const
	{
		return _w;
	}

	inline unsigned int Bpp() const
	{
		return _bpp;
	}

	void Clear(const Color& color);

	void Clear();

	void Update();

	virtual ~SDLCanvas();

private:
	int _w, _h, _bpp;

	SDL_Renderer* _sdl;

	Color3D _background = Color::Black;
};

