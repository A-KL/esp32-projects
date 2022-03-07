#pragma once

#include "../Canvas/Canvas.h"

class M5StackCanvas :
	public Canvas<Color>
{
public:
	M5StackCanvas(M5GFX& display) :
		_display(&display), _background(0, 0, 0)
	{}

	void Init(const Color& color);

	void DrawPoint(int x, int y, const Color& color);

	void DrawLine(int x0, int y0, int x1, int y1, const Color& color);

	void DrawFilledRect(int x0, int y0, int w, int h, const Color& color);

	void DrawText(int x0, int y0, const char* text, int size, const Color& color);

	inline int Height() const
	{
		return _display->height();
	}

	inline int Witdth() const
	{
		return _display->width();
	}

	inline unsigned int Bpp() const
	{
		return 24;
	}

	void Clear(const Color& color);

	void Clear();

	void Update();

	virtual ~M5StackCanvas();

private:
	M5GFX* _display;

	const Color _background;
};

