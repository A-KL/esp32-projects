#pragma once

#include <sstream>
#include <stdio.h>
#include <iostream>
#include <stdarg.h>
#include <string.h>

#include "UIElement.h"

class UILabel : public UIElement
{
public:
	UILabel(const UIRect& rect, const char* text, int fontIndex = 0);

	UILabel(const UIRect& rect, const char* text, const Color& foreColor, const Color& borderColor, int borderSize = 0);

	inline void SetText(const char* text)
	{		
		if (text == _text) {
			return;
		}
		_text = text;
		Invalidate(true);
	}

	inline void SetInt(int value)
	{		
		std::stringstream ss;  
		ss << value;  
		ss >> _text;

		Invalidate(true);
	}

	inline void SetTextF(const char* format, ...)
	{		
		va_list args;
		va_start(args, format);
		char buffer[50];
		const auto size = sizeof(buffer);
		memset(buffer, 0, size);
		vsnprintf(buffer, size, format, args);
		va_end(args);
		_text = buffer;
		Invalidate(true);
	}

	inline void SetForeColor(const Color& color)
	{
		if (color == _fcolor) {
			return;
		}
		_fcolor = color;
		Invalidate(false);
	}

	friend std::ostream& operator<<(std::ostream& out, const UILabel& obj)
	{
		out << obj._text;
		return out;
	}

protected:
	void Draw(Canvas<Color>& canvas);

private:
	Color _fcolor;
	std::string _text;
	int _fontIndex = 0;
};