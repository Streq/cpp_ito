#pragma once
#include "defines.h"
#include "Component.h"
#include "SFML\Graphics\Transformable.hpp"
struct Position : public Component, public sf::Transformable {
	sf::Vector2f frame_start_position;
	Handle relative_to = MAX_ENTITIES;
};

/*
struct Position : public Component {
sf::Vector2f position;
float angle;
};
*/