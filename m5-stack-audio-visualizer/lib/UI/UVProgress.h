
#pragma once

class UVProgress : public UIElement
{
public:
	UVProgress(const UIRect& rect, unsigned short value = 0) :
		UIElement(rect, {30, 30, 30}),
		_newValue(value),
		_currentValue(value),
		_backgroundColor(30, 30, 30),
		_activeColor(15, 185, 79),
		_activeThresholdColor(255, 0, 0),
		_activeWarningColor(185, 186, 48),
		_minValue(0), 
		_maxValue(rect.w /_item_w),
		_warning(_maxValue * 0.85),
		_threshold(_maxValue * 0.95)
	{ }

	virtual unsigned short Value() const
	{
		return _currentValue;
	}

	virtual bool IsValid() const
	{
		return _newValue == _currentValue || UIElement::IsValid();
	}

	virtual void SetValue(unsigned short new_value)
	{
		_newValue = new_value > _maxValue ? _maxValue : new_value;

		Invalidate();
	}
	
protected:
	void Draw(Canvas<Color>& canvas)
	{
		auto origin_x = _rect.x;
		auto origin_y = _rect.y;

		AbsolutePosition(origin_x, origin_y);

		if (_currentValue < _newValue)
		{
			for (auto index = _currentValue; index < _newValue; index++)
			{
				auto color = index >= _threshold ? _activeThresholdColor : index >= _warning ? _activeWarningColor : _activeColor;
				canvas.DrawFilledRect(origin_x + index * _item_w, origin_y, _item_fill_w, _rect.h, color);
			}
		}
		else
		{
			canvas.DrawFilledRect(origin_x + _newValue * _item_w, origin_y, (_currentValue - _newValue)*_item_w, _rect.h, _backgroundColor);
		}

		_currentValue = _newValue;
	}

private:
	const unsigned short _item_w = 3;
	const unsigned short _item_fill_w = _item_w - 1;

	unsigned short _newValue;
	unsigned short _currentValue;

	const Color _backgroundColor;
	const Color _activeColor;
	const Color _activeThresholdColor;
	const Color _activeWarningColor;

protected:
	const unsigned short _minValue;
	const unsigned short _maxValue;
	const unsigned short _warning;
	const unsigned short _threshold;
};

class UVAnimatedProgress : public UVProgress
{
public:
	UVAnimatedProgress(const UIRect& rect, const unsigned short value = 0) : 
		UVProgress(rect, value), _animatedValue(0)
	{}

	inline void SetAnimatedValue(unsigned short new_value)
	{
		_animatedValue = new_value > _maxValue ? _maxValue : new_value;
	}

	void SetValue(unsigned short new_value)
	{
		_animatedValue = new_value > _maxValue ? _maxValue : new_value;
		UVProgress::SetValue(UVProgress::Value());
	}

	bool IsValid() const
	{
		return UVProgress::IsValid() && (Value() == _animatedValue);
	}

	void Draw(Canvas<Color>& canvas)
	{
		if (!IsValid())
		{
			if (Value()<_animatedValue)
				UVProgress::SetValue(UVProgress::Value()+1);
			else
				UVProgress::SetValue(UVProgress::Value()-1);
		}

		UVProgress::Draw(canvas);
	}
private:
	unsigned short _animatedValue;
};

template <typename TValue>
class UVAnimatedProgressOf : public UVAnimatedProgress
{
public:
	UVAnimatedProgressOf(const UIRect& rect, TValue min, TValue max, TValue threshold, TValue value = 0) :
		UVAnimatedProgress(rect, value), _minValueT(min), _maxValueT(max)
	{}

	inline TValue ValueOf() const
	{
		return map(Value(), _minValue, _maxValue, _minValueT, _maxValueT);
	}

	inline void SetValueOf(TValue new_value)
	{
		SetValue(map(new_value, _minValueT, _maxValueT, _minValue, _maxValue));
	}

	inline void SetAnimatedValueOf(TValue new_value)
	{
		SetAnimatedValue(map(new_value, _minValueT, _maxValueT, _minValue, _maxValue));
	}

private:
	const TValue _minValueT;
	const TValue _maxValueT;

	inline static int map(TValue x, TValue in_min, TValue in_max, int out_min, int out_max)
	{
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}
};

template <typename TValue>
class UVProgressOf : public UVProgress
{
	public:
		UVProgressOf(const UIRect& rect, TValue min, TValue max, TValue threshold, TValue value = 0) :
			UVProgress(rect, value), _minValueT(min), _maxValueT(max)
	{}

	inline TValue ValueOf() const
	{
		return map(Value(), _minValue, _maxValue,  _minValueT, _maxValueT);
	}

	inline void SetValueOf(TValue new_value)
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