#pragma once

#include "UIElement.h"

template <size_t TChannels>
class UISoundAnalyzer : public UIElement
{
public:
	UISoundAnalyzer(const UIRect& rect) : UIElement(rect)
	{ 
		for (int j = 0; j < count(); j++)
		{
			_channels[j] = 0;
		}
	}

	inline int count() const
	{
		return TChannels;
	}

	inline int maxBand() const
	{
		return _vertical_elements_count;
	}

	bool setBand(int channel, unsigned char value)
	{
		if (channel >= TChannels)
		{
			return false;
		}

		auto new_value = value > _vertical_elements_count ? _vertical_elements_count : value;

		if (new_value == _channels[channel])
		{
			return false;
		}
		
		_channels[channel] = new_value;

		Invalidate();

		return true;
	}

protected:
	void Draw(Canvas<Color>& canvas)
	{
		auto origin_x = _rect.x;
		auto origin_y = _rect.y;

		//AbsolutePosition(origin_x, origin_y);

		auto y = origin_y;

		for (auto i = 0; i < _vertical_elements_count; i++)
		{
			auto x = origin_x;

			auto color = (i % 10 == 0 || i == (_vertical_elements_count - 1)) ? _colorDarkYellow : _colorDarkGreen;
			auto active_color = (i % 10 == 0 || i == (_vertical_elements_count - 1)) ? _colorYellow : _colorGreen;
			auto main_color = color;

			for (int j = 0; j < TChannels; j++)
			{
				if ((_vertical_elements_count - _channels[j]) < i)
				{
					main_color = active_color;
				}
				else
				{
					main_color = color;
				}

				x += _element_padding_x;

				canvas.DrawLine(x, y, x + element_width, y, main_color);

				x += element_width;
			}

			y += 1 + _element_padding_y;
		}
	}

private:
	const Color _colorDarkYellow{35, 35, 15 }; //99, 97, 48 
	const Color _colorYellow{ 243, 232, 53 };

	const Color _colorGreen{ 100, 199, 73 };
	const Color _colorDarkGreen{ 9, 14, 7 }; //44, 91, 45

	const Color _colorBlack{ 0, 0, 0 };

	uint8_t _channels[TChannels];
	uint8_t _vertical_elements_count = 10 * 6;
	uint8_t _element_padding_x = 3;
	uint8_t _element_padding_y = 1;
	uint8_t element_width = (_rect.w / TChannels) - _element_padding_x;
};