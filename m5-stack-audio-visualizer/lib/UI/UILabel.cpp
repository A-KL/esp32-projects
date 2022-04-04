#include <stdio.h>
#include <list>
#include <iterator>
#include <iostream>
#include <string>

#include "../BaseGraphics/Color.h"
#include "../BaseGraphics/Canvas.h"

#include "UIElement.h"
#include "UILabel.h"

UILabel::UILabel(const UIRect& rect, const char* text, const char* fontName, int size)
		: UIElement(rect), _forecolor(255,255,255), _fontSize(size)
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