#pragma once

#include "UIElement.h"
 
class UILabel : public UIElement
{
public:
	UILabel(const UIRect& rect, const char* text, const char* fontName, int size);

	inline void setText(const char* text)
	{
		_text = text;
		Invalidate();
	}

	inline void setForecolor(const Color& color)
	{
		_forecolor = color;
		Invalidate();
	}

	void Draw(Canvas<Color>& canvas);

private:
	Color _forecolor;
	std::string  _text;

	int _fontSize;
};