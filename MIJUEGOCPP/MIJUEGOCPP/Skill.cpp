#include "Skill.h"
#include "vec_magn.h"
sf::Time Skill::buildup[Skill::size];
sf::Time Skill::duration[Skill::size];
sf::Time Skill::bullet_duration[size];
int Skill::damage[Skill::size];
CollisionInfo Skill::col_info[size];
float Skill::bullet_speed[size];
float Skill::acceleration[size];
float Skill::max_speed[size];
float Skill::friction[size];
float Skill::normal_speed[size];

void Skill::init() {
	buildup[Simple_Melee] = sf::seconds(0);
	duration[Simple_Melee] = dt_max_fps * 15.f;
	damage[Simple_Melee] = 10.f;
	col_info[Simple_Melee].tag = Tag::Hit_Box;
	col_info[Simple_Melee].type = HitBoxType::Hit;
	col_info[Simple_Melee].stun_time = sf::seconds(0.5);
	col_info[Simple_Melee].knockback = 700.f;

	buildup[Simple_Shot] = sf::seconds(0);
	duration[Simple_Shot] = dt_max_fps * 2.f;
	bullet_duration[Simple_Shot] = sf::seconds(2.f);
	bullet_speed[Simple_Shot] = 600.f;
	damage[Simple_Shot] = 10.f;
	col_info[Simple_Shot].tag = Tag::Projectile;
	col_info[Simple_Shot].type = HitBoxType::Hit;
	col_info[Simple_Shot].stun_time = sf::seconds(0.5);
	col_info[Simple_Shot].knockback = 600.f;
	col_info[Simple_Shot].on_wall = CollisionInfo::remove;


	buildup[Bounce_Shot] = sf::seconds(0);
	duration[Bounce_Shot] = dt_max_fps * 5.f;
	bullet_duration[Bounce_Shot] = sf::seconds(2.f);
	bullet_speed[Bounce_Shot] = 500.f;
	damage[Bounce_Shot] = 10.f;
	col_info[Bounce_Shot].tag = Tag::Projectile;
	col_info[Bounce_Shot].type = HitBoxType::Hit;
	col_info[Bounce_Shot].stun_time = sf::seconds(0.5);
	col_info[Bounce_Shot].knockback = 600.f;
	col_info[Bounce_Shot].on_wall = CollisionInfo::bounce;


	buildup[Teleport] = sf::seconds(0);
	max_speed[Teleport] = 2000.f;
	acceleration[Teleport] = 120000.f;
	friction[Teleport] = 60000.f;
	col_info[Teleport].tag = Tag::Scope;


	buildup[Ram] = sf::seconds(0);
	duration[Ram] = dt_max_fps * 40.f;
	damage[Ram] = 10.f;
	acceleration[Ram] = 6000.f;
	max_speed[Ram] = 800.f;
	col_info[Ram].tag = Tag::Hit_Box;
	col_info[Ram].type = HitBoxType::Hit;
	col_info[Ram].stun_time = sf::seconds(0.5);
	col_info[Ram].knockback = 1000.f;



	buildup[Wave_Shot] = sf::seconds(0);
	duration[Wave_Shot] = dt_max_fps * 2.f;
	bullet_speed[Wave_Shot] = 300.f;
	acceleration[Wave_Shot] = 900.f;
	normal_speed[Wave_Shot] = 300.f;
	damage[Wave_Shot] = 10.f;
	col_info[Wave_Shot].tag = Tag::Projectile;
	col_info[Wave_Shot].type = HitBoxType::Hit;
	col_info[Wave_Shot].stun_time = sf::seconds(0.5);
	col_info[Wave_Shot].knockback = 600.f;
	col_info[Wave_Shot].on_wall = CollisionInfo::pass_through;
	bullet_duration[Wave_Shot] = sf::seconds(10.f);
}