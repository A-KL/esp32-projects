#pragma once

class UIElement
{
public:
	UIElement(
		const UIRect rect, 
		const Color background = {0, 0, 0, 0}, 
		const Color border = {0, 0, 0, 0}, 
		int borderSize = 0, 
		UIElement* parent = nullptr);

	inline void setParent(UIElement* parent)
	{
		_parent = parent;
		Invalidate();
	}

	inline void setBorderColor(const Color color)
	{
		if (color == _borderColor) {
			return;
		}
		_borderColor = color;
		Invalidate();
	}

	inline void setBackgroundColor(const Color color)
	{
		if (color == _backgroundColor) {
			return;
		}
		_backgroundColor = color;
		Invalidate();
	}

	inline void setBorderSize(int size)
	{
		if (size == _borderSize) {
			return;
		}
		_borderSize = size;
		Invalidate();
	}

	inline virtual bool IsValid() const
	{
			return _valid || !_visible;
	}

	inline virtual void Invalidate() 
	{
		_valid = false;
	}

	virtual void Clear(Canvas<Color>& canvas, bool draw = true);

	void Update(Canvas<Color>& canvas);
	bool Contains(const UIElement& other) const;

protected:
	virtual void Draw(Canvas<Color>& canvas);

	void SetVisible(bool visible);
	void AbsolutePosition(int& x, int& y) const;

	const UIRect _rect;
	Color _backgroundColor;
	Color _borderColor;
	int _borderSize;
	
private:
	volatile bool _valid;
	volatile bool _visible;
	UIElement* _parent;
};