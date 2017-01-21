#include "ControllerSystem.h"
#include "system_defines.h"
#include "vec_magn.h"
#include <iostream>
ControllerSystem::ControllerSystem(World& world, PlayerInput& input)
	:System(
		world,
		Requeriments(
			Flagset(
				#define X(C) flag::##C |
				COMPONENTS_CONTROLLER
				#undef X
				flag::none
				),
			Flagset(flag::none)
			)
		),
	player_controller(input)
{}

void ControllerSystem::update(sf::Time t) {
	float time = t.asSeconds();
	ITERATE_START
		Controller &c = mWorld.vec_Controller[i];
		if (c.controller == controller::Player) {
			auto& pressed = player_controller.get_pressed();
			auto& updated = player_controller.get_updated();
			sf::Vector2f dir = normalize(sf::Vector2f(static_cast<float>(pressed[Input::right] - pressed[Input::left]), static_cast<float>(pressed[Input::down] - pressed[Input::up])));

		}
	ITERATE_END
}
