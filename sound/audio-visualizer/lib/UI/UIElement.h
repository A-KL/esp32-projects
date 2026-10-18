#pragma once

#include <iostream>
#include <typeinfo>

class UIElement
{
public:
	UIElement(
		const UIRect rect = {0, 0, 50, 50}, 
		const Color background = Color::Black, 
		const Color borderColor = Color::LightGray, 
		const int borderSize = 0,
		UIElement* parent = nullptr);

	inline void SetParent(UIElement* parent)
	{
		_parent = parent;
		Invalidate();
	}

	inline void SetBorderColor(const Color& color)
	{
		if (color == _borderColor) {
			return;
		}
		_borderColor = color;
		Invalidate(true);
	}

	inline void SetBackgroundColor(const Color& color)
	{
		if (color == _backgroundColor) {
			return;
		}
		_backgroundColor = color;
		Invalidate(true);
	}

	inline void SetBorderSize(int size)
	{
		if (size == _borderSize) {
			return;
		}
		_borderSize = size;
		Invalidate(true);
	}

	inline bool IsVisible() const {
		return _visible;
	}

	inline virtual bool IsValid() const {
		return !_visible || (_updated && !_clean);
	}

	inline virtual void Invalidate(bool clean = false) {
		_updated = false;
		_clean |= clean;
	}

	// inline virtual const char* ToString() const {
	// 	return typeid(this).name();
	// }

	void SetVisible(bool visible);
	void Update(Canvas<Color>& canvas);

protected:
	virtual void Draw(Canvas<Color>& canvas);
	
	void Clear(Canvas<Color>& canvas) const;

	void AbsolutePosition(int& x, int& y) const;
	bool Contains(const UIElement& other) const;

	inline bool IsClean() const {
		return _clean;
	}

	const UIRect _rect;
	Color _backgroundColor;
	Color _borderColor;
	int _borderSize;
	
private:
	volatile bool _updated;
	volatile bool _clean;
	volatile bool _visible;
	UIElement* _parent;
};