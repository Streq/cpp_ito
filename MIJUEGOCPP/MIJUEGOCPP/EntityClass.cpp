#include "EntityClass.h"

#define do_for_all_chars \
	X(TimeTraveler)\
	X(Zombie)\
	X(Giant)\
	X(Tennist)\
	X(Default)\
	X(Minotaur)


float Character::Stats::size[ID::size];
float Character::Stats::health[ID::size];
float Character::Stats::p_attack[ID::size];
float Character::Stats::m_attack[ID::size];
float Character::Stats::mov_speed[ID::size];
float Character::Stats::mov_acceleration[ID::size];
float Character::Stats::mass[ID::size];
float Character::Stats::mana[ID::size];
Skill::ID Character::Stats::skill[ID::size][skill_num];
int Character::Stats::hitbox_type[ID::size][States::size];
sf::Color Character::Stats::color[ID::size][States::size];

#define set(att, ch, val) Character::Stats::##att[ID::##ch]=val;
#define setst(att, ch, st, val) Character::Stats::##att[ID::##ch][States::##st] = val;
void Character::Stats::init() {
//default set
#define X(ch) \
set(health,ch,100.f)\
set(size,ch,10.f)\
set(p_attack,ch,1.f)\
set(m_attack,ch,1.f)\
set(mov_speed,ch,200.f)\
set(mov_acceleration,ch,600.f)\
set(mass,ch,1.f)\
set(mana,ch,10.f)\
setst(hitbox_type, ch, Hurt, HitBoxType::Intangible)\
setst(hitbox_type, ch, Casting, HitBoxType::Damageable)\
setst(hitbox_type, ch, Normal, HitBoxType::Damageable)\
setst(hitbox_type, ch, Silenced, HitBoxType::Damageable)
	do_for_all_chars;
#undef X

	size[ID::Minotaur] = 12.f;
	size[ID::Giant] = 30.f;

	set(mov_speed, Zombie, 400.f);

	health[ID::Zombie] = 30.f;
	health[ID::Minotaur] = 150.f;
	health[ID::Giant] = 300.f;

	//Skills
#define set_skill(ch,num,sk) skill[ID::ch][num]=Skill::sk;

#define set_skills(ch,sk0,sk1,sk2,sk3) \
set_skill(ch,0,sk0);\
set_skill(ch,1,sk1);\
set_skill(ch,2,sk2);\
set_skill(ch,3,sk3);

	set_skills(TimeTraveler, Simple_Shot, Wave_Shot, Dash, Teleport);
	set_skills(Minotaur, Simple_Melee, Ram, Dash, Stun_Roar);
	set_skills(Giant, Slow_Big_Melee, None, None, None);
	set_skills(Tennist, Bounce_Shot, Mirror_Melee, Dash, None);




	
#define set_color(ch, st, col) color[ID::##ch][States::##st]=col;
	set_color(TimeTraveler, Normal, Color::Blue);
	set_color(TimeTraveler, Hurt, Color::Red);
	set_color(TimeTraveler, Casting, Color::Blue + Color::Grey);

	set_color(Tennist, Normal, Color::Green);
	set_color(Tennist, Hurt, Color::Red);
	set_color(Tennist, Casting, Color::Green + Color::Grey);

	set_color(Minotaur, Normal, Color::Magenta);
	set_color(Minotaur, Hurt, Color::Red);
	set_color(Minotaur, Casting, Color::Brown + Color::Grey);

	set_color(Zombie, Normal, Color::Dark_Green);
	set_color(Zombie, Hurt, Color::Red);

	set_color(Giant, Normal, Color::Brown);
	set_color(Giant, Hurt, Color::Red);
	set_color(Zombie, Spawner, Color::Transparent);

	set_color(Default, HitBox, Color::Red);
}
