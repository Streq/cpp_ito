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
	st.time_since_start += dtime;
	if (st.just_started) {
		st.just_started = false;
		auto& mov = mWorld.vec_Movement[i];

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
				mWorld.remove_component<Controller>(i);
			}break;
			case States::Normal:{
				mWorld.activate_component<Controller>(i);
				mov.maxspeed = Character::Stats::mov_speed[st.Class];

			}break;
			case States::Casting: {
				mWorld.remove_component<Controller>(i);

			}break;
		}
	}
	//do regular frame stuff
	switch (st.current) {
		case States::Normal: {
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
								
								mWorld.make_hit_box(sf::Vector2f(SIGN(st.facing_dir.x),SIGN(st.facing_dir.y)) * siz, siz*sf::Vector2f(3,3) ,i ,std::move(inf),sf::seconds(0.08));
								st.skill_counter++;
							}
						}break;
					}
				}break;
			}
			if (st.time_since_start > Skill::duration[st.current_skill]) {
				st.update(States::Normal);
			};
		}break;
		case States::Spawner: {
			if (st.time_since_start > st.duration) {
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


