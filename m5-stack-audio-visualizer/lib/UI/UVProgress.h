
#pragma once

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