#pragma once
#include "SFML\System\Time.hpp"
#include "CollisionInfo.h"
namespace Skill {
	enum ID {
		None,
		Simple_Melee,//funca
		Mirror_Melee,//
		Slow_Big_Melee,//
		Dash,//
		Dash_Strike,//
		Simple_Shot,//funca
		Bounce_Shot,//funca
		Teleport,//funca
		Ram,//
		Wave_Shot,//
		Stun_Roar,//
		size
	};
	extern sf::Time buildup[size];
	extern sf::Time duration[size];
	extern int damage[size];
	extern CollisionInfo col_info[size];
	extern float bullet_speed[size];
	extern float max_speed[size];
	extern float acceleration[size];
	extern float friction[size];
	extern void init();
}

