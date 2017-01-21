#pragma once
#include "System.h"
#include "system_defines.h"
class PhysicsSystem : public System {
public:
	virtual void							update(sf::Time);
	PhysicsSystem(World&);


};
