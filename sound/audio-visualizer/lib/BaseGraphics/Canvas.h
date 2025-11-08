#pragma once

template <typename TColor>
class Canvas
{
public:
	virtual void DrawLine(int x0, int y0, int x1, int y1, const TColor& color) = 0;

	virtual void DrawFilledRect(int x0, int y0, int w, int h, const TColor& color) = 0;

	virtual void DrawRect(int x0, int y0, int w, int h, const TColor& color) = 0;

	virtual void DrawText(int x, int y, int w, int h, const char* text, const TColor& color) = 0;

	virtual void SetFont(int index, unsigned char size) = 0;

	virtual void SpriteBegin(int w, int h, const Color& color) = 0;
	virtual void SpriteDrawLine(int x0, int y0, int x1, int y1, const Color& color) = 0;
	virtual void SpritePush(int x, int y) = 0;
	virtual void SpriteEnd() = 0;

	virtual int Height() const = 0;

	virtual int Width() const = 0;

	virtual void Clear(const TColor& color) = 0;

	virtual void Clear() = 0;

	virtual void Update() = 0;
};