#pragma once
#include "System.h"
#include "system_defines.h"
#include "PlayerInput.h"
class ControllerSystem : public System {
public:
	virtual void							update(sf::Time);
	ControllerSystem(World&, PlayerInput&);


	PlayerInput&							player_controller;

};
