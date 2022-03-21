#pragma once

#include "UIElement.h"
 
class UILabel : public UIElement
{
public:
	UILabel(const UIRect& rect, const char* text, const char* fontName, int size);

	inline void SetText(const char* text)
	{
		_text = text;
		_valid = false;
	}

	void Draw(Canvas<Color>& canvas);

	virtual ~UILabel();

private:
	const Color _colorWhite;
	const char* _text;
	const int _fontSize;
};