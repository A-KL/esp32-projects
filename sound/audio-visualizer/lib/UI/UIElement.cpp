
#include <stdio.h>
#include <Color.h>
#include <Canvas.h>

#include "UIElement.h"

UIElement::UIElement(const UIRect rect, const Color background, const Color border, int borderSize, const UIElement* parent) :
    _rect(rect), 
    _backgroundColor(background), 
    _borderColor(border), 
    _borderSize(borderSize), 
    _valid(false), 
    _visible(true), 
    _parent(parent)
{}

bool UIElement::IsValid() const
{
    return _valid || !_visible;
}

void UIElement::SetVisible(bool visible)
{
    if (_visible == visible) {
        return;
    }
    _visible = visible;
    Invalidate();
}

// bool UIElement::Update(Canvas<Color>& canvas)
// {
//     if (IsValid()) {
//         return false;
//     }

//     Draw(canvas);

//     _valid = true;

//     return true;
// }

void UIElement::Update(Canvas<Color>& canvas)
{
    if (IsValid()) {
        return;
    }

    Draw(canvas);

    _valid = true;
}

void UIElement::Clear(Canvas<Color>& canvas, bool draw)
{
    auto x = _rect.x;
    auto y = _rect.y;

    AbsolutePosition(x, y);
    
    canvas.DrawFilledRect(x, y, _rect.w, _rect.h, _backgroundColor);
    
    if (draw) {
        UIElement::Draw(canvas);
    }
}

void UIElement::Draw(Canvas<Color>& canvas)
{
    auto x = _rect.x;
    auto y = _rect.y;

    AbsolutePosition(x, y);
    
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