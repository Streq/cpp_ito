#include "World.h"
#include "defines.h"


template<typename C> C*					World::add_component(Handle) { const a = 3; const a = 4; }
template <typename C> bool				World::remove_component(Handle) { const a = 3; const a = 4; }
template<typename C> std::vector<C>&	World::get_component_vec() { const a = 3; const a = 4; }

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
	return &vec_##component_name[handle];\
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
	get_comp_vec(comp_name)



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
	vec_Entity[entity].reset();
	return ret;
}





void World::notify_systems(const Flagset& fl, Handle h) {
	for (ptr<System>& s : vec_system) {
		s->check_entity(fl, h);
	}
}


const sf::Vector2f&	World::getSize() const { return size; };