#pragma once

struct UIRect
{
	int x, y;
	int w, h;
};

class UIElement
{
public:
	UIElement(const UIRect& rect, const Color& background = {0, 0, 0, 0}, const Color& border = {0, 0, 0, 0}, int borderSize = 0, const UIElement* parent = NULL);

	inline void SetParent(const UIElement* parent)
	{
		_parent = parent;
	}

	bool IsVisisble;

	Color Background;

	virtual void Draw(Canvas<Color>& canvas) = 0;

	virtual bool IsValid() const;

	virtual void Clear(Canvas<Color>& canvas) const;

protected:
	const UIRect _rect;
	bool _valid;

	inline virtual void Invalidate()
	{
		_valid = false;
	}

	void AbsolutePosition(int& x, int& y) const;

private:
	const UIElement* _parent;
	Color _borderColor;
	int _borderSize;
};