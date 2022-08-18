#pragma once

class UVProgress : public UIElement
{
	public:
		UVProgress(const UIRect& rect, unsigned short value = 0) :
			UIElement(rect, {10, 10, 10, 0}),
			_first(true),
			_newValue(value),
			_currentValue(0),
			_activeColor(15, 185, 50, 0),
			_activeThresholdColor(255, 0, 0, 0),
			_activeWarningColor(185, 186, 48, 0),
			_minValue(0), 
			_maxValue(rect.w /_item_w),
			_warning(_maxValue * 0.85),
			_threshold(_maxValue * 0.95)
		{}

		virtual unsigned short value() const
		{
			return _currentValue;
		}

		virtual bool IsValid() const
		{
			return _newValue == _currentValue && UIElement::IsValid();
		}

		virtual void setValue(unsigned short new_value)
		{
			_newValue = new_value > _maxValue ? _maxValue : new_value;

			Invalidate();
		}
		
	protected:
		void Draw(Canvas<Color>& canvas)
		{
			if (_first)
			{
				_first = false;
				Clear(canvas);
			}

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
		bool _first;

		const unsigned short _item_w = 3;
		const unsigned short _item_fill_w = _item_w - 1;

		unsigned short _newValue;
		unsigned short _currentValue;

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

		void setValue(unsigned short new_value)
		{
			_animatedValue = new_value > _maxValue ? _maxValue : new_value;

			Invalidate();
		}

		bool IsValid() const
		{
			return UVProgress::IsValid() && (value() == _animatedValue);
		}

	protected:
		void Draw(Canvas<Color>& canvas)
		{
			if (!IsValid())
			{
				if (value()<_animatedValue)
					UVProgress::setValue(UVProgress::value()+1);
				else
					UVProgress::setValue(UVProgress::value()-1);
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

		inline TValue valueOf() const
		{
			return map(value(), _minValue, _maxValue, _minValueT, _maxValueT);
		}

		inline void setValueOf(TValue new_value)
		{
			setValue(map(new_value, _minValueT, _maxValueT, _minValue, _maxValue));
		}

	private:
		const TValue _minValueT;
		const TValue _maxValueT;

		inline static TValue map(TValue x, TValue in_min, TValue in_max, int out_min, int out_max)
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

		inline TValue valueOf() const
		{
			return map(value(), _minValue, _maxValue,  _minValueT, _maxValueT);
		}

		inline void setValueOf(TValue new_value)
		{
			setValue(map(new_value, _minValueT, _maxValueT, _minValue, _maxValue));
		}

	protected:
		inline TValue maxValueT() const
		{
			return _maxValueT;
		}

		inline TValue minValueT() const
		{
			return _minValueT;
		}

	private:
		const TValue _minValueT;
		const TValue _maxValueT;

		inline static TValue map(TValue x, TValue in_min, TValue in_max, int out_min, int out_max)
		{
			return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
		}
};

template <typename TValue>
class UVFadingProgressOf : public UVProgressOf<TValue>
{
	public:
		UVFadingProgressOf(const UIRect& rect, TValue min, TValue max, TValue threshold, TValue value = 0) :
			UVProgressOf<TValue>(rect, min, max, threshold, value)
		{}

		bool IsValid() const
		{
			return UVProgressOf<TValue>::IsValid() && (UVProgressOf<TValue>::valueOf() <= UVProgressOf<TValue>::minValueT());
		}

	protected:
		void Draw(Canvas<Color>& canvas)
		{
			if (!IsValid())
			{
				if (UVProgressOf<TValue>::valueOf() > UVProgressOf<TValue>::minValueT())
				{
					auto value = UVProgressOf<TValue>::valueOf() - 10;

					UVProgressOf<TValue>::setValueOf(value < UVProgressOf<TValue>::minValueT() ? UVProgressOf<TValue>::minValueT() : value);
				}
			}

			UVProgressOf<TValue>::Draw(canvas);
		}	
};