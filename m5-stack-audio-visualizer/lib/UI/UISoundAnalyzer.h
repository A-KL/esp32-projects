#pragma once

#include "UIElement.h"

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