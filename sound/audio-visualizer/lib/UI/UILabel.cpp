#include <stdio.h>
#include <list>
#include <iterator>
#include <iostream>
#include <string>

#include "../BaseGraphics/Color.h"
#include "../BaseGraphics/Canvas.h"

#include "UIElement.h"
#include "UILabel.h"

UILabel::UILabel(const UIRect& rect, const char* text, int fontIndex)
		: UILabel(rect, text, Color::White, Color::White, 0)
{ 
    _fontIndex = fontIndex;
    //setBackgroundColor(Color::Red);
}

UILabel::UILabel(const UIRect& rect, const char* text, const Color& foreColor, const Color& borderColor, int borderSize)
		: UIElement(rect, {0, 0, 0, 0}, borderColor, borderSize), _forecolor(foreColor), _fontIndex(0)
{
    setText(text);
}

void UILabel::Draw(Canvas<Color>& canvas)
{
    Clear(canvas, false);

    if (_text.c_str())
    {
        auto origin_x = _rect.x;
        auto origin_y = _rect.y;

        AbsolutePosition(origin_x, origin_y);

        canvas.SetFont(_fontIndex, 1);
        canvas.DrawText(origin_x, origin_y, _rect.w, _rect.h, _text.c_str(), _forecolor);
    }
    
    UIElement::Draw(canvas);
}