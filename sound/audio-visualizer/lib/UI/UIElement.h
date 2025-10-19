#pragma once

struct UIRect
{
	int x, y;
	int w, h;
};

class UIElement
{
public:
	UIElement(
		const UIRect& rect, 
		const Color& background = {0, 0, 0, 0}, 
		const Color& border = {0, 0, 0, 0}, 
		int borderSize = 0, 
		const UIElement* parent = nullptr);

	inline void setParent(const UIElement* parent)
	{
		_parent = parent;
	}

	inline void setBorderColor(const Color& color)
	{
		_borderColor = color;
		Invalidate();
	}

	inline void setBackgroundColor(const Color& color)
	{
		_backgroundColor = color;
		Invalidate();
	}

	inline void setBorderSize(int size)
	{
		_borderSize = size;
		Invalidate();
	}

	bool visible;

	virtual bool IsValid() const;

	void Update(Canvas<Color>& canvas);	

	virtual void Clear(Canvas<Color>& canvas) const;

protected:
	const UIRect _rect;
	
	inline virtual void Invalidate()
	{
		_valid = false;
	}

	virtual void Draw(Canvas<Color>& canvas) = 0;

	void AbsolutePosition(int& x, int& y) const;

	Color _backgroundColor;
	Color _borderColor;
	int _borderSize;

private:
	bool _valid;
	const UIElement* _parent;
};