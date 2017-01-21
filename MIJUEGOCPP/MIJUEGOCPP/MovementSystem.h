#pragma once
#include "System.h"
#include "system_defines.h"
class MovementSystem : public System {
public:
	virtual void							update(sf::Time);
	MovementSystem(World&);
};
