#pragma once
#include "Component.h"
#include "SFML\Graphics\Transformable.hpp"
struct Position : public Component, public sf::Transformable {
	sf::Vector2f frame_start_position;
};

/*
struct Position : public Component {
sf::Vector2f position;
float angle;
};
*/