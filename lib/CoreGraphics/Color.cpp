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