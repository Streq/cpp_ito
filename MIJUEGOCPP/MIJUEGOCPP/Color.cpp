#include "Color.h"

const sf::Color& Color::Brown = sf::Color(104, 47, 6);
const sf::Color& Color::Dark_Green = sf::Color(12, 73, 2);
const sf::Color& Color::Super_Dark_Green = sf::Color(0,30,0);
const sf::Color& Color::Dark_Blue = sf::Color(1, 5, 81);
const sf::Color& Color::Grey = sf::Color(128,128,128);

#define X(Col) const sf::Color& Color::Col = sf::Color::##Col;
	X(Blue)
	X(Black)
	X(Cyan)
	X(Green)
	X(Magenta)
	X(Red)
	X(Transparent)
	X(White)
X(Yellow)
#undef X
