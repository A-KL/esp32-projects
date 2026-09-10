#pragma once

#include <list>

#include "UIElement.h"

class UIContainer : public UIElement
{
public:
	UIContainer(const UIRect& rect) :
		UIElement(rect)
	{ }

	UIContainer(const UIRect& rect, const Color& color) :
		UIElement(rect, color)
	{ }

	void Add(UIElement &element)
	{
		element.setParent(this);

		_children.push_back(&element);

		Invalidate();
	}

	void Remove(UIElement &element)
	{
		element.setParent(NULL);

		_children.remove(&element);

		Invalidate();
	}

bool IsValid() const override
	 {
	 	if (_redraw) {
	 		return false;
	 	}

	 	for (const UIElement* element : _children) {
	 		if (!element->IsValid()) {
	 			return false;
	 		}
	 	}

	 	return true;
	 }

	 void Invalidate() override
	 {
	 	UIElement::Invalidate();
	 	_redraw = true;
	 }

protected:
	void Draw(Canvas<Color>& canvas)
	{		
		if (_redraw) {
			Clear(canvas);
			_redraw = false;
		}

		for (UIElement* element : _children) {
			element->Update(canvas);
		}
	}

private:
	std::list<UIElement*> _children;
	bool _redraw = true;
};