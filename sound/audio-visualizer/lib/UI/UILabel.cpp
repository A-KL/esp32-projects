#include <stdio.h>
#include <list>
#include <iterator>
#include <iostream>
#include <string>
#include <Color.h>
#include <Canvas.h>

#include "UIRect.h"
#include "UIElement.h"
#include "UILabel.h"

UILabel::UILabel(
    const UIRect& rect, 
    const char* text,
     const Color& foreColor, 
     const Color& borderColor, 
     const Color& backgroundColor, 
     int borderSize, int fontIndex, int fontSize)
: UIElement(rect, backgroundColor, borderColor, borderSize), _fcolor(foreColor), _fontIndex(fontIndex), _fontSize(fontSize)
{
    SetText(text);
}

UILabel::UILabel(
    const UIRect& rect, 
    const char* text, 
    const Color& foreColor,
    const Color& borderColor,
    int borderSize,
    int fontIndex,
    int fontSize)
: UILabel(rect, text, foreColor, borderColor, Color::Black, borderSize, fontIndex, fontSize)
{ }

UILabel::UILabel(
    const UIRect& rect, 
    const char* text, 
    int fontIndex, 
    int fontSize)
    : UILabel(rect, text, Color::White, Color::Black, 0, fontIndex, fontSize)
{ }

void UILabel::Draw(Canvas<Color>& canvas)
{
    if (_text.c_str())
    {
        auto origin_x = _rect.x;
        auto origin_y = _rect.y;

        AbsolutePosition(origin_x, origin_y);

        canvas.SetFont(_fontIndex, _fontSize);
        canvas.DrawText(origin_x, origin_y, _rect.w, _rect.h, _text.c_str(), _fcolor, UIElement::_backgroundColor);
    }
}