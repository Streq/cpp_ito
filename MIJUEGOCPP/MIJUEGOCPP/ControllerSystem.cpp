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

			switch (state.current) {
				case States::Normal: {
					for (unsigned j = Input::skill1; j < Input::skill1 + skill_num; j++){
						
						if (mpressed[j] && mupdated[j]) {
							state.cast(Character::Stats::skill[state.Class][j - Input::skill1]);
						}
					}
				}//no break on purpose
				case States::Silenced: {
					
					state.moving_dir = dir;
					if (state.moving_dir.x != 0 || state.moving_dir.y != 0) {
						state.facing_dir = state.moving_dir;
					}
					
					mWorld.vec_Movement[i].velocity += state.moving_dir * Character::Stats::mov_acceleration[state.Class] * time;
					
				}break;
				case States::Casting: {
					/*
					if (state.skill_counter == 0 && state.time_since_start >= Character::Stats::melee_hitbox_start_frame[state.Class]) {
						state.skill_counter++;
						const auto& c = mWorld.vec_Controller[i];
						auto& mov = mWorld.vec_Movement[i];
						sf::Vector2f off(SIGN(c.facing_dir.x), SIGN(c.facing_dir.y));
						CollisionInfo collinfo;
						collinfo.damage = Character::Stats::melee_hitbox_damage[state.Class];
						collinfo.type = HitBoxType::Hit;
						collinfo.stun_time = Character::Stats::melee_hitbox_stun_duration[state.Class];
						collinfo.tag = Tag::Hit_Box;
						off *= Character::Stats::melee_offset[state.Class];
						collinfo.knockback = Character::Stats::melee_hitbox_knockback[state.Class];
						mov.velocity += normalized(c.facing_dir) * Character::Stats::impulse[state.Class][state.current];
						auto siz = Character::Stats::melee_hitbox_size_factor[state.Class] * Character::Stats::size[state.Class];
						sf::Vector2f size(siz, siz);
						mWorld.make_hit_box(off, size, i, std::move(collinfo), Character::Stats::melee_hitbox_duration[state.Class]);
						
					}*/
				}break;
				case States::Teleport_Scope:{
					sf::Vector2f dir = normalize(sf::Vector2f(
						static_cast<float>(mpressed[Input::right] - mpressed[Input::left]),
						static_cast<float>(mpressed[Input::down] - mpressed[Input::up])));
					state.moving_dir = dir;
					
					mWorld.vec_Movement[i].velocity += state.moving_dir*mWorld.vec_Movement[i].diracceleration*time;


					if (!mpressed[Input::skill4]) {
						Handle owner = mWorld.vec_Owner[i].owner;
						//skill4
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
				mov.velocity += dir*Character::Stats::mov_acceleration[state.Class]*time;
			}
		}break;
	}
	ITERATE_END
}
