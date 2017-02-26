#include "Skill.h"

sf::Time Skill::buildup[Skill::size];
sf::Time Skill::duration[Skill::size];
int Skill::damage[Skill::size];
CollisionInfo Skill::col_info[size];
float Skill::bullet_speed[size];

void Skill::init() {
	buildup[Simple_Melee] = sf::seconds(0);
	duration[Simple_Melee] = dt_max_fps * 15.f;
	damage[Simple_Melee] = 10.f;
	col_info[Simple_Melee].tag = Tag::Hit_Box;
	col_info[Simple_Melee].type = HitBoxType::Hit;
	col_info[Simple_Melee].stun_time = sf::seconds(0.5);
	col_info[Simple_Melee].knockback = 1000.f;

	buildup[Simple_Shot] = sf::seconds(0);
	duration[Simple_Shot] = dt_max_fps * 2.f;
	bullet_speed[Simple_Shot] = 600.f;
	damage[Simple_Shot] = 10.f;
	col_info[Simple_Shot].tag = Tag::Projectile;
	col_info[Simple_Shot].type = HitBoxType::Hit;
	col_info[Simple_Shot].stun_time = sf::seconds(0.5);
	col_info[Simple_Shot].knockback = 600.f;
	col_info[Simple_Shot].on_wall = CollisionInfo::remove;


	buildup[Bounce_Shot] = sf::seconds(0);
	duration[Bounce_Shot] = dt_max_fps * 5.f;
	bullet_speed[Bounce_Shot] = 500.f;
	damage[Bounce_Shot] = 10.f;
	col_info[Bounce_Shot].tag = Tag::Projectile;
	col_info[Bounce_Shot].type = HitBoxType::Hit;
	col_info[Bounce_Shot].stun_time = sf::seconds(0.5);
	col_info[Bounce_Shot].knockback = 600.f;
	col_info[Bounce_Shot].on_wall = CollisionInfo::bounce;
}