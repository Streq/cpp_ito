#include "StateSystem.h"
#include "system_defines.h"
#include "components_header.h"
#include "EntityClass.h"
#include "vec_magn.h"
#include "clamp.h"
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
	auto& colinf = mWorld.vec_CollisionInfo[i];
	st.time_since_start += dtime;

	auto time = dtime.asSeconds();
	
	if (st.just_started) {
		st.just_started = false;
		
		//switch (st.previous) {//clean up from previous state
		//	
		//}

		auto& rend = mWorld.vec_Rendering[i];
		//set up current state
		if (st.current < States::player_states) {
			static_cast<sf::RectangleShape*>(rend.drawable.get())->
				setFillColor(
					Character::Stats::color[st.Class][st.current]);
			mWorld.vec_CollisionInfo[i].type = Character::Stats::hitbox_type[st.Class][st.current];
		}
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
				if (st.pressed[j] && st.updated[j]) {
					st.cast(static_cast<Skill::ID>(j));
					break;
				}
			}
		}
		case States::Silenced: {
			mov.velocity += st.moving_dir * Character::Stats::mov_acceleration[st.Class] * time;
		}break;
		case States::Teleport_Scope: {
			mov.velocity += st.moving_dir * Skill::acceleration[Skill::Teleport] * time;
			if (st.updated[st.current_skill] && !st.pressed[st.current_skill]) {
				switch (st.skill_counter) {
					case 0: {//if no wall then teleport
						Handle owner = mWorld.vec_Owner[i].owner;
						//set owner position
						mWorld.vec_Position[owner].setPosition(mWorld.vec_Position[i].getPosition());
						
					}
					case 1: {//if wall then don't teleport
						//kill this mofo scope
						mWorld.remove_entity(i);
					}break;
				}
			}
			st.skill_counter = 0;
		}break;
		case States::Hurt: {
			if (st.time_since_start > st.duration) {
				st.update(States::Normal);
			}; 
		}break;
		case States::Wave_Shot: {

			mov.velocity += st.moving_dir * Skill::acceleration[Skill::Wave_Shot] * time;
			


			if (vec_magn(mov.velocity) >= mov.maxspeed) {
				st.moving_dir = -st.moving_dir;
			}
			

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

								mWorld.make_bullet(pos.getPosition(), st.facing_dir, mov.velocity, Skill::bullet_speed[st.current_skill], std::move(inf), Skill::bullet_duration[st.current_skill], i);
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
								mWorld.make_teleport_scope(pos.getPosition(), Skill::max_speed[Skill::Teleport], sf::Vector2f(1,1)*Character::Stats::size[st.Class], i);
								st.skill_counter++;
							}
						}break;
						case 1: {
							if (!st.pressed[st.current_skill]) {
								st.update(States::Normal);
							}
						}break;
					}
				}break;
				case Skill::Ram: {
					switch (st.skill_counter) {
						case 0: {
							if (st.time_since_start >= Skill::buildup[st.current_skill]) {
								CollisionInfo inf;
								inf = Skill::col_info[st.current_skill];
								inf.damage = Skill::damage[st.current_skill] * Character::Stats::p_attack[st.Class];
								float siz = Character::Stats::size[st.Class];

								mWorld.make_hit_box(sf::Vector2f(SIGN(st.facing_dir.x), SIGN(st.facing_dir.y)) * siz, siz*sf::Vector2f(3, 3), i, std::move(inf), dt_max_fps * 35.f);
								mov.maxspeed = Skill::max_speed[st.current_skill];

								colinf.on_wall = CollisionInfo::bounce;
								st.skill_counter++;
							}
						}break;
						case 1: {
							mov.velocity += st.facing_dir * Skill::acceleration[st.current_skill] * time;
							if (st.time_since_start >= Skill::duration[st.current_skill]) {
								st.update(States::Normal);
								colinf.on_wall = CollisionInfo::stop;

							}
							if (st.wall) {
								st.duration = sf::seconds(0.5f);
								mov.velocity /= 2.f;
								colinf.type = HitBoxType::Invincible;
								st.time_since_start = sf::Time::Zero;
								mWorld.remove_children(i, Relacion::delete_on_hurt);
								st.skill_counter++;
							}
						}break;
						case 2: {
							if (st.time_since_start > st.duration) {
								st.update(States::Normal);
								colinf.on_wall = CollisionInfo::stop;
							}
						}
								
					}
				}break;
				case Skill::Wave_Shot: {
					switch (st.skill_counter) {
					case 0: {
						if (st.time_since_start >= Skill::buildup[st.current_skill]) {
							CollisionInfo inf;// inf2;
							inf = Skill::col_info[st.current_skill];
							inf.damage = Skill::damage[st.current_skill] * Character::Stats::m_attack[st.Class];
							float siz = 5.f;
							//inf2 = inf;
							mWorld.make_wave_bullet(pos.getPosition(), st.facing_dir, Skill::shot_angle[st.current_skill], Skill::acceleration[st.current_skill], Skill::bullet_speed[st.current_skill], std::move(inf), Skill::bullet_duration[st.current_skill], i);
							mWorld.make_wave_bullet(pos.getPosition(), st.facing_dir, -Skill::shot_angle[st.current_skill], Skill::acceleration[st.current_skill], Skill::bullet_speed[st.current_skill], std::move(inf), Skill::bullet_duration[st.current_skill], i);
							//mWorld.make_wave_bullet(pos.getPosition(), st.facing_dir, Skill::shot_angle[st.current_skill]/2, Skill::acceleration[st.current_skill], Skill::bullet_speed[st.current_skill], std::move(inf), Skill::bullet_duration[st.current_skill], i);
							//mWorld.make_wave_bullet(pos.getPosition(), st.facing_dir, -Skill::shot_angle[st.current_skill]/2, Skill::acceleration[st.current_skill], Skill::bullet_speed[st.current_skill], std::move(inf), Skill::bullet_duration[st.current_skill], i);
							mWorld.make_wave_bullet(pos.getPosition(), st.facing_dir, 0, Skill::acceleration[st.current_skill], Skill::bullet_speed[st.current_skill], std::move(inf), Skill::bullet_duration[st.current_skill], i);

							st.skill_counter++;
						}
					}break;


					}
					if (st.time_since_start >= Skill::duration[st.current_skill]) {
						st.update(States::Normal);
					}
				}
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
	st.wall = false;
	ITERATE_END
}


