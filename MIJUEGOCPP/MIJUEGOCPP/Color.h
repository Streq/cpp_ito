#pragma once
#include <SFML\Graphics\Color.hpp>
namespace Color {
	const extern sf::Color& Brown;
	const extern sf::Color& Dark_Green;
	const extern sf::Color& Super_Dark_Green;
	const extern sf::Color& Dark_Blue;
	const extern sf::Color& Grey;
#define X(Col) const extern sf::Color& Col;
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
}
