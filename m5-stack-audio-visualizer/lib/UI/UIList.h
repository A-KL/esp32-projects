#pragma once

#include <list>
#include <iterator>
#include <iostream>

#include "UIElement.h"

template <typename TItem>
class UIList : public UIContainer
{
public: 
	UIList(const UIRect& rect, int fontSize = 16) : 
		UIContainer(rect), 
		_selected(-1), 
		_selectedNew(-1), 
		_focused(0), 
		_focusedNew(0), 
		_visibleItems(rect.h / fontSize),
		_textColor(255, 255, 255)
	{}

	inline bool IsValid() const
	{
		return _selected == _selectedNew && _focused == _focusedNew;
	}

	inline void SetFocused(unsigned short focused)
	{
		_focusedNew = focused;
	}

	inline void SetSelected(unsigned short selected)
	{
		_selectedNew = selected;
	}

	inline TItem Selected() const
	{
		auto it = _items.begin();

		std::advance(it, _selected);

		return *it;
	}

	inline void Add(const TItem& item)
	{
		_items.push_back(item);
	}

	void Draw(Canvas<Color>& canvas)
	{		
		if (IsValid()) {
			return;
		}

		auto origin_x = _rect.x;
		auto origin_y = _rect.y;

		AbsolutePosition(origin_x, origin_y);



		_focused = _focusedNew;
		_selected = _selectedNew;
	}
	
    typedef void (*ItemSelectionChangedCallback)(const UIElement& object, const TItem& item);
    void OnItemSelectionChanged(ItemSelectionChangedCallback event);

protected:
	void RaiseOnItemSelectionChanged()
	{
		OnItemSelectionChanged(this, &Selected());
	}

private:
	short _selected;
	short _selectedNew;

	short _focused;
	short _focusedNew;

	unsigned short _visibleItems;

	const Color _textColor;

	std::list<TItem> _items;
};