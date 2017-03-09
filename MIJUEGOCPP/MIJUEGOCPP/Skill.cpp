#include "Skill.h"
#include "vec_magn.h"
sf::Time Skill::buildup[Skill::size];
sf::Time Skill::duration[Skill::size];
sf::Time Skill::hb_duration[Skill::size];
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
	hb_duration[Simple_Melee] = dt_max_fps * 10.f;
	damage[Simple_Melee] = 10.f;
	col_info[Simple_Melee].tag = Tag::Hit_Box;
	col_info[Simple_Melee].dTag = DTag::Intangible;
	col_info[Simple_Melee].oTag = OTag::Damage;
	col_info[Simple_Melee].pTag = PTag::Intangible;
	col_info[Simple_Melee].delete_on_hit = false;
	col_info[Simple_Melee].stun_time = sf::seconds(0.5);
	col_info[Simple_Melee].knockback = 700.f;
	col_info[Simple_Melee].momentum_knockback = 0.f;

	buildup[Mirror_Melee] = sf::seconds(0);
	duration[Mirror_Melee] = dt_max_fps * 10.f;
	hb_duration[Mirror_Melee] = dt_max_fps * 8.f;
	damage[Mirror_Melee] = 10.f;
	col_info[Mirror_Melee].tag = Tag::Hit_Box;
	col_info[Mirror_Melee].dTag = DTag::Reflect;
	col_info[Mirror_Melee].oTag = OTag::Damage;
	col_info[Mirror_Melee].pTag = PTag::Intangible;
	col_info[Mirror_Melee].delete_on_hit = false;
	col_info[Mirror_Melee].stun_time = sf::seconds(0.3);
	col_info[Mirror_Melee].knockback = 700.f;
	col_info[Mirror_Melee].momentum_knockback = 0.f;



	buildup[Simple_Shot] = sf::seconds(0);
	duration[Simple_Shot] = dt_max_fps * 2.f;
	bullet_duration[Simple_Shot] = sf::seconds(2.f);
	bullet_speed[Simple_Shot] = 600.f;
	damage[Simple_Shot] = 10.f;
	col_info[Simple_Shot].tag = Tag::Projectile;
	col_info[Simple_Shot].dTag = DTag::Intangible;
	col_info[Simple_Shot].oTag = OTag::Damage;
	col_info[Simple_Shot].pTag = PTag::Dynamic_Non_Solid;
	col_info[Simple_Shot].delete_on_hit = true;
	col_info[Simple_Shot].stun_time = sf::seconds(0.5);
	col_info[Simple_Shot].momentum_knockback = 600.f;
	col_info[Simple_Shot].knockback = 0.f;
	col_info[Simple_Shot].on_wall = CollisionInfo::remove;
	col_info[Simple_Shot].reflectable = true;


	buildup[Bounce_Shot] = sf::seconds(0);
	duration[Bounce_Shot] = dt_max_fps * 5.f;
	bullet_duration[Bounce_Shot] = sf::seconds(5.f);
	bullet_speed[Bounce_Shot] = 500.f;
	damage[Bounce_Shot] = 10.f;
	col_info[Bounce_Shot].tag = Tag::Projectile;
	col_info[Bounce_Shot].dTag = DTag::Intangible;
	col_info[Bounce_Shot].oTag = OTag::Damage;
	col_info[Bounce_Shot].pTag = PTag::Dynamic_Non_Solid;
	col_info[Bounce_Shot].bounce_factor = 1.f;
	col_info[Bounce_Shot].delete_on_hit = true;
	col_info[Bounce_Shot].stun_time = sf::seconds(0.5);
	col_info[Bounce_Shot].knockback = 0.f;
	col_info[Bounce_Shot].momentum_knockback = 600.f;
	col_info[Bounce_Shot].on_wall = CollisionInfo::bounce;
	col_info[Bounce_Shot].reflectable = true;


	buildup[Teleport] = sf::seconds(0);
	max_speed[Teleport] = 2000.f;
	acceleration[Teleport] = 120000.f;
	friction[Teleport] = 60000.f;
	col_info[Teleport].tag = Tag::Scope;
	col_info[Teleport].dTag = DTag::Intangible;
	col_info[Teleport].oTag = OTag::None;
	col_info[Teleport].pTag = PTag::Dynamic_Non_Solid;
	col_info[Teleport].on_wall = CollisionInfo::pass_through;

	buildup[Ram] = sf::seconds(0);
	duration[Ram] = dt_max_fps * 40.f;
	damage[Ram] = 10.f;
	acceleration[Ram] = 6000.f;
	max_speed[Ram] = 800.f;
	col_info[Ram].tag = Tag::Hit_Box;
	col_info[Ram].dTag = DTag::Intangible;
	col_info[Ram].oTag = OTag::Damage;
	col_info[Ram].pTag = PTag::Intangible;
	col_info[Ram].delete_on_hit = false;
	col_info[Ram].stun_time = sf::seconds(0.5);
	col_info[Ram].knockback = 1000.f;
	col_info[Ram].momentum_knockback = 0.f;



	buildup[Wave_Shot] = sf::seconds(0);
	duration[Wave_Shot] = dt_max_fps * 2.f;
	bullet_speed[Wave_Shot] = 400.f;
	acceleration[Wave_Shot] = 6000.f;
	normal_speed[Wave_Shot] = 800.f;
	damage[Wave_Shot] = 10.f;
	col_info[Wave_Shot].tag = Tag::Projectile;
	col_info[Wave_Shot].dTag = DTag::Intangible;
	col_info[Wave_Shot].oTag = OTag::Damage;
	col_info[Wave_Shot].pTag = PTag::Intangible;
	col_info[Wave_Shot].delete_on_hit = true;
	col_info[Wave_Shot].stun_time = sf::seconds(0.5);
	col_info[Wave_Shot].momentum_knockback = 600.f;
	col_info[Wave_Shot].knockback = 0.f;
	col_info[Wave_Shot].on_wall = CollisionInfo::pass_through;
	col_info[Wave_Shot].reflectable = true;
	bullet_duration[Wave_Shot] = sf::seconds(10.f);
	
	buildup[Dash]=sf::Time::Zero;
	duration[Dash]=sf::seconds(0.5f);
	normal_speed[Dash]=800.f;
	max_speed[Dash]=850.f;


	buildup[Dash_Strike]=sf::Time::Zero;
	duration[Dash_Strike]=sf::seconds(1.f/3.f);
	normal_speed[Dash_Strike]=800.f;
	max_speed[Dash_Strike]=850.f;
	damage[Dash_Strike] = 20.f;
	col_info[Dash_Strike].tag = Tag::Hit_Box;
	col_info[Dash_Strike].dTag = DTag::Intangible;
	col_info[Dash_Strike].oTag = OTag::Damage;
	col_info[Dash_Strike].pTag = PTag::Intangible;
	col_info[Dash_Strike].delete_on_hit = false;
	col_info[Dash_Strike].stun_time = sf::seconds(0.5);
	col_info[Dash_Strike].knockback = 800.f;
	col_info[Dash_Strike].momentum_knockback = 0.f;



	/*
	buildup[Wave_Shot2] = sf::seconds(0);
	duration[Wave_Shot2] = dt_max_fps * 2.f;
	bullet_speed[Wave_Shot2] = 400.f;
	acceleration[Wave_Shot2] = 6000.f;
	normal_speed[Wave_Shot2] = 800.f;
	damage[Wave_Shot2] = 10.f;
	col_info[Wave_Shot2].tag = Tag::Projectile;
	col_info[Wave_Shot2].type = HitBoxType::Hit;
	col_info[Wave_Shot2].stun_time = sf::seconds(0.5);
	col_info[Wave_Shot2].knockback = 600.f;
	col_info[Wave_Shot2].on_wall = CollisionInfo::pass_through;
	bullet_duration[Wave_Shot2] = sf::seconds(10.f);

	buildup[Wave_Shot3] = sf::seconds(0);
	duration[Wave_Shot3] = dt_max_fps * 2.f;
	bullet_speed[Wave_Shot3] = 400.f;
	acceleration[Wave_Shot3] = 6000.f;
	normal_speed[Wave_Shot3] = 800.f;
	damage[Wave_Shot3] = 10.f;
	col_info[Wave_Shot3].tag = Tag::Projectile;
	col_info[Wave_Shot3].type = HitBoxType::Hit;
	col_info[Wave_Shot3].stun_time = sf::seconds(0.5);
	col_info[Wave_Shot3].knockback = 600.f;
	col_info[Wave_Shot3].on_wall = CollisionInfo::pass_through;
	bullet_duration[Wave_Shot3] = sf::seconds(10.f);
	*/
}