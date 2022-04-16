#include "Color.h"

const Color Color::Black {0, 0, 0, 0};

const Color Color::White {255, 255, 255, 0};

const Color Color::Red {255, 0, 0, 0};

const Color Color::Green {0, 255, 0, 0};

const Color Color::DarkGreen {21, 178, 0, 0};

const Color Color::Blue {0, 0, 255, 0};

const Color Color::Orange {255, 165, 0, 0};

const Color Color::LightBlue {5, 117, 255, 0};

const Color Color::Purpule {178, 0, 255, 0};

const Color Color::Yellow {255, 255, 0, 0};

const Color Color::Pink {255, 0, 165, 0};

const Color Color::Gray { 56, 56, 56, 0 };

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