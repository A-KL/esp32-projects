
#include <stdio.h>
#include "../BaseGraphics/Color.h"
#include "../BaseGraphics/Canvas.h"

#include "UIElement.h"

UIElement::UIElement(const UIRect rect, const Color background, const Color border, int borderSize, const UIElement* parent) :
    visible(true), _rect(rect), _backgroundColor(background), _borderColor(border), _borderSize(borderSize), _valid(false), _parent(parent)
{}

bool UIElement::IsValid() const
{
    return _valid || !visible;
}

void UIElement::Update(Canvas<Color>& canvas)
{
    if (IsValid()) {
        return;
    }

    Draw(canvas);

    _valid = true;
}

void UIElement::Clear(Canvas<Color>& canvas) const
{
    auto x = _rect.x;
    auto y = _rect.y;

    AbsolutePosition(x, y);
    
    canvas.DrawFilledRect(x, y, _rect.w, _rect.h, _backgroundColor);

    if (_borderSize > 0) {
        for (auto i = 0; i < _borderSize; i++) {
            canvas.DrawRect(x+i, y+i, _rect.w - (i*2) - 1, _rect.h - (i*2) - 1, _borderColor);
        }
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