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
	
	for (int i = 0; i < players; i++) {
		updated[i] = player_controller[i].get_updated();
		pressed[i] = player_controller[i].get_pressed();
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
			switch (state.current) {
				case States::Player_Normal: {
					sf::Vector2f dir = normalize(sf::Vector2f(
						static_cast<float>(mpressed[Input::right] - mpressed[Input::left]),
						static_cast<float>(mpressed[Input::down] - mpressed[Input::up])));
					c.moving_dir = dir;
					if (c.moving_dir.x != 0 || c.moving_dir.y != 0) {
						c.facing_dir = c.moving_dir;
					}
					mWorld.vec_Movement[i].velocity += c.moving_dir*Player::stats::acceleration*time;
					if (mpressed[Input::shoot] && mupdated[Input::shoot]) {
						const auto& pos = mWorld.vec_Position[i];
						mWorld.make_bullet(pos.getPosition(), c.facing_dir, mov.velocity, Bullet::stats::speed, i);
					}

					if (mpressed[Input::duck] && mupdated[Input::duck]) {
						mov.velocity += dir*600.f;
						state.update(States::Player_Ducking);
					}
					if (mpressed[Input::teleport] && mupdated[Input::teleport]) {
						state.update(States::Player_Teleporting);
						mWorld.make_teleport_scope(pos.getPosition(), sf::Vector2f(1.f,1.f)*Player::stats::size, i);
					}
					if (mpressed[Input::melee] && mupdated[Input::melee]) {
						sf::Vector2f off(SIGN(c.facing_dir.x),SIGN(c.facing_dir.y));
						off *= Hit_Box::stats::offset;
						mov.velocity += normalized(c.facing_dir) * Player::stats::melee_impulse;
						sf::Vector2f siz(Hit_Box::stats::size, Hit_Box::stats::size);
						mWorld.make_hit_box(off, siz, i);
						state.update(States::Player_Melee);

					}
				}break;
				case States::Teleport_Scope:{
					sf::Vector2f dir = normalize(sf::Vector2f(
						static_cast<float>(mpressed[Input::right] - mpressed[Input::left]),
						static_cast<float>(mpressed[Input::down] - mpressed[Input::up])));
					c.moving_dir = dir;
					
					mWorld.vec_Movement[i].velocity += c.moving_dir*Teleport_Scope::stats::acceleration*time;


					if (!mpressed[Input::teleport]) {
						Handle owner = mWorld.vec_Team[i].owner;
						//teleport
						mWorld.vec_Position[owner].setPosition(mWorld.vec_Position[i].getPosition());
						//change state to normal
						mWorld.vec_State[owner].update(States::Player_Normal);
						//kill this mofo scope
						mWorld.remove_entity(i);
					}
				}break;
			}
		}break;

		case controller::Enemy: {

			if (c.target==Handle(-1) || mWorld.vec_Controller[c.target].controller != controller::Player || !mWorld.vec_Entity[i][type::Active]) {
				//look for closest player
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
				auto dir = normalize(mWorld.vec_Position[c.target].getPosition() - mWorld.vec_Position[i].getPosition());
				mov.velocity += dir*Enemy::stats::acceleration*time;
			}
		}break;
	}
	ITERATE_END
}
