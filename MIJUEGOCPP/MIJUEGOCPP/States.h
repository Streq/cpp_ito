#pragma once
#include "Color.h"
#include <SFML\System\Time.hpp>
#include "defines.h"

namespace States {
	enum ID {
		None = -1,
		//Player
		Normal,		//can move and attack
		Casting,	//can't move or attack
		Silenced,	//can't attack
		Stunned,	//can't move or attack
		Hurt_Invincible,
		Hurt,		//can't move or attack
		//Non player_states
		Wave_Shot,
		Teleport_Scope,
		Telekinetic_Blade,
		Spawner,
		HitBox,
		//Data
		size,
		player_states=Hurt+1,

	};
	
};
