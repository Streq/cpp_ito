#pragma once
#include "Component.h"
#include "Tag.h"
#include "Handle.h"
namespace HitBoxType {
	enum Character {
		Damageable,//can be hit
		Invincible,//hit but unaffected
		Intangible,//can't be hit
		
	};
	enum HitBox {
		Hit,//damages
		Reflect,//reflects bullets
		Absorb,//Absorbs bullets
		
	};

}

namespace Effect {
	//Effects of hitbox_type
	enum ID{
		Hurt,//Gives mercy invulnerability
		Stun,//Only stuns
		size
	};
}


struct CollisionInfo :public Component {

public:

	

	
	Physics_Tag::ID pTag;
	Offensive_Tag::ID oTag;
	Defensive_Tag::ID dTag;


	//int max_hits=0;
	//int hits=0;
	bool delete_on_hit=false;

	bool reflectable = false;
	bool change_team_on_wall = false;
	Handle last_reflection = MAX_ENTITIES;


	Effect::ID effect;
	
	//Damage(hitbox_type)
	float damage;

	float bounce_factor;
	//Knockback of the hitbox_type in the direction of the shortest line between the 2 entities
	float knockback;
	//Knockback of the hitbox_type in the direction of the movement of the hitbox entity
	float momentum_knockback;

	//Stun time of hitbox_type
	sf::Time stun_time;

	//Action to perform on wall collision
	enum On_Wall{
		stop,
		bounce,
		remove,
		pass_through,
	}on_wall;
	
	




public:
	
	typedef bool pMatrix[Physics_Tag::size][Physics_Tag::size];
	typedef bool odMatrix[Defensive_Tag::size][Offensive_Tag::size];
	
	static const pMatrix& get_pmatrix() { return pmatrix; }
	static const odMatrix& get_odmatrix() { return odmatrix; }

	static void init_matrix();
	static inline bool check_pair(Physics_Tag::ID a, Physics_Tag::ID b) { return pmatrix[a][b]; }
	static inline bool check_pair(Defensive_Tag::ID a, Offensive_Tag::ID b) { return odmatrix[a][b]; }


private:

	static pMatrix pmatrix;
	static odMatrix odmatrix;

	static void pair_tags(Physics_Tag::ID a, Physics_Tag::ID b, bool val = true);
	static void pair_tags(Defensive_Tag::ID a, Offensive_Tag::ID b, bool val = true);


};
