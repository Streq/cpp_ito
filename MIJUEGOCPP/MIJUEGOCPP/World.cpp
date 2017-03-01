#include "World.h"
#include "defines.h"
#include "components_header.h"
#include <SFML\Graphics.hpp>
#include "Color.h"
#include "vec_magn.h"
#include <iostream>
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
	bool tiene_padre = TESTMASK(vec_Entity[entity], flag::Owner);//retorna falso si no tiene padre
	const auto& mat = Owner::get_matriz_padre_hijo();

	if (tiene_padre) {
		Owner::set(vec_Owner[entity].owner, entity, Relacion::none);
	}

	
	for (Handle hijo = 0; hijo < max_entities; hijo++) {
		if (mat[entity][hijo]) {
			Owner::set(entity, hijo, Relacion::none);
			switch(mat[entity][hijo]){
				case Relacion::composition:
				case Relacion::delete_on_hurt:{
					remove_entity(hijo);
				}
				
			}
		}
	}
	this->notify_systems(vec_Entity[entity].reset(), entity);
	return ret;
}

void World::remove_children(Handle entity, Relacion::ID criteria){
	const auto& mat = Owner::get_matriz_padre_hijo();
	for (Handle hijo = 0; hijo < max_entities; hijo++) {
		if (mat[entity][hijo]==criteria) {
				Owner::set(entity, hijo, Relacion::none);
				remove_entity(hijo);
		}
	}
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

void World::make_player(const sf::Vector2f & pos, short unsigned player, Character::ID _class){
	Handle h = new_entity();
	vec_players[player] = h;
	Rendering *r;
	if (r = add_component<Rendering>(h)) {
		auto siz=Character::Stats::size[_class];
		sf::Vector2f size(siz, siz);
		sf::RectangleShape *c = new sf::RectangleShape(size);
		c->setOutlineThickness(-1);
		CollisionBody* box = add_component<CollisionBody>(h);
		box->offset = -(size / 2.f);
		box->size = c->getSize();
		c->setOrigin(size / 2.f);
		r->drawable.reset(c);
	}
	Position *p;
	if (p = add_component<Position>(h)) {
		p->setPosition(pos);
	}
	Movement *m;
	if (m = add_component<Movement>(h)) {
	}
	CollisionInfo* tf;
	if (tf = add_component<CollisionInfo>(h)) {
		tf->tag = Tag::Character_Entity;
	}
	Controller* cont;
	if (cont = add_component<Controller>(h)) {
		cont->controller = controller::Player;
		cont->player = player;
	}
	State *st;
	if (st = add_component<State>(h)) {
		st->Class=_class;
		st->update(States::Normal);
		st->facing_dir.y = -1.f;

	}

	Health* hl;
	if (hl = add_component<Health>(h)) {
		hl->init(Character::Stats::health[_class]);
	}

}

void World::make_bullet(const sf::Vector2f & position, const sf::Vector2f & direction, const sf::Vector2f & inertial_speed, float speed, CollisionInfo&& colinfo, sf::Time duration, Handle owner)
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
	CollisionInfo* tag = add_component<CollisionInfo>(h);
	if (tag) {
		*tag = colinfo;
	}

	Movement* mov = add_component<Movement>(h);
	if (mov) {
		mov->velocity = inertial_speed + direction*speed;
		mov->capped = false;
		mov->friction = 0.f;
	}
	Position* pos = add_component<Position>(h);
	if (pos) {
		pos->setPosition(position);
	}
	Owner* own = add_component<Owner>(h);
	if (own) {
		own->set_owner(owner, h ,Relacion::aggregation);
	}
	
	TimeSpan* time = add_component<TimeSpan>(h);
	if (time) {
		time->time = duration;
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
		p->relative_to = owner;
	}
	CollisionInfo *tf;
	if (tf = add_component<CollisionInfo>(h)) {
		tf->tag = Tag::Hit_Box;
	}
	Owner* tm;
	if (tm = add_component<Owner>(h)) {
		tm->set_owner(owner, h, Relacion::delete_on_hurt);
	}
	State *st;
	if (st = add_component<State>(h)) {
		st->update(States::HitBox);
	}
}

void World::make_hit_box(const sf::Vector2f & offset, const sf::Vector2f & size, Handle owner, CollisionInfo && info, sf::Time duration){
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
		p->relative_to = owner;
	}
	CollisionInfo *tf;
	if (tf = add_component<CollisionInfo>(h)) {
		*tf = std::move(info);
	}
	Owner* tm;
	if (tm = add_component<Owner>(h)) {
		tm->set_owner(owner, h, Relacion::delete_on_hurt);
	}
	State *st;
	if (st = add_component<State>(h)) {
		st->update(States::HitBox);
		st->duration = duration;
		st->just_started = false;
	}
	
}

