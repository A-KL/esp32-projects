#pragma once

#include "UIElement.h"
#include <sstream>
#include <stdio.h>
#include <iostream>
#include <stdarg.h>
#include <string.h>

class UILabel : public UIElement
{
public:
	UILabel(const UIRect& rect, const char* text, int fontIndex = 0);

	UILabel(const UIRect& rect, const char* text, const Color& foreColor, const Color& borderColor, int borderSize);

	inline void setText(const char* text)
	{		
		if (text == _text) {
			return;
		}
		_text = text;
		Invalidate();
	}

	inline void setInt(int value)
	{		
		std::stringstream ss;  
		ss << value;  
		ss >> _text;

		Invalidate();
	}

	inline void setTextF(const char* format, ...)
	{		
		va_list args;
		va_start(args, format);
		char buffer[100];
		memset(buffer, 0, sizeof(buffer));
		snprintf(buffer, sizeof(buffer), format, args);
		va_end(args);
		_text = buffer;
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

	friend std::ostream& operator<<(std::ostream& out, const UILabel& obj)
	{
		out << obj._text;
		return out;
	}

protected:
	void Draw(Canvas<Color>& canvas);

private:
	Color _forecolor;
	std::string _text;
	int _fontIndex = 0;
};