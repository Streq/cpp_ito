#pragma once
#include "States.h"
#include"Component.h"
#include "Character.h"
#include "Color.h"
#include "CollisionInfo.h"
#include "Skill.h"
namespace Character{
	struct Stats {
		//Tamaño del sprite
		static float size[ID::size];
		
		//Salud máxima
		static float health[ID::size];
		
		//Ataque físico
		static float p_attack[ID::size];
		
		//Ataque mágico
		static float m_attack[ID::size];
		
		//Para calcular velocidad de movimiento e impulso en general
		static float mov_speed[ID::size];
		
		static float mov_acceleration[ID::size];

		//Para calcular fricción e impulso en golpe
		static float mass[ID::size];
		
		//todavia no se si va tar
		static float mana[ID::size];
		
		static float melee_hitbox_size_factor[ID::size];
		static sf::Color color[ID::size][States::size];
		static int hitbox_type[ID::size][States::size];
		//Habilidades
		static Skill::ID skill[ID::size][skill_num];

		/*
		static sf::Time melee_hitbox_start_frame[ID::size];//no sirve
		static sf::Time melee_hitbox_duration[ID::size];//no sirve
		static sf::Time melee_hitbox_stun_duration[ID::size];//no sirvee

		static bool melee_hitbox_reflects[ID::size];//no sirve
		static bool melee_hitbox_absorbs[ID::size];//no sirve
		static int melee_hitbox_damage[ID::size];//no sirve
		static float melee_hitbox_knockback[ID::size];//no sirve
		static float melee_offset[ID::size];//no sirve
	


		//Shoot info
		static float bullet_size[ID::size];//no sirve
		static float bullet_speed[ID::size];//no sirve

		static float max_speed[ID::size][States::size];//no sirve
		static float friction[ID::size][States::size];//no sirve
		static float acceleration[ID::size][States::size];//no sirve
		static float impulse[ID::size][States::size];//no sirve
		static sf::Time duration[ID::size][States::size];//no sirve
		*/
		
		static void init();
	};
}

