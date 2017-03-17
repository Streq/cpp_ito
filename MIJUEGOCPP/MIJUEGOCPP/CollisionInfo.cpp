#include "CollisionInfo.h"
CollisionInfo::pMatrix CollisionInfo::pmatrix = {};
CollisionInfo::odMatrix CollisionInfo::odmatrix = {};



//BUELA

void CollisionInfo::pair_tags(Physics_Tag::ID a, Physics_Tag::ID b, bool val){
	pmatrix[a][b] = pmatrix[b][a] = val;
}
void CollisionInfo::pair_tags(Defensive_Tag::ID a, Offensive_Tag::ID b, bool val){
	odmatrix[a][b] = val;
}


void CollisionInfo::init_matrix()
{
	//BUELA
	//BUELA

	pair_tags(Physics_Tag::Static, Physics_Tag::Dynamic_Solid);
	pair_tags(Physics_Tag::Static, Physics_Tag::Dynamic_Non_Solid);
	pair_tags(Physics_Tag::Dynamic_Solid, Physics_Tag::Dynamic_Solid);

	pair_tags(Defensive_Tag::Damageable, Offensive_Tag::Damage);
	pair_tags(Defensive_Tag::Damageable, Offensive_Tag::Stun);
	pair_tags(Defensive_Tag::NonStunneable, Offensive_Tag::Damage);
	pair_tags(Defensive_Tag::NonStunneable, Offensive_Tag::Stun);
	pair_tags(Defensive_Tag::Invincible, Offensive_Tag::Damage);
	pair_tags(Defensive_Tag::Invincible, Offensive_Tag::Stun);
	pair_tags(Defensive_Tag::Reflect,	 Offensive_Tag::Damage);
	pair_tags(Defensive_Tag::Reflect,	 Offensive_Tag::Stun);




	
}
