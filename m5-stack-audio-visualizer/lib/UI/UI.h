#pragma once

#include <list>
#include <iterator>
#include <iostream>
#include <algorithm>

struct UIRect
{
	int x, y;
	int w, h;
};

class UIElement
{
public:
	UIElement(const UIRect& rect, const UIElement* parent = NULL) :
		Background(0, 0, 0), _rect(rect), _valid(false), _parent(parent), _visible(true)
	{}

	inline void SetParent(const UIElement* parent)
	{
		_parent = parent;
	}

	inline bool IsVisisble()
	{
		return _visible;
	}

	inline void SetVisisble(bool visible)
	{
		_visible = visible;
	}

	Color Background;

	virtual void Draw(Canvas<Color>& canvas) = 0;

	virtual bool IsValid() const
	{
		return _valid;
	}

	virtual void Clear(Canvas<Color>& canvas) const
	{
		canvas.DrawFilledRect(_rect.x, _rect.y, _rect.w, _rect.h, Background);
	}

protected:
	const UIRect _rect;
	bool _valid;

	inline virtual void Invalidate()
	{
		_valid = false;
	}

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
	bool _visible;
};

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
 
class UILabel : public UIElement
{
public:
	UILabel(const UIRect& rect, const char* text, const char* fontName, int size)
		: UIElement(rect), _colorWhite(255,255,255), _text(text), _fontSize(size)
	{ }

	inline void SetText(const char* text)
	{
		_text = text;
	}

	void Draw(Canvas<Color>& canvas)
	{
		if (IsValid()) {
			return;
		}

		Clear(canvas);

		auto origin_x = _rect.x;
		auto origin_y = _rect.y;

		AbsolutePosition(origin_x, origin_y);

		std::string str(_text);
		auto textSize = str.length();
		auto textWidth = textSize * 6;

		auto center_x = origin_x + (_rect.w - textWidth) / 2;
		auto center_y = origin_y + (_rect.h - _fontSize) / 2;

		canvas.DrawText(center_x, center_y, _text, _fontSize, _colorWhite);

		_valid = true;
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
	const int _fontSize;
};

// template <typename TItem>
// class UIList : public UIContainer
// {
// public: 
// 	UIList(const UIRect& rect, const std::list<TItem>& list, int fontSize) : 
// 		UIContainer(rect), 
// 		_selected(-1), 
// 		_selectedNew(-1), 
// 		_focused(0), 
// 		_focusedNew(0), 
// 		_visibleItems(rect.h / fontSize),
// 		_textColor(255, 255, 255),
// 		_items(&list)
// 	{}

// 	inline bool IsValid() const
// 	{
// 		return _selected == _selectedNew && _focused == _focusedNew;
// 	}

// 	inline void SetFocused(unsigned short focused)
// 	{
// 		_focusedNew = focused;
// 	}

// 	inline void SetSelected(unsigned short selected)
// 	{
// 		_selectedNew = selected;
// 	}

// 	inline TItem Selected() const
// 	{
// 		auto it = _items->begin();

// 		std::advance(it, _selected);

// 		return *it;
// 	}

// 	void Draw(Canvas<Color>& canvas)
// 	{		
// 		if (IsValid()) {
// 			return;
// 		}

// 		auto origin_x = _rect.x;
// 		auto origin_y = _rect.y;

// 		AbsolutePosition(origin_x, origin_y);



// 		_focused = _focusedNew;
// 		_selected = _selectedNew;
// 	}
	
//     typedef void (*ItemSelectionChangedCallback)(const UIElement& object, const TItem& item);
//     void OnItemSelectionChanged(ItemSelectionChangedCallback event);

// protected:
// 	void RaiseOnItemSelectionChanged()
// 	{
// 		OnItemSelectionChanged(this, &Selected());
// 	}

// private:
// 	unsigned short _selected;
// 	unsigned short _selectedNew;

// 	unsigned short _focused;
// 	unsigned short _focusedNew;

// 	unsigned short _visibleItems;

// 	const Color _textColor;

// 	std::list<TItem>* _items;
// };

template <size_t TChannels>
class UISoundAnalyzer : public UIElement
{
public:
	UISoundAnalyzer(const UIRect& rect)
		: UIElement(rect)
	{ 
		for (int j = 0; j < TChannels; j++)
		{
			_channels[j] = 0;
		}
	}

	bool Update(int channel, unsigned char value)
	{
		if (channel >= TChannels)
		{
			return false;
		}
		_channels[channel] = value;

		Invalidate();

		return true;
	}

	void Draw(Canvas<Color>& canvas)
	{
		if (IsValid()) {
			return;
		}

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

			auto color = (i % 10 == 0 || i == (vertical_elements_count - 1)) ? _colorDarkYellow : _colorDarkGreen;
			auto active_color = (i % 10 == 0 || i == (vertical_elements_count - 1)) ? _colorYellow : _colorGreen;
			auto main_color = color;

			for (int j = 0; j < TChannels; j++)
			{
				if ((vertical_elements_count - _channels[j]) < i)
				{
					main_color = active_color;
				}
				else
				{
					main_color = color;
				}

				x += element_padding_x;

				canvas.DrawLine(x, y, x + element_width, y, main_color);

				x += element_width;
			}

			y += 1 + element_padding_y;
		}

