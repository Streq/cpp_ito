#pragma once
#include "System.h"
#include "system_defines.h"
#include "PlayerInput.h"
class ControllerSystem : public System {
public:

	typedef std::array<PlayerInput,MAX_PLAYERS> controllers;
	virtual void							update(sf::Time);
	ControllerSystem(World&, controllers&);

	controllers&							player_controller;

};
