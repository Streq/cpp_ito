#include "World.h"
#include "defines.h"
#include "components_header.h"
#include <SFML\Graphics.hpp>
#include "Color.h"

template<typename C> C*					World::add_component(Handle) { const a = 3; const a = 4; }
template <typename C> bool				World::remove_component(Handle) { const a = 3; const a = 4; }
template<typename C> std::vector<C>&	World::get_component_vec() { const a = 3; const a = 4; }
template<typename C> C*					World::activate_component(Handle) { const a = 3; const a = 4; }


World::World(std::vector<ptr<System>>& vec,const sf::Vector2f& size)
	: vec_Entity(max_entities)
#define X(Comp) , vec_##Comp(max_entities)
	DO_X_FOR_COMPONENT_LIST
#undef X
	, vec_system(vec)
	, size(size)
{
}



#define add_comp(component_name)\
template<>\
component_name* World::add_component<component_name>(Handle handle)\
{\
	Entity& entity = vec_Entity[handle];\
	if(TESTMASK(entity,flag::##component_name)){ return NULL; }\
	entity |= flag::##component_name;\
	notify_systems(entity,handle);\
	auto& comp=vec_##component_name[handle];\
	comp=component_name();\
	return &comp;\
}

#define activate_comp(component_name)\
template<>\
component_name* World::activate_component<component_name>(Handle handle)\
{\
	Entity& entity = vec_Entity[handle];\
	if(TESTMASK(entity,flag::##component_name)){ return NULL; }\
	entity |= flag::##component_name;\
	notify_systems(entity,handle);\
	auto& comp=vec_##component_name[handle];\
	return &comp;\
}



#define remove_comp(component_name)\
template<>\
bool World::remove_component<component_name>(Handle handle)\
{\
	Entity& entity = vec_Entity[handle];\
	bool ret = TESTMASK(entity, flag::##component_name);\
	entity &= ~flag::##component_name;\
	if(ret){\
		notify_systems(entity, handle);\
	};\
	return ret;\
}


#define get_comp_vec(component_name) template<>\
std::vector<component_name>& World::get_component_vec<component_name>() {\
	return vec_##component_name;\
}

#define component_defines(comp_name) \
	add_comp(comp_name)\
	remove_comp(comp_name)\
	get_comp_vec(comp_name)\
	activate_comp(comp_name)



#define X(component) component_defines(component)
DO_X_FOR_COMPONENT_LIST
#undef X



Handle World::new_entity() {
	for (uint32_t i = 0; i < max_entities; i++) {
		if ((vec_Entity[i] & (Flagset)flag::Active).none()) {//si la entidad no existe	
			vec_Entity[i] = flag::Active;
			return i;
		}
	}
	return max_entities;
}

bool World::remove_entity(Handle entity)
{
	bool ret = TESTMASK(vec_Entity[entity], flag::Active);//retorna falso si la entidad no existe
	this->notify_systems(vec_Entity[entity].reset(), entity);
	return ret;
}





void World::notify_systems(const Flagset& fl, Handle h) {
	for (ptr<System>& s : vec_system) {
		s->update_entity(fl, h);
	}
}


const sf::Vector2f&	World::getSize() const { return size; }

void World::clear(){
	for (Handle i = 0; i < max_entities; i++) {
		remove_entity(i);
	}
}

void World::make_player(const sf::Vector2f & pos){
	Handle h = new_entity();
	Rendering *r;
	if (r = add_component<Rendering>(h)) {
		sf::RectangleShape *c = new sf::RectangleShape(sf::Vector2f(Player::stats::size, Player::stats::size));
		c->setFillColor(sf::Color::Green);
		c->setOutlineThickness(-1);
		auto& rect = c->getSize();
		CollisionBody* box = add_component<CollisionBody>(h);
		box->offset = -(sf::Vector2f(rect.x, rect.y) / 2.f);
		box->size = c->getSize();
		c->setOrigin(sf::Vector2f(rect.x, rect.y) / 2.f);
		r->drawable.reset(c);
	}
	Position *p;
	if (p = add_component<Position>(h)) {
		p->setPosition(pos);
	}
	Movement *m;
	if (m = add_component<Movement>(h)) {
		m->friction = Player::stats::friction;
		m->maxspeed = Player::stats::max_speed;
	}
	CollisionTag* tf;
	if (tf = add_component<CollisionTag>(h)) {
		tf->tag = Tag::Player;
	}
	Controller* cont;
	if (cont = add_component<Controller>(h)) {
		cont->controller = controller::Player;
		cont->facing_dir.y = -1.f;
	}
	State *st;
	if (st = add_component<State>(h)) {
		st->update(States::Normal);
	}

	Health* hl;
	if (hl = add_component<Health>(h)) {
		hl->init(Player::stats::health);
	}

}

void World::make_bullet(const sf::Vector2f & position, const sf::Vector2f & direction, const sf::Vector2f & inertial_speed, float speed, Handle owner)
{
	Handle h = new_entity();
	Rendering* rend = add_component<Rendering>(h);
	if (rend) {
		sf::CircleShape* circ = new sf::CircleShape(Bullet::stats::radius);
		circ->setOrigin(Bullet::stats::radius,Bullet::stats::radius);
		circ->setFillColor(sf::Color::Red);
		rend->drawable.reset(circ);
	}
	CollisionBody* body = add_component<CollisionBody>(h);
	if (body) {
		body->type = BoxType::Circle;
		body->size = sf::Vector2f(1.f, 1.f) * (Bullet::stats::radius * 2.f);
		body->offset = sf::Vector2f(1.f, 1.f) * (-Bullet::stats::radius);
	}
	CollisionTag* tag = add_component<CollisionTag>(h);
	if (tag) {
		tag->tag = Tag::Bullet;
	}

	Movement* mov = add_component<Movement>(h);
	if (mov) {
		mov->velocity = inertial_speed + direction*speed;
	}
	Position* pos = add_component<Position>(h);
	if (pos) {
		pos->setPosition(position);
	}
	Team* own = add_component<Team>(h);
	if (own) {
		own->owner = owner;
	}
	
	TimeSpan* time = add_component<TimeSpan>(h);
	if (time) {
		time->time = Bullet::stats::time;
	}
	
	Damage* dmg = add_component<Damage>(h);
	if (dmg) {
		dmg->amount = Bullet::stats::damage;
	}

}

void World::make_hit_box(const sf::Vector2f & offset, const sf::Vector2f & size, Handle owner){
	Handle h = new_entity();
	Rendering *r;
	if (r = add_component<Rendering>(h)) {
		sf::RectangleShape *c = new sf::RectangleShape(size);
		c->setFillColor(Color::Red);
		c->setOrigin(size / 2.f - offset);
		r->drawable.reset(c);
	}
	CollisionBody* box = add_component<CollisionBody>(h);
	if (box) {
		box->size = size;
		box->offset = -(size / 2.f - offset);
	}
	Position *p;
	if (p = add_component<Position>(h)) {
		p->setPosition(vec_Position[owner].getPosition());
	}
	CollisionTag *tf;
	if (tf = add_component<CollisionTag>(h)) {
		tf->tag = Tag::Hit_Box;
	}
	Team* tm;
	if (tm = add_component<Team>(h)) {
		tm->owner = owner;
	}
	State *st;
	if (st = add_component<State>(h)) {
		st->update(States::Hit_Box);
	}
	Damage *dmg;
	if (dmg = add_component<Damage>(h)) {
		dmg->amount = Hit_Box::stats::damage;
	}
}

void World::make_wall(const sf::Vector2f & position, const sf::Vector2f & size){
	Handle h = new_entity();
	Rendering *r;
	if (r = add_component<Rendering>(h)) {
		sf::RectangleShape *c = new sf::RectangleShape(size);
		c->setFillColor(Color::Brown);
		auto& rect = c->getSize();
		r->drawable.reset(c);
	}
	CollisionBody* box = add_component<CollisionBody>(h);
	if (box) {
		box->size = size;
	}
	Position *p;
	if (p = add_component<Position>(h)) {
		p->setPosition(position);
	}
	CollisionTag *tf;
	if (tf = add_component<CollisionTag>(h)) {
		tf->tag = Tag::Building;
	}

}

void World::make_teleport_scope(const sf::Vector2f & position, const sf::Vector2f & size, Handle owner){
	Handle h = new_entity();
	Rendering *r;
	if (r = add_component<Rendering>(h)) {
		sf::RectangleShape *c = new sf::RectangleShape(size);
		c->setFillColor(Color::Transparent);
		c->setOutlineThickness(-1.f);
		c->setOutlineColor(Color::Red);
		c->setOrigin(size.x / 2, size.y / 2);
		r->drawable.reset(c);
	}
	Position *p;
	if (p = add_component<Position>(h)) {
		p->setPosition(position);
	}
	Movement *m;
	if (m = add_component<Movement>(h)) {
		m->friction = Teleport_Scope::stats::friction;
		m->maxspeed = Teleport_Scope::stats::max_speed;
	}
	Controller *cont;
	if (cont = add_component<Controller>(h)) {
		cont->controller = vec_Controller[owner].controller;
	}
	State *st;
	if (st = add_component<State>(h)) {
		st->update(States::Teleport_Scope);
	}
	Team *tm;
	if (tm = add_component<Team>(h)) {
		tm->owner = owner;
	}
}

void World::make_spawner(const sf::Vector2f & pos, sf::Time spawn_time, int amount){
	Handle h = new_entity();
	Rendering *r;
	if (r = add_component<Rendering>(h)) {
		sf::Vector2f msize (Enemy::stats::size, Enemy::stats::size);
		sf::RectangleShape *c = new sf::RectangleShape(msize);
		c->setFillColor(Color::Transparent);
		c->setOutlineThickness(-1.f);
		c->setOutlineColor(Color::Blue);
		c->setOrigin(msize.x / 2, msize.y / 2);
		r->drawable.reset(c);
	}
	Position *p;
	if (p = add_component<Position>(h)) {
		p->setPosition(pos);
	}
	State *st;
	if (st = add_component<State>(h)) {
		st->update(States::Spawner);
		st->duration = spawn_time;
	}
	if (amount >= 0) {
		TimeSpan *ts;
		if (ts = add_component<TimeSpan>(h)) {
			ts->time = spawn_time*sf::Int64(amount);
		}
	}
}



void World::make_enemy(const sf::Vector2f & pos) {
	Handle h = new_entity();
	Rendering *r;
	if (r = add_component<Rendering>(h)) {
		sf::RectangleShape *c = new sf::RectangleShape(sf::Vector2f(Enemy::stats::size, Enemy::stats::size));
		c->setFillColor(sf::Color::Green);
		c->setOutlineThickness(-1);
		auto& rect = c->getSize();
		CollisionBody* box = add_component<CollisionBody>(h);
		box->offset = -(sf::Vector2f(rect.x, rect.y) / 2.f);
		box->size = c->getSize();
		c->setOrigin(sf::Vector2f(rect.x, rect.y) / 2.f);
		r->drawable.reset(c);
	}
	Position *p;
	if (p = add_component<Position>(h)) {
		p->setPosition(pos);
	}
	Movement *m;
	if (m = add_component<Movement>(h)) {
		m->friction = Enemy::stats::friction;
		m->maxspeed = Enemy::stats::max_speed;
	}
	CollisionTag *tf;
	if (tf = add_component<CollisionTag>(h)) {
		tf->tag = Tag::Enemy;
	}
	Controller* cont;
	if (cont = add_component<Controller>(h)) {
		cont->controller = controller::Enemy;
	}
	State* st;
	if (st = add_component<State>(h)) {
		st->update(States::Enemy_Normal);
	}
	Health* hl;
	if (hl = add_component<Health>(h)) {
		hl->init(Enemy::stats::health);
	}
	Damage* dmg;
	if(dmg = add_component<Damage>(h)){
		dmg->amount = Enemy::stats::damage;
	}
}
