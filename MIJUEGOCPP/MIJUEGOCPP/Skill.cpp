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
int Skill::owner_type[size];
sf::Color Skill::bullet_color[size];
float Skill::bullet_radius[size];
float Skill::period_factor[size];
float Skill::inertia_factor[size];
int Skill::ammo[size];


void Skill::init() {

	#define inf col_info[sk]


	
#define sk Simple_Melee
	ammo[sk] = -1;
	buildup[sk] = sf::seconds(0);
	duration[sk] = dt_max_fps * 15.f;
	hb_duration[sk] = dt_max_fps * 10.f;
	damage[sk] = 100.f;
	 
	inf.dTag = DTag::Intangible;
	inf.oTag = OTag::Damage;
	inf.pTag = PTag::Intangible;
	inf.delete_on_hit = false;
	inf.stun_time = sf::seconds(0.5);
	inf.knockback = 400.f;
	inf.momentum_knockback = 0.f;
#undef sk
	
#define sk Slow_Big_Melee
	ammo[sk] = -1;
	buildup[sk] = sf::seconds(0.3f);
	duration[sk] = buildup[sk] + dt_max_fps * 15.f;
	hb_duration[sk] = dt_max_fps * 10.f;
	damage[sk] = 200.f;
	inf.dTag = DTag::Intangible;
	inf.oTag = OTag::Damage;
	inf.pTag = PTag::Intangible;
	inf.delete_on_hit = false;
	inf.stun_time = sf::seconds(0.5);
	inf.knockback = 800.f;
	inf.momentum_knockback = 0.f;
#undef sk
	




#define sk Mirror_Melee
	ammo[sk] = -1;
	buildup[sk] = sf::seconds(0);
	duration[sk] = dt_max_fps * 10.f;
	hb_duration[sk] = dt_max_fps * 8.f;
	damage[sk] = 100.f;
	inf.dTag = DTag::Reflect;
	inf.oTag = OTag::Damage;
	inf.pTag = PTag::Intangible;
	inf.delete_on_hit = false;
	inf.stun_time = sf::seconds(0.3);
	inf.knockback = 450.f;
	inf.momentum_knockback = 0.f;
#undef sk
	



	
#define sk Simple_Shot
	inertia_factor[sk] = 1.f;
	ammo[sk] = -1;
	buildup[sk] = sf::seconds(0);
	duration[sk] = dt_max_fps * 2.f;
	bullet_duration[sk] = sf::seconds(2.f);
	bullet_speed[sk] = 350.f;
	bullet_color[sk] = Color::Red;
	bullet_radius[sk] = 2.5;
	damage[sk] = 100.f;
	owner_type[sk] = Relacion::aggregation;
	inf.dTag = DTag::Intangible;
	inf.oTag = OTag::Damage;
	inf.pTag = PTag::Dynamic_Non_Solid;
	inf.delete_on_hit = true;
	inf.stun_time = sf::seconds(0.5);
	inf.momentum_knockback = 400.f;
	inf.knockback = 0.f;
	inf.on_wall = CollisionInfo::remove;
	inf.reflectable = true;
	inf.change_team_on_wall = false;
#undef sk
	
#define sk Quake

	inertia_factor[sk] = 0.f;
	ammo[sk] = -1;
	buildup[sk] = sf::seconds(1.f);
	duration[sk] = buildup[sk] + dt_max_fps * 2.f;
	bullet_duration[sk] = sf::seconds(20.f);
	bullet_speed[sk] = 100.f;
	bullet_color[sk] = sf::Color(Color::Brown.r,Color::Brown.g,Color::Brown.b,128);
	bullet_radius[sk] = 75;
	
	damage[sk] = 1.f;
	owner_type[sk] = Relacion::aggregation;
	inf.dTag = DTag::Intangible;
	inf.oTag = OTag::Stun;
	inf.pTag = PTag::Intangible;
	inf.delete_on_hit = false;
	inf.stun_time = sf::seconds(2.0f);
	inf.momentum_knockback = 0.f;
	inf.knockback = 0.f;
	inf.on_wall = CollisionInfo::pass_through;
	inf.reflectable = false;
	inf.change_team_on_wall = false;
#undef sk
	
#define sk Simple_Shot_Zombie

	inertia_factor[sk] = 1.f;
	ammo[sk] = -1;
	buildup[sk] = sf::seconds(0);
	duration[sk] = sf::seconds(0.5f);
	bullet_duration[sk] = sf::seconds(2.f);
	bullet_speed[sk] = 350.f;
	bullet_color[sk] = Color::Red;
	bullet_radius[sk] = 2.5;
	damage[sk] = 100.f;
	owner_type[sk] = Relacion::aggregation;
	inf.dTag = DTag::Intangible;
	inf.oTag = OTag::Damage;
	inf.pTag = PTag::Dynamic_Non_Solid;
	inf.delete_on_hit = true;
	inf.stun_time = sf::seconds(0.5);
	inf.momentum_knockback = 400.f;
	inf.knockback = 0.f;
	inf.on_wall = CollisionInfo::remove;
	inf.reflectable = true;
	inf.change_team_on_wall = false;
#undef sk


#define sk Bounce_Shot

	inertia_factor[sk] = 1.f;
	ammo[sk] = -1;
	buildup[sk] = sf::seconds(0);
	duration[sk] = dt_max_fps * 5.f;
	bullet_duration[sk] = sf::seconds(5.f);
	bullet_speed[sk] = 300.f;
	bullet_color[sk] = Color::Red;
	bullet_radius[sk] = 2.5;
	damage[sk] = 100.f;
	owner_type[sk] = Relacion::aggregation;
	 
	inf.dTag = DTag::Intangible;
	inf.oTag = OTag::Damage;
	inf.pTag = PTag::Dynamic_Non_Solid;
	inf.bounce_factor = 1.f;
	inf.delete_on_hit = true;
	inf.stun_time = sf::seconds(0.5);
	inf.knockback = 0.f;
	inf.momentum_knockback = 400.f;
	inf.on_wall = CollisionInfo::bounce;
	inf.reflectable = true;
	inf.change_team_on_wall = false;
#undef sk





#define sk Teleport
	ammo[sk] = -1;
	buildup[sk] = sf::seconds(0);
	max_speed[sk] = 1000.f;
	acceleration[sk] = 12000.f;
	friction[sk] = 6000.f;
	 
	inf.dTag = DTag::Intangible;
	inf.oTag = OTag::None;
	inf.pTag = PTag::Dynamic_Non_Solid;
	inf.on_wall = CollisionInfo::pass_through;
	#undef sk
	




#define sk Ram
	ammo[sk] = -1;
	buildup[sk] = sf::seconds(0);
	duration[sk] = dt_max_fps * 40.f;
	damage[sk] = 100.f;
	acceleration[sk] = 6000.f;
	max_speed[sk] = 650.f;
	 
	inf.dTag = DTag::Intangible;
	inf.oTag = OTag::Damage;
	inf.pTag = PTag::Intangible;
	inf.delete_on_hit = false;
	inf.stun_time = sf::seconds(0.5);
	inf.knockback = 800.f;
	inf.momentum_knockback = 0.f;
#undef sk
	


#define sk Wave_Shot
	ammo[sk] = -1;
	buildup[sk] = sf::seconds(0);
	duration[sk] = dt_max_fps * 2.f;
	bullet_speed[sk] = 300.f;
	bullet_radius[sk] = 2.5f;
	acceleration[sk] = 1000.f;
	normal_speed[sk] = 0.f;
	period_factor[sk] = 10.f;
	owner_type[sk] = Relacion::aggregation;
	damage[sk] = 5.f;
	 
	inf.dTag = DTag::Intangible;
	inf.oTag = OTag::Damage;
	inf.pTag = PTag::Dynamic_Non_Solid;
	inf.delete_on_hit = true;
	inf.stun_time = sf::seconds(0.3);
	inf.momentum_knockback = 400.f;
	inf.knockback = 0.f;
	inf.on_wall = CollisionInfo::remove;
	inf.reflectable = true;
	bullet_duration[sk] = sf::seconds(10.f);
#undef sk
	


#define sk Dash
	ammo[sk] = -1;
	buildup[sk] = sf::Time::Zero;
	duration[sk] = sf::seconds(1.f/3.f);
	normal_speed[sk] = 600.f;
	max_speed[sk] = 650.f;
	friction[sk] = 800;
#undef sk
	



#define sk Dash_Strike
	ammo[sk] = -1;
	buildup[sk]=sf::Time::Zero;
	duration[sk]=sf::seconds(1.f/3.f);
	normal_speed[sk]=500.f;
	max_speed[sk]=550.f;
	damage[sk] = 100.f;
	 
	inf.dTag = DTag::Intangible;
	inf.oTag = OTag::Damage;
	inf.pTag = PTag::Intangible;
	inf.delete_on_hit = false;
	inf.stun_time = sf::seconds(0.5);
	inf.knockback = 650.f;
	inf.momentum_knockback = 0.f;
#undef sk
	


#define sk Stun_Roar
	ammo[sk] = -1;
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

	inertia_factor[sk] = 1.f;
	ammo[sk] = 3;
	buildup[sk] = sf::seconds(0);
	duration[sk] = dt_max_fps * 5.f;
	bullet_duration[sk] = sf::seconds(-1.f);
	bullet_speed[sk] = 350.f;
	bullet_color[sk] = Color::Green;
	bullet_radius[sk] = 4.f;
	owner_type[sk] = Relacion::aggregation;
	damage[sk] = 100.f;
	 
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


#define sk Telekinetic_Blade

	inertia_factor[sk] = 1.f;
	ammo[sk] = 7;
	buildup[sk] = sf::seconds(0);
	duration[sk] = dt_max_fps * 5.f;
	bullet_duration[sk] = sf::seconds(-1.f);
	bullet_speed[sk] = 300.f;
	bullet_color[sk] = Color::Grey;
	bullet_radius[sk] = 3.f;
	acceleration[sk] = 700.f;
	max_speed[sk] = 350.f;
	friction[sk] = 0.f;
	owner_type[sk] = Relacion::composition;
	damage[sk] = 100.f;
	 
	inf.dTag = DTag::Intangible;
	inf.oTag = OTag::Damage;
	inf.pTag = PTag::Dynamic_Non_Solid;
	inf.bounce_factor = 1.f;
	inf.delete_on_hit = false;
	inf.stun_time = sf::seconds(0.5);
	inf.knockback = 0.f;
	inf.momentum_knockback = 400.f;
	inf.on_wall = CollisionInfo::bounce;
	inf.reflectable = false;
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