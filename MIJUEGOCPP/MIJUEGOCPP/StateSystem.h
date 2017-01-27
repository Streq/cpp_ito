#pragma once
#include "System.h"
#include "system_defines.h"
class StateSystem : public System {
public:
	virtual void							update(sf::Time);
	StateSystem(World&);
	
};
