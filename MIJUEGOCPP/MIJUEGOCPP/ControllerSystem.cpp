#include "ControllerSystem.h"
#include "system_defines.h"
#include "vec_magn.h"
#include <iostream>
#include "components_header.h"
#include "clamp.h"
#include "Player.h"
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
	auto& pressed = player_controller.get_pressed();
	auto& updated = player_controller.get_updated();

	ITERATE_START
	Controller &c = mWorld.vec_Controller[i];
	auto& state = mWorld.vec_State[i];
	auto& mov = mWorld.vec_Movement[i];
	auto& pos = mWorld.vec_Position[i];
	switch (c.controller) {
		case controller::Player: {
			switch (state.current) {
				case States::Normal: {
					sf::Vector2f dir = normalize(sf::Vector2f(
						static_cast<float>(pressed[Input::right] - pressed[Input::left]),
						static_cast<float>(pressed[Input::down] - pressed[Input::up])));
					c.moving_dir = dir;
					if (c.moving_dir.x != 0 || c.moving_dir.y != 0) {
						c.facing_dir = c.moving_dir;
					}
					mWorld.vec_Movement[i].velocity += c.moving_dir*Player::stats::acceleration*time;
					if (pressed[Input::shoot] && updated[Input::shoot]) {
						const auto& pos = mWorld.vec_Position[i];
						mWorld.make_bullet(pos.getPosition(), c.facing_dir, mov.velocity, Bullet::stats::speed, i);
					}

					if (pressed[Input::duck] && updated[Input::duck]) {
						mov.velocity = dir*600.f + mov.velocity;
						state.update(States::Ducking);
					}
					if (pressed[Input::teleport] && updated[Input::teleport]) {
						state.update(States::Teleporting);
						mWorld.make_teleport_scope(pos.getPosition(), sf::Vector2f(1.f,1.f)*Player::stats::size, i);
					}
				}break;
				case States::Teleport_Scope:{
					sf::Vector2f dir = normalize(sf::Vector2f(
						static_cast<float>(pressed[Input::right] - pressed[Input::left]),
						static_cast<float>(pressed[Input::down] - pressed[Input::up])));
					c.moving_dir = dir;
					
					mWorld.vec_Movement[i].velocity += c.moving_dir*Teleport_Scope::stats::acceleration*time;


					if (!pressed[Input::teleport]) {
						Handle owner = mWorld.vec_Team[i].owner;
						//teleport
						mWorld.vec_Position[owner].setPosition(mWorld.vec_Position[i].getPosition());
						//change state to normal
						mWorld.vec_State[owner].update(States::Normal);
						//kill this mofo scope
						mWorld.remove_entity(i);
					}
				}break;
			}
		}break;

		case controller::Enemy: {
				//look for a player
				Handle player = 0;
				for (size_t j = 0; j < max_entities; j++)
					if (TESTMASK(mWorld.vec_Entity[j],(flag::Active|flag::CollisionBody)) && mWorld.vec_CollisionTag[j].tag == Tag::Player) {
						player = j;
						break;
					}
				auto dir = normalize(mWorld.vec_Position[player].getPosition() - mWorld.vec_Position[i].getPosition());
				mov.velocity += dir*Enemy::stats::acceleration*time;
		}break;
	}
	ITERATE_END
}
