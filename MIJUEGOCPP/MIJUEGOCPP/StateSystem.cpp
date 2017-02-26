#include "StateSystem.h"
#include "system_defines.h"
#include "components_header.h"
#include "EntityClass.h"
#include "vec_magn.h"

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

void StateSystem::update(sf::Time dtime){
	ITERATE_START	
	auto& st = mWorld.vec_State[i];
	auto& mov = mWorld.vec_Movement[i];
	auto& pos = mWorld.vec_Position[i];
	st.time_since_start += dtime;

	auto time = dtime.asSeconds();
	
	if (st.just_started) {
		st.just_started = false;
		
		switch (st.previous) {//clean up from previous state
			
		}

		auto& rend = mWorld.vec_Rendering[i];
		//set up current state
		static_cast<sf::RectangleShape*>(rend.drawable.get())->
			setFillColor(
				Character::Stats::color[st.Class][st.current]);
		
		mWorld.vec_CollisionInfo[i].type = Character::Stats::hitbox_type[st.Class][st.current];
		switch (st.current) {
			case States::Hurt:{
				//mWorld.remove_component<Controller>(i);
				mWorld.remove_children(i, Relacion::delete_on_hurt);
				mov.capped = false;
			}break;
			case States::Normal:{
				mov.capped = true;
				mWorld.activate_component<Controller>(i);
				mov.maxspeed = Character::Stats::mov_speed[st.Class];
				mov.friction = Character::Stats::mass[st.Class] * Character::Stats::friction[st.Class];
			}break;
			case States::Casting: {
				
			}break;
		}
	}
	//do regular frame stuff
	switch (st.current) {
		case States::Normal: {
			for (int j = 0; j < Skill::size; j++) {
				if (st.pressed[j]&&st.updated[j]) {
					st.cast(static_cast<Skill::ID>(j));
					break;
				}
			}
		}
		case States::Silenced:{
			mov.velocity += st.moving_dir * Character::Stats::mov_acceleration[st.Class] * time;
		}break;
		case States::Hurt: {
			if (st.time_since_start > st.duration) {
				st.update(States::Normal);
			}; 
		}break;
		case States::Casting: {
			switch (st.current_skill) {
				case Skill::Simple_Melee: {
					switch (st.skill_counter) {
						case 0:{
							if(st.time_since_start>=Skill::buildup[st.current_skill]){
								CollisionInfo inf;
								inf = Skill::col_info[st.current_skill];
								inf.damage = Skill::damage[st.current_skill] * Character::Stats::p_attack[st.Class];
								float siz = Character::Stats::size[st.Class];
								
								mWorld.make_hit_box(sf::Vector2f(SIGN(st.facing_dir.x),SIGN(st.facing_dir.y)) * siz, siz*sf::Vector2f(3,3) ,i ,std::move(inf),dt_max_fps*10.f);
								st.skill_counter++;
							}
						}break;
					}
					if (st.time_since_start >= Skill::duration[st.current_skill]) {
						st.update(States::Normal);
					}
				}break;
				case Skill::Simple_Shot:
				case Skill::Bounce_Shot:{
					switch (st.skill_counter) {
						case 0: {
							if (st.time_since_start >= Skill::buildup[st.current_skill]) {
								CollisionInfo inf;
								inf = Skill::col_info[st.current_skill];
								inf.damage = Skill::damage[st.current_skill] * Character::Stats::m_attack[st.Class];
								float siz = 5.f;

								mWorld.make_bullet(pos.getPosition(), st.facing_dir, mov.velocity, Skill::bullet_speed[st.current_skill], std::move(inf), i);
								st.skill_counter++;
							}
						}break;
							
						
					}
					if (st.time_since_start >= Skill::duration[st.current_skill]) {
						st.update(States::Normal);
					}
				}break;
				case Skill::Teleport: {
					switch (st.skill_counter) {
						case 0: {
							if (st.time_since_start >= Skill::buildup[st.current_skill]) {
								mWorld.make_teleport_scope(pos.getPosition(), sf::Vector2f(1,1)*Character::Stats::size[st.Class], i);
								st.skill_counter++;
							}
						}break;
						case 1: {
							if (!st.pressed[st.current_skill]) {
								st.update(States::Normal);
							}
						}
					}
				}break;
			}
		}break;
		case States::Spawner: {
			if (st.time_since_start >= st.duration) {
				auto& pos = mWorld.vec_Position[i];
				mWorld.make_zombie(pos.getPosition());
				auto aux = st.duration;
				st.update(States::Spawner);
				st.duration = aux;
			}
		}break;
		case States::HitBox: {
			if (st.duration>=sf::Time::Zero)
			if (st.time_since_start > st.duration) {
				mWorld.remove_entity(i);
			}

		}break;
	}
	ITERATE_END
}


