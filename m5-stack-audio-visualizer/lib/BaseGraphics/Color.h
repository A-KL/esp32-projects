struct Color
{
	Color(unsigned char r=0, unsigned char g=0, unsigned char b=0, unsigned char a=0)
	: _r(r), _g(g), _b(b), _a(a)
	{}

	unsigned char _r = 0, _g = 0, _b = 0, _a = 0;

	operator unsigned int() const;

	operator unsigned short() const;

	const static Color Black;

	const static Color White;

	const static Color Red;

	const static Color Green;

	const static Color Blue;

	const static Color Orange;

	const static Color LightBlue;

	const static Color Gray;
};