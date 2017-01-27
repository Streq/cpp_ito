#pragma once
#include "Component.h"
#include "SFML\Graphics\Drawable.hpp"
struct Rendering :public Component, public sf::Transformable {
	std::unique_ptr<sf::Drawable> drawable;
};
