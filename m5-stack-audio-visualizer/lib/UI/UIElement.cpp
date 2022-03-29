
#include <stdio.h>
#include "../BaseGraphics/Color.h"
#include "../BaseGraphics/Canvas.h"

#include "UIElement.h"

UIElement::UIElement(const UIRect& rect, const Color& background, const Color& border, int borderSize, const UIElement* parent) :
    IsVisisble(true), Background(background), _rect(rect), _valid(false), _parent(parent), _borderColor(border), _borderSize(borderSize)
{}

bool UIElement::IsValid() const
{
    return _valid;
}

void UIElement::Clear(Canvas<Color>& canvas) const
{
    auto x = _rect.x;
    auto y = _rect.y;

    AbsolutePosition(x, y);
    
    canvas.DrawFilledRect(x, y, _rect.w, _rect.h, Background);

    if (_borderSize > 0) {
        canvas.DrawFilledRect(x, y, _rect.w, _rect.h, _borderColor);
    }
}

void UIElement::AbsolutePosition(int& x, int& y) const
{
    if (_parent == NULL)
    {
        x = _rect.x;
        y = _rect.y;
    }
    else
    {
        _parent->AbsolutePosition(x, y);

        x += _rect.x;
        y += _rect.y;
    }
}