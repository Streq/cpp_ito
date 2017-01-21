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
		std::vector<CollisionBox>							section_boxes;
		
		#ifdef MASKS
		//MASK MODE WACHIN (ta pa pensarlo)
		std::array<std::bitset<grid_leaves>, grid_nodes>	section_masks;
		std::array<std::bitset<grid_leaves>, max_entities>	entity_masks;
		#endif // MASKS
		
		
		
		std::vector<CollisionBox>							entity_boxes;
		
		inline unsigned										first_child(unsigned nodo) { return nodo*grid_direct_children + 1; };
		inline unsigned										children_end(unsigned nodo) { return nodo*grid_direct_children + grid_direct_children + 1; };
		inline unsigned										parent(unsigned nodo) { return (nodo - 1)/grid_direct_children; };
		bool												check_collision(Handle h1, Handle h2)const;
		static bool											check_collision(const CollisionBox& b1, const CollisionBox& b2);
		void												translate_boxes_to_global();
		inline void											collide(Handle h1, Handle h2);
};

