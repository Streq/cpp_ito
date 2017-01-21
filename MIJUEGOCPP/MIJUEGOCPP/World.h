#pragma once
#include "Entity.h"
#include "Component.h"
#include "System.h"
#include <array>
#include "defines.h"
#include "Collision.h"
#include <queue>


class System;

class World {
	private:
		std::vector<Entity>						vec_Entity;
		std::vector<ptr<System>>&				vec_system;
		sf::Vector2f							size;

		void									notify_systems(const Flagset& fl, Handle h);

	public:

		/*std::vector<Component1>				vec_Component1;*/
		#define X(component) std::vector<component> vec_##component;
		DO_X_FOR_COMPONENT_LIST
		#undef X

		std::queue<Collision>					collision_queue;

		
		Handle									new_entity();
		bool									remove_entity(Handle);
		template<typename C> C*					add_component(Handle);//(returns reference to component) 
		template <typename C> bool				remove_component(Handle);
		template<typename C> std::vector<C>&	get_component_vec();
		World(std::vector<ptr<System>>&,const sf::Vector2f& size);
		const sf::Vector2f&						getSize() const;
};