#include "ControllerSystem.h"
#include "system_defines.h"
#include "vec_magn.h"
#include <iostream>
#include "components_header.h"
#include "clamp.h"
#include "Player.h"
ControllerSystem::ControllerSystem(World& world, controllers& input)
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
	const bool* pressed[players];
	const bool* updated[players];
	
	for (int i2 = 0; i2 < players; i2++) {
		updated[i2] = player_controller[i2].get_updated();
		pressed[i2] = player_controller[i2].get_pressed();
	}
	

	ITERATE_START
	Controller &c = mWorld.vec_Controller[i];
	auto& state = mWorld.vec_State[i];
	auto& mov = mWorld.vec_Movement[i];
	auto& pos = mWorld.vec_Position[i];
	auto& mpressed = pressed[c.player];
	auto& mupdated = updated[c.player];
	
	switch (c.controller) {
		case controller::Player: {

			sf::Vector2f dir = normalize(sf::Vector2f(
				static_cast<float>(mpressed[Input::right] - mpressed[Input::left]),
				static_cast<float>(mpressed[Input::down] - mpressed[Input::up])));

			for (unsigned j = Input::skill1; j < Input::skill1 + skill_num; j++) {
				Skill::ID _skill = Character::Stats::skill[state.Class][j - Input::skill1];
				state.updated[_skill] = mupdated[j];
				state.pressed[_skill] = mpressed[j];
				
			}
			switch (state.current) {
				case States::Normal: {
					
				}//no break on purpose
				case States::Silenced: {
					
					state.moving_dir = dir;
					if (state.moving_dir.x != 0 || state.moving_dir.y != 0) {
						state.facing_dir = state.moving_dir;
					}
					
					
				}break;
				case States::Casting: {
					
				}break;
				case States::Teleport_Scope:{
					sf::Vector2f dir = normalize(sf::Vector2f(
						static_cast<float>(mpressed[Input::right] - mpressed[Input::left]),
						static_cast<float>(mpressed[Input::down] - mpressed[Input::up])));
					state.moving_dir = dir;
					
				}break;
			}
		}break;

		case controller::Enemy: {

			if (c.target==Handle(-1) || mWorld.vec_Controller[c.target].controller != controller::Player || !mWorld.vec_Entity[i][type::Active]) {
				//look for closest player_states
				float distance = -1;
				Handle current_player = -1;
				for (int j = 0; j < players; j++) {
					Handle player = mWorld.vec_players[j];
					if (entities[player] && mWorld.vec_Controller[player].controller==controller::Player) {
						if (distance < 0) {
							current_player = player;
							distance = vec_magn(pos.getPosition() - mWorld.vec_Position[player].getPosition());
						}
						else {
							auto dis_pl =vec_magn(pos.getPosition() - mWorld.vec_Position[player].getPosition());
							if (dis_pl < distance) {
								distance = dis_pl;
								current_player = player;
							}	
						}
					}	
				}
				c.target = current_player;
			}
			if (c.target != Handle(-1)) {
				state.moving_dir = normalize(mWorld.vec_Position[c.target].getPosition() - mWorld.vec_Position[i].getPosition());

			}
		}break;
	}
	ITERATE_END
}
