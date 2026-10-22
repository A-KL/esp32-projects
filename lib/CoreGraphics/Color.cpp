#include "Color.h"

Color::operator unsigned int() const
{
	return (_r << 16) | (_g << 8) | _b;
}

Color::operator unsigned short() const
{
	return ((_r >> 3) << 11 | (_g >> 2) << 5 | _b>> 3);
}

bool Color::operator ==(const Color& other) const 
{
	return _r == other._r &&
		   _g == other._g &&
		   _b == other._b &&
		   _a == other._a;
}

inline const Color Color::Black {0, 0, 0, 0};
inline const Color Color::White {255, 255, 255, 0};
inline const Color Color::Red {255, 0, 0, 0};
inline const Color Color::Green {0, 255, 0, 0};
inline const Color Color::DarkGreen {21, 178, 0, 0};
inline const Color Color::Blue {0, 0, 255, 0};
inline const Color Color::Orange {255, 165, 0, 0};
inline const Color Color::LightBlue {5, 117, 255, 0};
inline const Color Color::Purple {178, 0, 255, 0};
inline const Color Color::Yellow {255, 255, 0, 0};
inline const Color Color::Pink {255, 0, 165, 0};
inline const Color Color::Gray {56, 56, 56, 0};
inline const Color Color::LightGray {179, 179, 179, 0};
inline const Color Color::DarkGray { 20, 20, 20, 0 };