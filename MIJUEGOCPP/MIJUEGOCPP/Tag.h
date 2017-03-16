#pragma once
#define DO_X_FOR_TAG_LIST \
X(Building)\
X(Bullet)\
X(Char_Body)\
X(Scope)\
X(Hit_Box)


namespace Physics_Tag {		//Only collides with itself
	enum ID {
		Static,				//Collides with Dynamic_Solid and Dynamic_Non_Solid
		Dynamic_Solid,		//Collides with Static and Dynamic_Solid
		Dynamic_Non_Solid,	//Collides with Static
		Intangible,			//Ignores everything
		size
	};
}

namespace Defensive_Tag {	//Only collides with Offensive_Tag
	enum ID {
		Damageable,			//Can be hit
		Invincible,			//Can be hit but remains unaffected
		NonStunneable,		//Can be hit but not stunned (that includes hurt stun)
		Reflect,			//Hitting it triggers a reflection on projectiles
		Intangible,			//Ignores completely
		size
	};
}

namespace Offensive_Tag {	//Only collides with Defensive_Tag
	enum ID {
		Damage,				//changes state to Hurt
		Stun,				//changes state to Stunned
		None,				//Ignores completely
		size
	};
}


namespace Tag {//Esto BUELA
	enum ID {
		Wall,				//Interacts with Solid Entities
		Character_Entity,	//Interacts with Buildings
		Projectile,
		Hit_Box,			//Interacts with other Hit_Boxes
		Scope,
		size
	};

}