void World::make_wave_bullet(const sf::Vector2f& position, const sf::Vector2f& axis_direction, float normal_speed, float tangent_speed, float normal_acceleration, CollisionInfo&& colinfo, sf::Time duration, Handle owner)
{
	Handle h = new_entity();
	Rendering* rend = add_component<Rendering>(h);
	if (rend) {
		sf::CircleShape* circ = new sf::CircleShape(Bullet::stats::radius);
		circ->setOrigin(Bullet::stats::radius, Bullet::stats::radius);
		circ->setFillColor(sf::Color::Magenta);
		rend->drawable.reset(circ);
	}
	CollisionBody* body = add_component<CollisionBody>(h);
	if (body) {
		body->type = BoxType::Circle;
		body->size = sf::Vector2f(1.f, 1.f) * (Bullet::stats::radius * 2.f);
		body->offset = sf::Vector2f(1.f, 1.f) * (-Bullet::stats::radius);
	}
	CollisionInfo* tag = add_component<CollisionInfo>(h);
	if (tag) {
		*tag = colinfo;
	}

	Movement* mov = add_component<Movement>(h);
	if (mov) {

		mov->maxspeed = abs(normal_speed);
		//mov->velocity = from_angle(_angle + angle)*mov->maxspeed;
		//mov->velocity = axis_direction*init_speed;
		mov->velocity = axis_direction*tangent_speed + normal(axis_direction)*normal_speed;
		mov->capped = false;
		mov->friction = 0.f;
	}
	Position* pos = add_component<Position>(h);
	if (pos) {
		pos->setPosition(position);
	}
	Owner* own = add_component<Owner>(h);
	if (own) {
		own->set_owner(owner, h, Relacion::aggregation);
	}
	State* st = add_component<State>(h);
	if (st) {
		st->update(States::Wave_Shot);
		st->moving_dir = normalized(normal(axis_direction));
		if (normal_speed < 0){
			st->moving_dir = -st->moving_dir;
		}
		else if (normal_speed == 0.f) {
			st->moving_dir *= 0.f;
		}
	}

	TimeSpan* time = add_component<TimeSpan>(h);
	if (time) {
		time->time = duration;
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
	CollisionInfo *tf;
	if (tf = add_component<CollisionInfo>(h)) {
		tf->tag = Tag::Wall;
	}

}

void World::make_teleport_scope(const sf::Vector2f & position, float maxspeed, const sf::Vector2f & size, Handle owner){
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
		m->maxspeed = maxspeed;
		m->capped = true;
		m->friction = Skill::friction[Skill::Teleport];
	}
	Controller *cont;
	if (cont = add_component<Controller>(h)) {
		cont->controller = vec_Controller[owner].controller;
		cont->player = vec_Controller[owner].player;
	}
	State *st;
	if (st = add_component<State>(h)) {
		st->Class = Character::Default;
		st->update(States::Teleport_Scope);
	}
	Owner *tm;
	if (tm = add_component<Owner>(h)) {
		tm->set_owner(owner, h, Relacion::delete_on_hurt);
	}
	CollisionInfo *ci;
	if (ci = add_component<CollisionInfo>(h)) {
		*ci = Skill::col_info[Skill::Teleport];
	}
	CollisionBody *cb;
	if (cb = add_component<CollisionBody>(h)) {
		cb->offset = -size / 2.f;
		cb->size = size;
		cb->type = BoxType::Box;
	}
}

void World::make_spawner(const sf::Vector2f & pos, sf::Time spawn_time, int amount){
	Handle h = new_entity();
	Rendering *r;
	if (r = add_component<Rendering>(h)) {
		auto siz = Character::Stats::size[Character::Zombie];
		sf::Vector2f msize (siz,siz);
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
		st->Class = Character::Zombie;
	}
	if (amount >= 0) {
		TimeSpan *ts;
		if (ts = add_component<TimeSpan>(h)) {
			ts->time = spawn_time*sf::Int64(amount);
		}
	}
}



void World::make_zombie(const sf::Vector2f & pos) {
	Handle h = new_entity();
	auto siz = Character::Stats::size[Character::Zombie];
	sf::Vector2f size = sf::Vector2f(siz, siz);
	Rendering *r;
	if (r = add_component<Rendering>(h)) {
		sf::RectangleShape *c = new sf::RectangleShape(size);
		c->setOutlineThickness(-1);
		CollisionBody* box = add_component<CollisionBody>(h);
		box->offset = -(size / 2.f);
		box->size = size;
		c->setOrigin(size / 2.f);
		r->drawable.reset(c);
	}
	Position *p;
	if (p = add_component<Position>(h)) {
		p->setPosition(pos);
	}
	Movement *m;
	if (m = add_component<Movement>(h)) {
	}
	CollisionInfo *tf;
	if (tf = add_component<CollisionInfo>(h)) {
		tf->tag = Tag::Character_Entity;
	}
	Controller* cont;
	if (cont = add_component<Controller>(h)) {
		cont->controller = controller::Enemy;
	}
	State* st;
	if (st = add_component<State>(h)) {
		st->Class = Character::Zombie;
		st->update(States::Normal);
	}
	Health* hl;
	if (hl = add_component<Health>(h)) {
		hl->init(Character::Stats::health[Character::Zombie]);
	}
	/*CollisionInfo inf;
	inf.damage = Character::Stats::melee_hitbox_damage[Character::Zombie];
	inf.type = HitBoxType::Hit;
	inf.tag = Tag::Hit_Box;
	inf.knockback = 100.f;
	inf.stun_time = sf::seconds(0.5f);
	make_hit_box(sf::Vector2f(0, 0), size, h, std::move(inf), sf::seconds(-1.f));*/
}
