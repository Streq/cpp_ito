#include "StateSystem.h"
#include "system_defines.h"
#include "components_header.h"
#include "EntityClass.h"
#include "vec_magn.h"
#include "clamp.h"
#include <SFML/Graphics/RectangleShape.hpp>
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
		){
}

void StateSystem::update(sf::Time dtime){
	ITERATE_START
		auto& st = mWorld.vec_State[i];
	auto& mov = mWorld.vec_Movement[i];
	auto& pos = mWorld.vec_Position[i];
	auto& colinf = mWorld.vec_CollisionInfo[i];
	st.time_since_start += dtime;

	auto time = dtime.asSeconds();

	if(st.just_started){
		st.just_started = false;

		//switch (st.previous) {//clean up from previous state
		//	
		//}

		auto& rend = mWorld.vec_Rendering[i];
		//set up current state
		if(st.current < States::player_states){

			static_cast<sf::RectangleShape*>(rend.drawable.get())->
				setFillColor(
					Character::Stats::color[st.Class][st.current]);
			
			colinf.oTag = (OTag::ID)Character::Stats::offensive_hitbox_type[st.Class][st.current];
			colinf.dTag = (DTag::ID)Character::Stats::defensive_hitbox_type[st.Class][st.current];
			colinf.pTag = (PTag::ID)Character::Stats::physical_box_type[st.Class][st.current];
		}
		switch(st.current){
			case States::Hurt:
			case States::Stunned:
			{

				//mWorld.remove_component<Controller>(i);
				mWorld.remove_children(i, Relacion::delete_on_hurt);
				mov.capped = false;
			}break;
			case States::Normal:
			{
				mov.capped = true;
				mWorld.activate_component<Controller>(i);
				mov.maxspeed = Character::Stats::mov_speed[st.Class];
				mov.friction = Character::Stats::mass[st.Class] * Character::Stats::friction[st.Class];
				colinf.on_wall = (CollisionInfo::On_Wall)Character::Stats::on_wall[st.Class];
			}break;
			case States::Casting:
			{

			}break;
		}
	}
	//do regular frame stuff
	switch(st.current){
		case States::Normal:
		{

			if(st.time_since_start>=st.duration){
				st.update(States::Normal);
			}
			for(int j = 0; j < Skill::size; j++){
				if(st.pressed[j] && st.updated[j]){
					
					st.cast(static_cast<Skill::ID>(j));
					break;
				}
			}
		}
		case States::Silenced:
		{
			mov.velocity += st.moving_dir * Character::Stats::mov_acceleration[st.Class] * time;
		}break;
		case States::Teleport_Scope:
		{
			if(st.updated[st.current_skill] && !st.pressed[st.current_skill]){
				if(!st.wall){
					Handle owner = mWorld.vec_Owner[i].owner;
					//set owner position
					mWorld.vec_Position[owner].setPosition(mWorld.vec_Position[i].getPosition());
				}

				mWorld.remove_entity(i);
			}
			mov.velocity += st.moving_dir * Skill::acceleration[Skill::Teleport] * time;
		}break;
		case States::Hurt:{
			//bool inv=st.time_since_start>State::INV_HURT_FRAMES;
			//colinf.dTag = (DTag::ID)(DTag::Invincible*inv + Character::Stats::defensive_hitbox_type[st.Class][st.current]*!inv);
			if(st.time_since_start>State::INV_HURT_FRAMES){
				colinf.dTag = static_cast<DTag::ID>(Character::Stats::defensive_hitbox_type[st.Class][st.current]);
			}
			else colinf.dTag = DTag::Invincible;
				
			
		}//follows up
		case States::Stunned:
		{
			if(st.time_since_start > st.duration){
				st.update(States::Normal);
			};
		}break;
		case States::Wave_Shot:
		{

			/*
			normal_acceleration
			initial_tan_speed        remains constant
			initial_normal_speed     changes with time

			each frame normal_speed += normal_acceleration*frame_time
			if normal_speed>=initial_normal_speed then normal_speed = 2*initial_normal_speed - normal_speed

			*/
			sf::Vector2f& normal_direction = st.moving_dir;
			sf::Vector2f tangent_direction(normal(normal_direction));

			float normal_acceleration = Skill::acceleration[Skill::Wave_Shot] * -cos(st.time_since_start.asSeconds()*Skill::period_factor[Skill::Wave_Shot]);
			mov.velocity += normal_direction * normal_acceleration * time;

			//float& initial_normal_speed = mov.maxspeed;
			//float normal_speed = mov.velocity.x * st.moving_dir.x + mov.velocity.y * st.moving_dir.y;
			//float tangent_speed = abs(mov.velocity.x*tangent_direction.x + mov.velocity.y*tangent_direction.y);
			//if(normal_speed > initial_normal_speed){
			//	mov.velocity += normal_direction * (2 * (initial_normal_speed - normal_speed));
			//	normal_direction = -normal_direction;

			//}


		}break;

		case States::Telekinetic_Blade:
		{
			sf::Vector2f direction = normalized(mWorld.vec_Position[mWorld.vec_Team[i].caster].getPosition() - pos.getPosition());
			mov.velocity += Skill::acceleration[Skill::Telekinetic_Blade] * direction * time;

			

		}break;
		case States::Casting:
		{
			switch(st.current_skill){
				case Skill::Simple_Melee:
				case Skill::Mirror_Melee:
				case Skill::Slow_Big_Melee:
				{
					switch(st.skill_counter){
						case 0:
						{
							if(st.time_since_start >= Skill::buildup[st.current_skill]){
								CollisionInfo inf;
								inf = Skill::col_info[st.current_skill];
								inf.damage = Skill::damage[st.current_skill] * Character::Stats::p_attack[st.Class];
								float siz = Character::Stats::size[st.Class];

								mWorld.make_hit_box(sf::Vector2f(SIGN(st.facing_dir.x), SIGN(st.facing_dir.y)) * siz, siz*sf::Vector2f(3, 3), i, std::move(inf), Skill::hb_duration[st.current_skill]);
								st.skill_counter++;
								st.skill_uses[st.current_skill]++;
							}
						}break;
					}
					if(st.time_since_start >= Skill::duration[st.current_skill]){
						st.update(States::Normal);
					}
				}break;
				case Skill::Simple_Shot:
				case Skill::Bounce_Shot:
				case Skill::Tennis_Ball:
				case Skill::Simple_Shot_Zombie:
				{
					switch(st.skill_counter){
						case 0:
						{
							if(st.time_since_start >= Skill::buildup[st.current_skill]){
								CollisionInfo inf;
								inf = Skill::col_info[st.current_skill];
								inf.damage = Skill::damage[st.current_skill] * Character::Stats::m_attack[st.Class];
								float siz = 5.f;

								mWorld.make_bullet(pos.getPosition(), st.facing_dir, mov.velocity, Skill::bullet_speed[st.current_skill], std::move(inf), Skill::bullet_duration[st.current_skill], i,Skill::bullet_radius[st.current_skill],Skill::bullet_color[st.current_skill],Skill::owner_type[st.current_skill]);
								st.skill_counter++;
								st.skill_uses[st.current_skill]++;
							}
						}break;


					}
					if(st.time_since_start >= Skill::duration[st.current_skill]){
						st.update(States::Normal);
					}
				}break;

				case Skill::Wave_Shot:
				{
					switch(st.skill_counter){
						case 0:
						{
							if(st.time_since_start >= Skill::buildup[st.current_skill]){
								CollisionInfo inf;// inf2;
								inf = Skill::col_info[st.current_skill];
								inf.damage = Skill::damage[st.current_skill] * Character::Stats::m_attack[st.Class];
								float siz = 5.f;

								mWorld.make_wave_bullet(pos.getPosition(), st.facing_dir, 1, Skill::bullet_speed[st.current_skill], Skill::acceleration[st.current_skill], std::move(inf), Skill::bullet_duration[st.current_skill], i);
								//mWorld.make_wave_bullet(pos.getPosition(), from_angle(get_angle(st.facing_dir) + 10.f / 180.f*M_PI), 1, Skill::bullet_speed[st.current_skill], Skill::acceleration[st.current_skill], std::move(inf), Skill::bullet_duration[st.current_skill], i);
								//mWorld.make_wave_bullet(pos.getPosition(), from_angle(get_angle(st.facing_dir) - 10.f / 180.f*M_PI), 1, Skill::bullet_speed[st.current_skill], Skill::acceleration[st.current_skill], std::move(inf), Skill::bullet_duration[st.current_skill], i);
								mWorld.make_wave_bullet(pos.getPosition(), st.facing_dir, -1, Skill::bullet_speed[st.current_skill], Skill::acceleration[st.current_skill], std::move(inf), Skill::bullet_duration[st.current_skill], i);
								//mWorld.make_wave_bullet(pos.getPosition(), from_angle(get_angle(st.facing_dir) + 10.f / 180.f*M_PI), -1, Skill::bullet_speed[st.current_skill], Skill::acceleration[st.current_skill], std::move(inf), Skill::bullet_duration[st.current_skill], i);
								//mWorld.make_wave_bullet(pos.getPosition(), from_angle(get_angle(st.facing_dir) - 10.f / 180.f*M_PI), -1, Skill::bullet_speed[st.current_skill], Skill::acceleration[st.current_skill], std::move(inf), Skill::bullet_duration[st.current_skill], i);

								st.skill_counter++;

								st.skill_uses[st.current_skill]++;
							}
						}break;


					}
					if(st.time_since_start >= Skill::duration[st.current_skill]){
						st.update(States::Normal);
					}
				}break;
				case Skill::Teleport:
				{
					switch(st.skill_counter){
						case 0:
						{
							if(st.time_since_start >= Skill::buildup[st.current_skill]){
								mWorld.make_teleport_scope(pos.getPosition(), Skill::max_speed[Skill::Teleport], sf::Vector2f(1, 1)*Character::Stats::size[st.Class], i);
								st.skill_counter++;
							}
						}break;
						case 1:
						{
							if(!st.pressed[st.current_skill]){
								st.update(States::Normal);

								st.skill_uses[st.current_skill]++;
							}
						}break;
					}
				}break;
				case Skill::Ram:
				{
					switch(st.skill_counter){
						case 0:
						{
							if(st.time_since_start >= Skill::buildup[st.current_skill]){
								CollisionInfo inf;
								inf = Skill::col_info[st.current_skill];
								inf.damage = Skill::damage[st.current_skill] * Character::Stats::p_attack[st.Class];
								float siz = Character::Stats::size[st.Class];

								mWorld.make_hit_box(sf::Vector2f(SIGN(st.facing_dir.x), SIGN(st.facing_dir.y)) * siz, siz*sf::Vector2f(3, 3), i, std::move(inf), dt_max_fps * 35.f);
								mov.maxspeed = Skill::max_speed[st.current_skill];

								colinf.on_wall = CollisionInfo::bounce;
								colinf.bounce_factor=0.75f;
								st.skill_counter++;

								st.skill_uses[st.current_skill]++;
							}
						}break;
						case 1:
						{
							mov.velocity += st.facing_dir * Skill::acceleration[st.current_skill] * time;
							if(st.time_since_start >= Skill::duration[st.current_skill]){
								st.update(States::Normal);
								colinf.on_wall = CollisionInfo::stop;

							}
							if(st.wall){
								st.duration = sf::seconds(0.5f);
								mov.velocity /= 2.f;
								colinf.dTag = DTag::Damageable;
								st.time_since_start = sf::Time::Zero;
								mWorld.remove_children(i, Relacion::delete_on_hurt);
								st.skill_counter++;
							}
						}break;
						case 2:
						{
							if(st.time_since_start > st.duration){
								st.update(States::Normal);
								colinf.on_wall = CollisionInfo::stop;
							}
						}

					}
				}break;
				case Skill::Telekinetic_Blade:
				{
					switch(st.skill_counter){
						case 0:
						{
							if(st.time_since_start >= Skill::buildup[st.current_skill]){
								CollisionInfo inf;
								inf = Skill::col_info[st.current_skill];
								inf.damage = Skill::damage[st.current_skill] * Character::Stats::m_attack[st.Class];
								
								mWorld.make_special_bullet(pos.getPosition(), st.facing_dir, mov.velocity, Skill::bullet_speed[st.current_skill], std::move(inf), Skill::bullet_duration[st.current_skill], i,Skill::bullet_radius[st.current_skill],Skill::bullet_color[st.current_skill],States::Telekinetic_Blade);
								st.skill_counter++;

								st.skill_uses[st.current_skill]++;
							}
						}break;


					}
					if(st.time_since_start >= Skill::duration[st.current_skill]){
						st.update(States::Normal);
					}
				}break;
				case Skill::Dash:{
					switch(st.skill_counter){
						case 0:{
							if(st.time_since_start>=Skill::buildup[st.current_skill])
								mov.maxspeed = Skill::max_speed[st.current_skill];
							mov.velocity += st.facing_dir * Skill::normal_speed[st.current_skill];
							st.skill_counter++;
							mov.friction=Skill::friction[st.current_skill];
							st.skill_uses[st.current_skill]++;
						}break;
						case 1:{
							if(st.time_since_start>=Skill::duration[st.current_skill]){
								st.update(States::Normal);
							}
						}
					}
				}break;
				case Skill::Dash_Strike:{
					switch(st.skill_counter){
						case 0:{
							if(st.time_since_start>=Skill::buildup[st.current_skill])
								mov.maxspeed = Skill::max_speed[st.current_skill];
							mov.velocity += st.facing_dir * Skill::normal_speed[st.current_skill];
							CollisionInfo inf;
							inf = Skill::col_info[st.current_skill];
							inf.damage = Skill::damage[st.current_skill] * Character::Stats::p_attack[st.Class];
							float siz = Character::Stats::size[st.Class];

							mWorld.make_hit_box(sf::Vector2f(SIGN(st.facing_dir.x), SIGN(st.facing_dir.y)) * siz, siz*sf::Vector2f(3, 3), i, std::move(inf), dt_max_fps*10.f);

							st.skill_counter++;

							st.skill_uses[st.current_skill]++;
						}break;
						case 1:{
							if(st.time_since_start>=Skill::duration[st.current_skill]){
								st.update(States::Normal);
							}
						}
					}
				}break;
				case Skill::Stun_Roar:{
					switch(st.skill_counter){
						case 0:
						{
							if(st.time_since_start >= Skill::buildup[st.current_skill]){
								CollisionInfo inf;
								inf = Skill::col_info[st.current_skill];
								inf.damage = Skill::damage[st.current_skill] * Character::Stats::p_attack[st.Class];
								float siz = Character::Stats::size[st.Class];

								mWorld.make_hit_box(sf::Vector2f(0,0) * siz, sf::Vector2f(300, 300), i, std::move(inf), Skill::hb_duration[st.current_skill], BoxType::Circle);
								st.skill_counter++;

								st.skill_uses[st.current_skill]++;
							}
						}break;
					}
					if(st.time_since_start >= Skill::duration[st.current_skill]){
						st.update(States::Normal);
					}
				}break;
			}
		}break;
		case States::Spawner:
		{
			if(st.time_since_start >= st.duration){
				auto& pos = mWorld.vec_Position[i];
				mWorld.make_zombie(pos.getPosition());
				auto aux = st.duration;
				st.update(States::Spawner);
				st.duration = aux;
			}
		}break;
		case States::HitBox:
		{
			if(st.duration >= sf::Time::Zero)
				if(st.time_since_start > st.duration){
					mWorld.remove_entity(i);
				}

		}break;
	}
	st.wall = false;
	ITERATE_END
}


