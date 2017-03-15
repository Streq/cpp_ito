#pragma once
#include "Entity.h"
#include "components_header.h"
#include "System.h"
#include <array>
#include "defines.h"
#include "Collision.h"
#include <queue>
#include "Player.h"

class System;

class World {
	private:
		std::vector<ptr<System>>&				vec_system;
		sf::Vector2f							size;


		void									notify_systems(const Flagset& fl, Handle h);

	public:

		/*std::vector<Component1>				vec_Component1;*/
		#define X(component) std::vector<component> vec_##component;
		DO_X_FOR_COMPONENT_LIST
		#undef X
		std::vector<Entity>						vec_Entity;

		std::queue<Collision>					collision_queue;
		std::queue<Collision>					collision_queue_physics;
		std::queue<Collision>					collision_queue_hitbox;
		std::array<Handle,MAX_PLAYERS>			vec_players;
		


		Handle									new_entity();
		bool									remove_entity(Handle);
		void									remove_children(Handle, Relacion::ID criteria);
		template<typename C> C*					add_component(Handle);//(returns reference to component) 
		template<typename C> C*					activate_component(Handle);//(returns reference to component) 
		template <typename C> bool				remove_component(Handle);
		template<typename C> std::vector<C>&	get_component_vec();
												World(std::vector<ptr<System>>&,const sf::Vector2f& size);
		const sf::Vector2f&						getSize() const;
		void									clear();
		
	public:
		Handle									make_player(const sf::Vector2f& pos, short unsigned player, Character::ID _class);
		Handle									make_zombie(const sf::Vector2f& pos);
		Handle									make_bullet(const sf::Vector2f & position, const sf::Vector2f & direction, const sf::Vector2f & inertial_speed, float speed, CollisionInfo && colinfo, sf::Time duration, Handle owner, float radius, sf::Color color, int owner_type);
		Handle									make_wall(const sf::Vector2f& position, const sf::Vector2f& size);
		Handle									make_hit_box(const sf::Vector2f& offset, const sf::Vector2f& size, Handle owner, CollisionInfo&& info, sf::Time duration, BoxType::Type btype = BoxType::Box);
		Handle									make_wave_bullet(const sf::Vector2f& position, const sf::Vector2f& axis_direction, float normal_speed, float tangent_speed, float normal_acceleration, CollisionInfo&& colinfo, sf::Time duration, Handle owner);
		Handle									make_teleport_scope(const sf::Vector2f& position,float maxspeed, const sf::Vector2f & size, Handle owner);
		Handle									make_spawner(const sf::Vector2f& pos, sf::Time spawn_time, int amount=-1);
		Handle									make_special_bullet(const sf::Vector2f & position, const sf::Vector2f & direction, const sf::Vector2f & inertial_speed, float speed, CollisionInfo && colinfo, sf::Time duration, Handle owner, float radius, sf::Color color, States::ID state);
};