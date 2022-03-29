
#include <stdio.h>
#include "../BaseGraphics/Color.h"
#include "../BaseGraphics/Canvas.h"

#include "UIElement.h"

UIElement::UIElement(const UIRect& rect, const Color& background, const UIElement* parent) :
    IsVisisble(true), Background(background), _rect(rect), _valid(false), _parent(parent) 
{}

bool UIElement::IsValid() const
{
    return _valid;
}

void UIElement::Clear(Canvas<Color>& canvas) const
{
    canvas.DrawFilledRect(_rect.x, _rect.y, _rect.w, _rect.h, Background);
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