#pragma once
#include "Component.h"
#include "SFML\System.hpp"
struct TimeSpan : public Component {
	sf::Time time;
};