#pragma once

#include "UIElement.h"
 
class UILabel : public UIElement
{
public:
	UILabel(const UIRect& rect, const char* text);

	UILabel(const UIRect& rect, const char* text, const Color& borderColor, int borderSize);

	inline void setText(const char* text)
	{		
		if (text == _text) {
			return;
		}
		_text = text;
		Invalidate();
	}

	inline void setForecolor(const Color& color)
	{
		if (color == _forecolor) {
			return;
		}
		_forecolor = color;
		Invalidate();
	}

	void Draw(Canvas<Color>& canvas);

private:
	Color _forecolor;
	std::string  _text;
};