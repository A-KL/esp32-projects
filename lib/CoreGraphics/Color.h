#pragma once

struct Color
{
	constexpr Color(unsigned char r=0, unsigned char g=0, unsigned char b=0, unsigned char a=0)
	: _r(r), _g(g), _b(b), _a(a)
	{}

	unsigned char _r = 0, _g = 0, _b = 0, _a = 0;

	operator unsigned int() const;

	operator unsigned short() const;

	bool operator==(const Color& other) const;

	const static Color Black;

	const static Color White;

	const static Color Red;

	const static Color Green;

	const static Color DarkGreen;

	const static Color Blue;

	const static Color Orange;

	const static Color LightBlue;

	const static Color Purpule;

	const static Color Yellow;

	const static Color Pink;

	const static Color Gray;

	const static Color LightGray;
};

inline const Color Color::Black {0, 0, 0, 0};
inline const Color Color::White {255, 255, 255, 0};
inline const Color Color::Red {255, 0, 0, 0};
inline const Color Color::Green {0, 255, 0, 0};
inline const Color Color::DarkGreen {21, 178, 0, 0};
inline const Color Color::Blue {0, 0, 255, 0};
inline const Color Color::Orange {255, 165, 0, 0};
inline const Color Color::LightBlue {5, 117, 255, 0};
inline const Color Color::Purpule {178, 0, 255, 0};
inline const Color Color::Yellow {255, 255, 0, 0};
inline const Color Color::Pink {255, 0, 165, 0};
inline const Color Color::Gray {56, 56, 56, 0};
inline const Color Color::LightGray {179, 179, 179, 0};