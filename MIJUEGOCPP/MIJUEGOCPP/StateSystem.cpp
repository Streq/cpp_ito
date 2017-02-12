#include "StateSystem.h"
#include "system_defines.h"
#include "components_header.h"



StateSystem::StateSystem(World& world)
	:System(
		world,
		Requeriments(
			Flagset(
				#define X(C) flag::##C |
				COMPONENTS_STATE
				#undef X
				flag::none
				),
			Flagset(flag::none)
			)
		)
{
}

void StateSystem::update(sf::Time dt){
	ITERATE_START	
	auto& st = mWorld.vec_State[i];
	st.time_since_start += dt;
	if (st.just_started) {
		st.just_started = false;
		switch (st.previous) {//clean up from previous state
			case States::Player_Melee: 
			case States::Player_Ducking:
			case States::Player_Hurt:{
				mWorld.activate_component<Controller>(i);
				auto& mov = mWorld.vec_Movement[i];
				mov.maxspeed = Player::stats::max_speed;
				mov.friction = Player::stats::friction;
				auto& health = mWorld.vec_Health[i];
				health.invulnerable = false;
			}break;
			case States::Enemy_Hurt:{
				auto& mov = mWorld.vec_Movement[i];
				mov.maxspeed = Enemy::stats::max_speed;
				mov.friction = Enemy::stats::friction;
				mWorld.activate_component<Controller>(i);
			}break;
		}

		auto& rend = mWorld.vec_Rendering[i];
		static_cast<sf::RectangleShape*>(rend.drawable.get())->setFillColor(States::Color[st.current]);
		switch (st.current) {//set up current state
			case States::Player_Melee: {
				mWorld.remove_component<Controller>(i);
				auto& mov = mWorld.vec_Movement[i];
				mov.maxspeed = Player::stats::melee_max_speed;
				mov.friction = Player::stats::melee_friction;
				if (mWorld.vec_Controller[i].player == 1) {
					static_cast<sf::RectangleShape*>(rend.drawable.get())->setFillColor(Color::Cyan);
				}
			}break;
			case States::Player_Normal:
			case States::Player_Teleporting:{
				if(mWorld.vec_Controller[i].player == 1){
					static_cast<sf::RectangleShape*>(rend.drawable.get())->setFillColor(Color::Cyan);
				}
			}break;
			case States::Player_Ducking:{
				mWorld.remove_component<Controller>(i);
				auto& mov = mWorld.vec_Movement[i];
				mov.maxspeed = Player::stats::ducking_max_speed;
				mov.friction = Player::stats::ducking_friction;
			}break;
			case States::Player_Hurt:{
				mWorld.remove_component<Controller>(i);
				auto& mov = mWorld.vec_Movement[i];
				mov.maxspeed = Player::stats::hurt_max_speed;
				mov.friction = Player::stats::hurt_friction;
				auto& health = mWorld.vec_Health[i];
				health.invulnerable = true;
			}break;
			case States::Enemy_Normal:{
				

			}break;
			case States::Enemy_Hurt: {
				auto& mov = mWorld.vec_Movement[i];
				mov.maxspeed = 600.f;
				mov.friction = 1000.f;
				mWorld.remove_component<Controller>(i);
				mWorld.vec_Health[i].invulnerable = true;
			}break;

		}
	}
	switch (st.current) {
		case States::Player_Normal: {
		}break;
			
		case States::Player_Ducking:{
			if (st.time_since_start > st.duration) {
				st.update(States::Player_Normal);
			}
		}break;
		case States::Enemy_Hurt: {
			if(st.time_since_start > Enemy::stats::inv_time){
				mWorld.vec_Health[i].invulnerable = false;
				if (st.time_since_start > st.duration) {
					st.update(States::Enemy_Normal);
				}
			}
		}break;
		case States::Player_Hurt: {
			if (st.time_since_start > st.duration) {
				st.update(States::Player_Normal);
			}
		}break;
		case States::Spawner: {
			if (st.time_since_start > st.duration) {
				auto& pos = mWorld.vec_Position[i];
				mWorld.make_enemy(pos.getPosition());
				auto aux = st.duration;
				st.update(States::Spawner);
				st.duration = aux;
			}
		}break;
		case States::Hit_Box: {
			if (st.time_since_start > st.duration) {
				mWorld.remove_entity(i);
			}

			auto own=mWorld.vec_Team[i].owner;
			mWorld.vec_Position[i].setPosition(
				mWorld.vec_Position[own].getPosition()
			);
			mWorld.vec_Movement[i] = mWorld.vec_Movement[own];
		}break;
		case States::Player_Melee: {
			if (st.time_since_start > st.duration) {
				st.update(States::Player_Normal);
			}
		}break;
	}
	ITERATE_END
}


