#include "Skill.h"

sf::Time Skill::buildup[Skill::size];
sf::Time Skill::duration[Skill::size];
int Skill::damage[Skill::size];
CollisionInfo Skill::col_info[size];


void Skill::init() {
	buildup[Simple_Melee] = sf::seconds(0);
	duration[Simple_Melee] = sf::seconds(0.13);
	damage[Simple_Melee] = 10.f;
	col_info[Simple_Melee].tag = Tag::Hit_Box;
	col_info[Simple_Melee].type = HitBoxType::Hit;
	col_info[Simple_Melee].stun_time = sf::seconds(0.5);
	col_info[Simple_Melee].knockback = 400.f;
}