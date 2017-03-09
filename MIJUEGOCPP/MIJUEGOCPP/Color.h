#pragma once
#include <SFML\Graphics\Color.hpp>
namespace Color {
	const sf::Color Brown(104, 47, 6);
	const sf::Color Dark_Green(12, 73, 2);
	const sf::Color Super_Dark_Green(0,30,0);
	const sf::Color Dark_Blue(1, 5, 81);
	const sf::Color Grey(128,128,128);
#define X(Col) const sf::Color Col=sf::Color::##Col;
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
