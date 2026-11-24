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

		Invalidate(true);
	}

	void Remove(UIElement &element)
	{
		element.SetParent(NULL);
		_children.remove(&element);

		Invalidate(true);
	}

	virtual void Invalidate(bool clean = false) 
	{
		UIElement::Invalidate(clean);

		for (auto element : _children) {
			element->Invalidate(clean);
		}
	}

	virtual bool IsValid() const
	{
		if (!UIElement::IsValid()){
			return false;
		}

		if (!IsVisible()) {
			return true;
		}

		// for (const auto element : _children) {
		// 	if (!element->IsValid()) {
		// 		return false;
		// 	}
		// }
		return false;
	}

protected:
	void Draw(Canvas<Color>& canvas)
	{
		// auto offset = 0;
		for (auto element : _children) {
			// if ((element->_rect.x == -1) && (_rect.w > element->_rect.w + element->_rect.x)) {
			// 	element->_rect.x = offset;
			// 	offset+= element->_rect.w;
			// }
			element->Update(canvas);
		}
	}

private:
	std::list<UIElement*> _children;
};