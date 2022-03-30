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
		element.SetParent(this);

		_children.push_back(&element);
	}

	bool IsValid()
	{
		if (!UIElement::IsValid()) {
			return false;
		}

		for (UIElement* element : _children) {
			if(!element->IsValid()) {
				return false;
			}
		}

		return true;
	}

protected:
	void Draw(Canvas<Color>& canvas)
	{
		if (IsValid()) {
			return;
		}

		// Clear(canvas);

		for (UIElement* element : _children) {
			element->Update(canvas);
		}

		//_valid = true;
	}

private:
	std::list<UIElement*> _children;
};