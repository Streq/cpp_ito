#pragma once
#include "SFML\System\Time.hpp"
#include "CollisionInfo.h"
namespace Skill {
	enum ID {
		None,
		Simple_Melee,
		Mirror_Melee,//Tennist
		Slow_Big_Melee,//Giant
		Dash,//Time_Traveler
		Dash_Strike,//Swordsman
		Simple_Shot,
		Bounce_Shot,
		Teleport,//Time_Traveler
		Ram,//Minotaur
		Wave_Shot,//Time_Traveler
		Stun_Roar,//Minotaur
		size
	};
	extern sf::Time buildup[size];
	extern sf::Time duration[size];
	extern int damage[size];
	extern CollisionInfo col_info[size];
	extern void init();
}

