#pragma once

#include "UIElement.h"

template <size_t TChannels>
class UISoundAnalyzer : public UIElement
{
public:
	UISoundAnalyzer(const UIRect& rect) : UIElement(rect)
	{ 
		for (int j = 0; j < count(); j++){
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

		auto x = origin_x;
		

		for (int j = 0; j < TChannels; j++) 
		{	
			auto y = 0;

			x += _element_padding_x;

			canvas.SpriteEnd();
			canvas.SpriteBegin(_element_width, _rect.h, TFT_BLACK);

			for (auto i = 0; i < _vertical_elements_count; i++)
			{
				auto color_off = _colorDarkGreen;
				auto color_on = _colorGreen;
				auto main_color = color_off;

				if (i % 10 == 0 || i == last_row_index) {
					color_off = _colorDarkYellow;
					color_on = _colorYellow;
				}

				main_color = ((_vertical_elements_count - _channels[j]) <= i) ? color_on : color_off;		

				canvas.SpriteDrawLine(0, y, _element_width, y, main_color);

				y += (1 + _element_padding_y);
			}
	
			canvas.SpritePush(x, origin_y);
			lgfx::delayMicroseconds(250);
			x += _element_width;
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