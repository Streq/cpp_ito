#pragma once
#include <SFML/Graphics/Text.hpp>
void centerOrigin(sf::Text& text){
	auto bounds = text.getLocalBounds();
	text.setOrigin(std::floor(bounds.left + bounds.width / 2.f),std::floor(bounds.top + bounds.height / 2.f));
}