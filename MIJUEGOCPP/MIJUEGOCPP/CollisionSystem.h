#pragma once
#include "System.h"
#include "system_defines.h"
#include <math.h>
#include <bitset>
#include "compile_pow.h"
#define grid_x 3
#define grid_y 3
#define grid_depth 2
const size_t grid_direct_children = grid_x*grid_y;
const size_t grid_leaves = get_power<grid_direct_children,grid_depth>::value;
const size_t grid_nodes = grid_depth>0?(grid_direct_children*grid_leaves - 1) / (grid_direct_children - 1):1;
const size_t grid_parent_nodes = grid_nodes - grid_leaves;
class CollisionSystem : public System {
	public:
		virtual void							update(sf::Time);
		CollisionSystem(World&);
		
		typedef bool CollisionMatrix[max_entities][max_entities];

	private:
		//Grid collision

		CollisionMatrix										collision_matrix = {};
		#ifdef LISTS
		std::vector<std::vector<Handle>>					section_entities;
		#endif //LISTS
		std::vector<CollisionBody>							section_boxes;
		
		#ifdef MASKS
		//MASK MODE WACHIN (ta pa pensarlo)
		std::array<std::bitset<grid_leaves>, grid_nodes>	section_masks;
		std::array<std::bitset<grid_leaves>, max_entities>	entity_masks;
		#endif // MASKS
		
		
		
		std::vector<CollisionBody>							entity_boxes;
		std::vector<sf::Vector2f>							frame_start_offset;
		void												check_collisions(float time);
		void												handle_collisions(float time);

		inline unsigned										first_child(unsigned nodo) { return nodo*grid_direct_children + 1; };
		inline unsigned										children_end(unsigned nodo) { return nodo*grid_direct_children + grid_direct_children + 1; };
		inline unsigned										parent(unsigned nodo) { return (nodo - 1)/grid_direct_children; };
		static bool											check_collision_box(const CollisionBody& b1, const CollisionBody& b2);
		static bool											check_collision_circle_box(const CollisionBody& circ, const CollisionBody& box);
		static bool											check_collision_circle(const CollisionBody& c1, const CollisionBody& c2);
		void												translate_boxes_to_global();
		inline void											collide(Handle h1, Handle h2);
};

