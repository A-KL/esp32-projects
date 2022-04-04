#include <stdio.h>
#include <list>
#include <iterator>
#include <iostream>
#include <string>

#include "../BaseGraphics/Color.h"
#include "../BaseGraphics/Canvas.h"

#include "UIElement.h"
#include "UILabel.h"

UILabel::UILabel(const UIRect& rect, const char* text)
		: UIElement(rect), _forecolor(255, 255, 255, 0)
{
    setText(text);
}

UILabel::UILabel(const UIRect& rect, const char* text, const Color& borderColor, int borderSize)
		: UIElement(rect, {0, 0, 0, 0}, borderColor, borderSize), _forecolor(255, 255, 255, 0)
{
    setText(text);
}

void UILabel::Draw(Canvas<Color>& canvas)
{
    Clear(canvas);

    if (_text.c_str() == NULL) {
        return;
    }

    auto origin_x = _rect.x;
    auto origin_y = _rect.y;

    AbsolutePosition(origin_x, origin_y);

    canvas.DrawText(origin_x, origin_y, _rect.w, _rect.h, _text.c_str(), _forecolor);
}