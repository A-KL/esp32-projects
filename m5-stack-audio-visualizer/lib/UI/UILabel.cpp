#include <stdio.h>
#include <list>
#include <iterator>
#include <iostream>

#include "../BaseGraphics/Color.h"
#include "../BaseGraphics/Canvas.h"

#include "UIElement.h"
#include "UILabel.h"

UILabel::UILabel(const UIRect& rect, const char* text, const char* fontName, int size)
		: UIElement(rect), _colorWhite(255,255,255), _text(text), _fontSize(size)
{ }

void UILabel::Draw(Canvas<Color>& canvas)
{
    if (IsValid()) {
        return;
    }

    _valid = true;

    Clear(canvas);

    if (_text == NULL) {
        return;
    }

    auto origin_x = _rect.x;
    auto origin_y = _rect.y;

    AbsolutePosition(origin_x, origin_y);

    std::string str(_text);
    auto textSize = str.length();
    auto textWidth = textSize * 6;

    auto center_x = origin_x + (_rect.w - textWidth) / 2;
    auto center_y = origin_y + (_rect.h - _fontSize) / 2;

    canvas.DrawText(center_x, center_y, _text, _fontSize, _colorWhite);
}

UILabel::~UILabel()
{
    // Don't forget to free your surface and texture
    //SDL_FreeSurface(surfaceMessage);
    //SDL_DestroyTexture(Message);
}