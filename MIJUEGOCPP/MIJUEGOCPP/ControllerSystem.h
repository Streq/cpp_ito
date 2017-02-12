#pragma once
#include "System.h"
#include "system_defines.h"
#include "PlayerInput.h"
class ControllerSystem : public System {
public:

	typedef PlayerInput controllers[players];
	virtual void							update(sf::Time);
	ControllerSystem(World&, controllers&);

	controllers&							player_controller;

};
