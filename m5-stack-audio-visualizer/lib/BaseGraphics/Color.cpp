#include "Color.h"

// const Color Color::Black {0, 0, 0, 0};

// const Color Color::White {255, 255, 255, 0};

// const Color Color::Red {255, 0, 0, 0};

// const Color Color::Green {0, 255, 0, 0}

// const Color Color::Blue {0, 0, 255, 0}

Color::operator unsigned int() const
{
	return (_r << 16) | (_g << 8) | _b;
}

Color::operator unsigned short() const
{
	return ((_r >> 3) << 11 | (_g >> 2) << 5 | _b>> 3);
}