#pragma once

#include <list>

struct UIRect
{
	int x, y;
	int w, h;
};

class UIElement
{
public:
	UIElement(const UIRect& rect, const UIElement* parent = NULL) :
		_rect(rect), _parent(parent), Background(0, 0, 0)
	{}

	virtual void Draw(Canvas<Color>& canvas) const = 0;

	inline void SetParent(const UIElement* parent)
	{
		_parent = parent;
	}

	Color Background;

	virtual void Clear(Canvas<Color>& canvas) const
	{
		canvas.DrawFilledRect(_rect.x, _rect.y, _rect.w, _rect.h, Background);
	}

protected:
	const UIRect _rect;

	void AbsolutePosition(int& x, int& y) const
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

private:
	const UIElement* _parent;
};

class UIContainer : public UIElement
{
public:
	UIContainer(const UIRect& rect) :
		UIElement(rect)
	{}

	inline void Add(UIElement &element)
	{
		element.SetParent(this);

		_children.push_back(&element);
	}

	void Draw(Canvas<Color>& canvas) const
	{
		Clear(canvas); // TODO remove?

		for (const UIElement* element : _children)
		{
			element->Draw(canvas);
		}
	}

private:
	std::list<const UIElement*> _children;
};

class UILabel : public UIElement
{
public:
	UILabel(const UIRect& rect, const char* text, const char* fontName, int size = 24)
		: UIElement(rect), _text(text), _colorWhite(255,255,255)
	{ }

	inline void SetText(const char* text)
	{
		_text = text;
	}

	void Draw(Canvas<Color>& canvas) const
	{
		Clear(canvas);

		auto origin_x = _rect.x;
		auto origin_y = _rect.y;

		AbsolutePosition(origin_x, origin_y);
		 
		// auto center_x = origin_x + (_rect.w - surfaceMessage->w) / 2;
		// auto center_y = origin_y + (_rect.h - surfaceMessage->h) / 2;

		// canvas.DrawText(_text, center_x, center_y, 16, _colorWhite)
	}

	virtual ~UILabel()
	{
		// Don't forget to free your surface and texture
		//SDL_FreeSurface(surfaceMessage);
		//SDL_DestroyTexture(Message);
	}

private:
	const Color _colorWhite;
	const char* _text;
};

template <int TChannels>
class UISoundAnalyzer : public UIElement
{
public:
	UISoundAnalyzer(const UIRect& rect)
		: UIElement(rect), _channels{}
	{ }

	bool Update(int channel, unsigned char value)
	{
		if (channel > TChannels)
		{
			return false;
		}
		_channels[channel] = value;

		return true;
	}

	void Draw(Canvas<Color>& canvas) const
	{
		Clear(canvas);

		auto vertical_elements_count = 10 * 6;
		auto element_padding_x = 3;
		auto element_padding_y = 1;

		auto element_width = (_rect.w / TChannels) - element_padding_x;

		auto origin_x = _rect.x;
		auto origin_y = _rect.y;

		AbsolutePosition(origin_x, origin_y);

		auto y = origin_y;

		for (int i = 0; i < vertical_elements_count; i++)
		{
			auto x = origin_x;

			auto color = (i % 10 == 0 || i == vertical_elements_count - 1) ? _colorDarkYellow : _colorDarkGreen;

			for (int j = 0; j < TChannels; j++)
			{
				x += element_padding_x;

				canvas.DrawLine(x, y, x + element_width, y, color);

				x += element_width;
			}

			y += 1 + element_padding_y;
		}
	}

private:
	const Color _colorDarkYellow{ 99, 97, 48 };
	const Color _colorYellow{ 243, 232, 53 };

	const Color _colorGreen{ 100, 199, 73 };
	const Color _colorDarkGreen{ 44, 91, 45 };

	const Color _colorBlack{ 0, 0, 0 };

	const uint8_t _channels[TChannels];
};

template <typename TValue>
class UVProgress : public UIElement
{
public:
	UVProgress(const UIRect& rect, TValue min, TValue max, TValue threshold, TValue value = 0) :
		UIElement(rect), _minValue(min), _maxValue(max), _threshold(threshold), _value(value)
	{ }

	inline TValue Get()
	{
		return _value;
	}

	inline void Set(TValue value)
	{
		_value = value;
	}

	void Draw(Canvas<Color>& canvas) const
	{
		Clear(canvas);

		auto origin_x = _rect.x;
		auto origin_y = _rect.y;

		AbsolutePosition(origin_x, origin_y);

		auto origin_threshold = map(_threshold, _minValue, _maxValue, origin_x, origin_x + _rect.w);
		auto origin_max = map(_value, _minValue, _maxValue, origin_x, origin_x + _rect.w);

		for (int i = origin_x; i < origin_max; i += 3)
		{
			auto color = (i > origin_threshold) ? Color(255, 0, 0) : Color(15, 185, 79) ;

			canvas.DrawLine(i, origin_y, i, origin_y + _rect.h - 1, color);
		}
	}

	inline static int map(TValue x, TValue in_min, TValue in_max, int out_min, int out_max)
	{
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}

	//virtual ~UVProgress() = 0;

private:
	TValue _value;
	TValue _maxValue;
	TValue _minValue;

	TValue _threshold;
};