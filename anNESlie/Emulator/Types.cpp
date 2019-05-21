#include "Types.h"

Color::Color()
{
	R = 0;
	G = 0;
	B = 0;
}

Color::Color(DWord RGB)
{
	R = RGB;
	G = RGB >> 8;
	B = RGB >> 16;
}
