#include "Skill.h"
#include "vec_magn.h"
#include "Color.h"

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
sf::Color Skill::bullet_color[size];
float Skill::bullet_radius[size];
float Skill::period_factor[size];

void Skill::init() {

	#define inf col_info[sk]



#define sk Simple_Melee
	buildup[sk] = sf::seconds(0);
	duration[sk] = dt_max_fps * 15.f;
	hb_duration[sk] = dt_max_fps * 10.f;
	damage[sk] = 10.f;
	inf.tag = Tag::Hit_Box;
	inf.dTag = DTag::Intangible;
	inf.oTag = OTag::Damage;
	inf.pTag = PTag::Intangible;
	inf.delete_on_hit = false;
	inf.stun_time = sf::seconds(0.5);
	inf.knockback = 700.f;
	inf.momentum_knockback = 0.f;
#undef sk
	




#define sk Mirror_Melee
	buildup[sk] = sf::seconds(0);
	duration[sk] = dt_max_fps * 10.f;
	hb_duration[sk] = dt_max_fps * 8.f;
	damage[sk] = 10.f;
	inf.tag = Tag::Hit_Box;
	inf.dTag = DTag::Reflect;
	inf.oTag = OTag::Damage;
	inf.pTag = PTag::Intangible;
	inf.delete_on_hit = false;
	inf.stun_time = sf::seconds(0.3);
	inf.knockback = 700.f;
	inf.momentum_knockback = 0.f;
#undef sk
	




#define sk Simple_Shot
	buildup[sk] = sf::seconds(0);
	duration[sk] = dt_max_fps * 2.f;
	bullet_duration[sk] = sf::seconds(2.f);
	bullet_speed[sk] = 600.f;
	bullet_color[sk] = Color::Red;
	bullet_radius[sk] = 2.5;
	damage[sk] = 10.f;
	inf.tag = Tag::Projectile;
	inf.dTag = DTag::Intangible;
	inf.oTag = OTag::Damage;
	inf.pTag = PTag::Dynamic_Non_Solid;
	inf.delete_on_hit = true;
	inf.stun_time = sf::seconds(0.5);
	inf.momentum_knockback = 600.f;
	inf.knockback = 0.f;
	inf.on_wall = CollisionInfo::remove;
	inf.reflectable = true;
	inf.change_team_on_wall = false;
#undef sk
	



	#define sk Bounce_Shot
	buildup[sk] = sf::seconds(0);
	duration[sk] = dt_max_fps * 5.f;
	bullet_duration[sk] = sf::seconds(5.f);
	bullet_speed[sk] = 400.f;
	bullet_color[sk] = Color::Red;
	bullet_radius[sk] = 2.5;
	damage[sk] = 10.f;
	inf.tag = Tag::Projectile;
	inf.dTag = DTag::Intangible;
	inf.oTag = OTag::Damage;
	inf.pTag = PTag::Dynamic_Non_Solid;
	inf.bounce_factor = 1.f;
	inf.delete_on_hit = true;
	inf.stun_time = sf::seconds(0.5);
	inf.knockback = 0.f;
	inf.momentum_knockback = 600.f;
	inf.on_wall = CollisionInfo::bounce;
	inf.reflectable = true;
	inf.change_team_on_wall = false;
	#undef sk





#define sk Teleport
	buildup[sk] = sf::seconds(0);
	max_speed[sk] = 2000.f;
	acceleration[sk] = 120000.f;
	friction[sk] = 60000.f;
	inf.tag = Tag::Scope;
	inf.dTag = DTag::Intangible;
	inf.oTag = OTag::None;
	inf.pTag = PTag::Dynamic_Non_Solid;
	inf.on_wall = CollisionInfo::pass_through;
	#undef sk
	




#define sk Ram
	buildup[sk] = sf::seconds(0);
	duration[sk] = dt_max_fps * 40.f;
	damage[sk] = 10.f;
	acceleration[sk] = 6000.f;
	max_speed[sk] = 800.f;
	inf.tag = Tag::Hit_Box;
	inf.dTag = DTag::Intangible;
	inf.oTag = OTag::Damage;
	inf.pTag = PTag::Intangible;
	inf.delete_on_hit = false;
	inf.stun_time = sf::seconds(0.5);
	inf.knockback = 1000.f;
	inf.momentum_knockback = 0.f;
#undef sk
	


#define sk Wave_Shot
	buildup[sk] = sf::seconds(0);
	duration[sk] = dt_max_fps * 2.f;
	bullet_speed[sk] = 400.f;
	acceleration[sk] = 12000.f;
	normal_speed[sk] = 0.f;
	period_factor[sk] = 20.f;
	damage[sk] = 10.f;
	inf.tag = Tag::Projectile;
	inf.dTag = DTag::Intangible;
	inf.oTag = OTag::Damage;
	inf.pTag = PTag::Intangible;
	inf.delete_on_hit = true;
	inf.stun_time = sf::seconds(0.5);
	inf.momentum_knockback = 600.f;
	inf.knockback = 0.f;
	inf.on_wall = CollisionInfo::pass_through;
	inf.reflectable = true;
	bullet_duration[sk] = sf::seconds(10.f);
#undef sk
	


#define sk Dash
	buildup[sk]=sf::Time::Zero;
	duration[sk]=sf::seconds(0.5f);
	normal_speed[sk]=800.f;
	max_speed[sk]=850.f;
#undef sk
	



#define sk Dash_Strike
	buildup[sk]=sf::Time::Zero;
	duration[sk]=sf::seconds(1.f/3.f);
	normal_speed[sk]=800.f;
	max_speed[sk]=850.f;
	damage[sk] = 20.f;
	inf.tag = Tag::Hit_Box;
	inf.dTag = DTag::Intangible;
	inf.oTag = OTag::Damage;
	inf.pTag = PTag::Intangible;
	inf.delete_on_hit = false;
	inf.stun_time = sf::seconds(0.5);
	inf.knockback = 800.f;
	inf.momentum_knockback = 0.f;
#undef sk
	


#define sk Stun_Roar
	inf.dTag = DTag::Intangible;
	inf.pTag = PTag::Intangible;
	inf.oTag = OTag::Stun;
	inf.stun_time = sf::seconds(2.f);
	inf.knockback = 0;
	inf.damage = 0;
	inf.momentum_knockback = 0;
	inf.delete_on_hit = false;
	buildup[sk]=sf::seconds(0.3f);
	duration[sk]=sf::seconds(0.6f);
	hb_duration[sk]=dt_max_fps * 2.f;
#undef sk


#define sk Tennis_Ball
	buildup[sk] = sf::seconds(0);
	duration[sk] = dt_max_fps * 5.f;
	bullet_duration[sk] = sf::seconds(-1.f);
	bullet_speed[sk] = 400.f;
	bullet_color[sk] = Color::Green;
	bullet_radius[sk] = 4.f;
	damage[sk] = 10.f;
	inf.tag = Tag::Projectile;
	inf.dTag = DTag::Intangible;
	inf.oTag = OTag::Damage;
	inf.pTag = PTag::Dynamic_Non_Solid;
	inf.bounce_factor = 1.f;
	inf.delete_on_hit = false;
	inf.stun_time = sf::seconds(0.5);
	inf.knockback = 0.f;
	inf.momentum_knockback = 600.f;
	inf.on_wall = CollisionInfo::bounce;
	inf.reflectable = true;
	inf.change_team_on_wall = false;
#undef sk



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
	#undef inf
}