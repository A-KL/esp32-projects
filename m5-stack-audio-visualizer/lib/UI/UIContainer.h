#pragma once

#include <list>

#include "UIElement.h"

class UIContainer : public UIElement
{
public:
	UIContainer(const UIRect& rect) :
		UIElement(rect)
	{ }

	void Add(UIElement &element)
	{
		element.SetParent(this);

		_children.push_back(&element);
	}

	bool IsValid()
	{
		if (!_valid) {
			return false;
		}

		for (UIElement* element : _children) {
			if(!element->IsValid()) {
				return false;
			}
		}

		return true;
	}

	void Draw(Canvas<Color>& canvas)
	{
		// if (IsValid()) {
		// 	return;
		// }

		// Clear(canvas);

		for (UIElement* element : _children) {
			element->Draw(canvas);
		}

		//_valid = true;
	}

private:
	std::list<UIElement*> _children;
};