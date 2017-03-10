#pragma once
#include "SFML\System\Time.hpp"
#include "CollisionInfo.h"
namespace Skill {
	enum ID {
		None,
		Simple_Melee,//funca
		Mirror_Melee,//funca
		Slow_Big_Melee,//
		Dash,		//funca
		Dash_Strike,//funca
		Simple_Shot,//funca
		Bounce_Shot,//funca
		Teleport,	//funca
		Ram,		//funca
		Wave_Shot,	//funca
		/*Wave_Shot2,
		Wave_Shot3,*/
		Stun_Roar,//NEXT ONE
		Telekinetic_Blades,
		Tennis_Ball,
		Quake,
		size
	};
	extern sf::Time buildup[size];
	extern sf::Time duration[size];
	extern sf::Time hb_duration[size];
	extern int damage[size];
	extern CollisionInfo col_info[size];
	extern float bullet_speed[size];
	extern float max_speed[size];
	extern float acceleration[size];
	extern sf::Time bullet_duration[size];
	extern float friction[size];
	extern float normal_speed[size];
	extern void init();
}

