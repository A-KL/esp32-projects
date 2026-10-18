#include <stdio.h>

#include <Color.h>
#include <Canvas.h>

#include "UIRect.h"
#include "UIElement.h"

UIElement::UIElement(const UIRect rect, const Color background, const Color borderColor, int borderSize, UIElement* parent) :
    _rect(rect),
    _backgroundColor(background),
    _borderColor(borderColor),
    _borderSize(borderSize),
    _updated(false),
    _clean(true),
    _visible(true),
    _parent(parent)
{ }

void UIElement::SetVisible(bool visible)
{
    if (visible == _visible) {
        return;
    }
    _visible = visible;

    if (_visible) {
        Invalidate(true);
    } 
    else if (_parent != nullptr) {
        _parent->Invalidate(true);
    }
}

void UIElement::Update(Canvas<Color>& canvas)
{
    if (IsValid()) {
       return;
    }
    if (_clean) {
       Clear(canvas);              // Background color fill
    }
    Draw(canvas);                   // Update dynamic content
    if (_clean) {
       UIElement::Draw(canvas);    // Borders
    }
    _updated = true;
    _clean = false;
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

void UIElement::Clear(Canvas<Color>& canvas) const
{
    auto x = _rect.x;
    auto y = _rect.y;

    AbsolutePosition(x, y);
    
    canvas.DrawFilledRect(x, y, _rect.w, _rect.h, _backgroundColor);
}

bool UIElement::Contains(const UIElement& other) const
{
    UIRect result;
    return _rect.Diff(other._rect, result);
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