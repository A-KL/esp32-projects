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

	bool setBand(uint8_t channel, uint8_t value)
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

		AbsolutePosition(origin_x, origin_y);

		const auto last_row_index = _vertical_elements_count - 1;
		auto y = origin_y - 4; // TMP

		for (auto i = 0; i < _vertical_elements_count; i++)
		{
			canvas.SpriteBegin(_rect.w, 1, TFT_BLACK);

			auto x = 0;
			auto color_off = _colorDarkGreen;
			auto color_on = _colorGreen;
			auto main_color = color_off;

			if (i % 10 == 0 || i == last_row_index) {
				color_off = _colorDarkYellow;
				color_on = _colorYellow;
			}

			for (int j = 0; j < TChannels; j++) {
				main_color = ((_vertical_elements_count - _channels[j]) <= i) ? color_on : color_off;

				x += _element_padding_x;

				canvas.SpriteDrawLine(x, 0, x + _element_width, 0, main_color);

				x += _element_width;
			}

			canvas.SpritePush(origin_x, y);
			canvas.SpriteEnd();

			y += (1 + _element_padding_y);
		}
	}

private:
	const Color _colorDarkYellow { 35, 35, 15 }; //99, 97, 48 
	const Color _colorYellow { 243, 232, 53 };

	const Color _colorGreen { 100, 199, 73 };
	const Color _colorDarkGreen { 9, 14, 7 }; //44, 91, 45

	const Color _colorBlack{ 0, 0, 0 };

	uint8_t _channels[TChannels];

	const uint8_t _vertical_elements_count = 10 * 6;
	const uint8_t _element_padding_x = 3;
	const uint8_t _element_padding_y = 1;
	const uint8_t _element_width = (_rect.w / TChannels) - _element_padding_x;
};