#pragma once

#include "UIElement.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

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

	inline void setTextF(const char* format, ...)
	{		
		va_list args;
    	va_start(args, format);
		char buffer[100];
		memset(&buffer[0], 0, sizeof(buffer));
		auto size = sprintf(&buffer[0], format, args);
		va_end(args);

		_text = &buffer[0];

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
	std::string _text;
};