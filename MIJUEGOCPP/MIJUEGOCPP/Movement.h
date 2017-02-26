#pragma once
#include "Component.h"
struct Movement : public Component {
	sf::Vector2f velocity;
	sf::Vector2f frame_start_velocity;
	sf::Vector2f acceleration;
	float diracceleration;
	float rotation;
	float friction;
	float maxspeed;
	bool capped=true;
};