		_valid = true;
	}

private:
	const Color _colorDarkYellow{35, 35, 15 }; //99, 97, 48 
	const Color _colorYellow{ 243, 232, 53 };

	const Color _colorGreen{ 100, 199, 73 };
	const Color _colorDarkGreen{ 9, 14, 7 }; //44, 91, 45

	const Color _colorBlack{ 0, 0, 0 };

	uint8_t _channels[TChannels];
};

class UVProgress : public UIElement
{
public:
	UVProgress(const UIRect& rect, unsigned short value = 0) :
		UIElement(rect),
		_newValue(value),
		_currentValue(value),
		_backgroundColor(30, 30, 30),
		_activeColor(15, 185, 79),
		_activeThresholdColor(255, 0, 0),
		_minValue(0), 
		_maxValue(rect.w /_item_w), 
		_threshold(_maxValue * 0.9)
	{ 
		 Background = Color(30, 30, 30);
	}

	inline unsigned short Value() const
	{
		return _currentValue;
	}

	inline bool IsValid() const
	{
		return _newValue == _currentValue;
	}

	inline void SetValue(unsigned short new_value)
	{
		_newValue = new_value > _maxValue ? _maxValue : new_value;
	}

	void Draw(Canvas<Color>& canvas)
	{
		if (IsValid()) {
			return;
		}

		auto origin_x = _rect.x;
		auto origin_y = _rect.y;

		AbsolutePosition(origin_x, origin_y);

		if (_currentValue < _newValue)
		{
			for (auto index = _currentValue; index < _newValue; index++)
			{
				auto color = index >= _threshold ? _activeThresholdColor : _activeColor;
				canvas.DrawFilledRect(origin_x + index * _item_w, origin_y, _item_fill_w, _rect.h, color);
			}
		}
		else
		{
			canvas.DrawFilledRect(origin_x + _newValue * _item_w, origin_y, (_currentValue - _newValue)*_item_w, _rect.h, _backgroundColor);
		}

		_currentValue = _newValue;
	}

	//virtual ~UVProgress() = 0;

private:
	const unsigned short _item_w = 3;
	const unsigned short _item_fill_w = _item_w - 1;

	unsigned short _newValue;
	unsigned short _currentValue;

	const Color _backgroundColor;
	const Color _activeColor;
	const Color _activeThresholdColor;

protected:
	const unsigned short _minValue;
	const unsigned short _maxValue;
	const unsigned short _threshold;
};

template <typename TValue>
class UVProgressTyped : public UVProgress
{
	public:
		UVProgressTyped(const UIRect& rect, TValue min, TValue max, TValue threshold, TValue value = 0) : 
			UVProgress(rect, value), _minValueT(min), _maxValueT(max)
	{}

	inline TValue ValueT() const
	{
		return map(Value(), _minValue, _maxValue,  _minValueT, _maxValueT);
	}

	inline void SetValueT(TValue new_value)
	{
		SetValue(map(new_value, _minValueT, _maxValueT, _minValue, _maxValue));
	}

private:
	const TValue _minValueT;
	const TValue _maxValueT;

	inline static int map(TValue x, TValue in_min, TValue in_max, int out_min, int out_max)
	{
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}
};

// template <typename TValue>
// class UVAnimatedProgress : public UVProgress<TValue>
// {
// public:
// 	UVAnimatedProgress(const UIRect& rect, TValue min, TValue max, TValue threshold, TValue value = 0) :
// 		UVProgress<TValue>(rect, min, max, threshold, value)
// 	{ }

// 	virtual void Draw(Canvas<Color>& canvas)
// 	{
// 		if (IsValid()) {
// 			return;
// 		}

// 		auto origin_x = _rect.x;
// 		auto origin_y = _rect.y;

// 		AbsolutePosition(origin_x, origin_y);

// 		auto color = (_deltaValue > 0) ? _activeColor : _backgroundColor;

// 		auto item_w = 3;

// 		auto current_w = map(_oldValue, _minValue, _maxValue, 0, _rect.w);
// 		auto new_w = map(_newValue, _minValue, _maxValue, 0, _rect.w);

// 		auto items = abs(new_w - current_w) / item_w;
// 		auto count = (current_w / item_w);

// 		if (items > 0)
// 		{
// 			if (_deltaValue > 0)
// 			{
// 				canvas.DrawFilledRect(origin_x + count * item_w, origin_y, item_w - 1, _rect.h, color);
// 			}
// 			else if (_deltaValue < 0)
// 			{
// 				canvas.DrawFilledRect(origin_x + (count - 1) * item_w, origin_y, item_w, _rect.h, color);
// 			}
// 			else
// 			{
// 				return;
// 			}
// 		}

// 		_oldValue += _deltaValue;
// 	}
// };