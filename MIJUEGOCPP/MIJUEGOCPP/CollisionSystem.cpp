#include "CollisionSystem.h"
#include "CollisionSystem.h"
#include "system_defines.h"
#include "vec_magn.h"
#include <iostream>
#include "Game.h"
#include "Component.h"
CollisionSystem::CollisionSystem(World& world)
	:System(
		world,
		Requeriments(
			Flagset(
				#define X(C) flag::##C |
				COMPONENTS_COLLISION
				#undef X
				flag::none),
			Flagset(flag::none))),
	entity_boxes(max_entities),
	section_boxes(grid_nodes)
	#ifdef LISTS
	,section_entities(grid_nodes)
	#endif
{
	section_boxes[0] = CollisionBox(sf::Vector2f(0, 0), mWorld.getSize());
	for (unsigned cur = 0; cur < grid_parent_nodes; cur++) {
		CollisionBox& cur_box = section_boxes[cur];
		sf::Vector2f size = cur_box.size;
		size.x /= grid_x;
		size.y /= grid_y;

		unsigned end = children_end(cur);
		for (unsigned count = 0, child = first_child(cur); child < end; child++,count++) {
			CollisionBox& child_box = section_boxes[child];
			child_box.size = size;
			child_box.offset = cur_box.offset + sf::Vector2f(size.x * (count % grid_x), size.y * (count / grid_x));		
		}
	}
	#ifdef MASKS
	//Setear las mascaras wachin;
	//Somthin like this
	//[0]	   [1]      [5]
	//1 1 1 1  1 1 0 0  1 0 0 0
	//1 1 1 1  1 1 0 0  0 0 0 0
	//1 1 1 1  0 0 0 0  0 0 0 0
	//1 1 1 1  0 0 0 0  0 0 0 0

	for (unsigned leaf = 0; leaf < grid_leaves; leaf++) {
		//We offset to grid_parent_nodes because section_masks also contains the masks for the parents
		//but the bitset only contains the bits for the leaves
		section_masks[grid_parent_nodes + leaf].set(leaf);
	}

	//Now we fill the inner nodes, we start from the back since the first ones depend on the last ones
	for (int parent = grid_parent_nodes - 1; parent >= 0; parent--) {
		unsigned end = children_end(parent);
		for (unsigned child = first_child(parent); child < end; child++) {
			//every mask is made of its children's masks
			section_masks[parent] |= section_masks[child];
		}
	}
	#endif
}




void CollisionSystem::update(sf::Time t) {
	memset(collision_matrix,0,sizeof(CollisionMatrix));
	
	translate_boxes_to_global();
	//Clear entities
	//NOT MASK
	#ifdef LISTS
	for (auto i=0; i < grid_nodes; i++) {
		section_entities[i].clear();
	}
	//Fill main box with colliding entities
	const CollisionBox& main_box = section_boxes[0];
	auto& sec_entities = section_entities[0];
	#endif
	ITERATE_START
	
#ifdef MASKS
	entity_masks[i].reset();
#endif

	
	if (check_collision(section_boxes[0],entity_boxes[i])) {
		#ifdef LISTS
		sec_entities.push_back(i);
		#endif
		//MASK
		#ifdef MASKS
		entity_masks[i] = section_masks[0];
		#endif	
	}
	
	ITERATE_END;
	
	//fill every sub section recursively
	//NOT MASK
	#ifdef LISTS
	for (unsigned parent = 0; parent < grid_parent_nodes; parent++){
		const auto& parent_entities = section_entities[parent];
		unsigned end = children_end(parent);
		for (unsigned child = first_child(parent); child < end; child++) {
			for (Handle h : parent_entities) {
				if (check_collision(section_boxes[child], entity_boxes[h])) {
					section_entities[child].push_back(h);
				}
			}
		}
	}
	#endif
	//MASK
	#ifdef MASKS
	for (unsigned sec = 0; sec < grid_nodes; sec++) {
		ITERATE_START
		const auto& sec_mask = section_masks[sec];
		auto& ent_mask = entity_masks[i];

		if ((ent_mask & sec_mask).any()) {
			if (!check_collision(section_boxes[sec], entity_boxes[i])) {
				ent_mask &= ~sec_mask;
			}
		}
		ITERATE_END;
	}
	#endif
	//Check for collisions on every leaf
	//NOT MASK
#ifdef LISTS
	for (unsigned leaf = grid_parent_nodes; leaf < grid_nodes; leaf++) {
		std::vector<Handle>& leaf_entities = section_entities[leaf];
		auto size = leaf_entities.size();

		for (unsigned i = 0; i < size; i++) {
			for (unsigned j = i+1; j < size; j++) {
				Handle h1 = leaf_entities[i], h2 = leaf_entities[j];
				if(CollisionTag::check_pair(mWorld.vec_CollisionTag[h1].tag, mWorld.vec_CollisionTag[h1].tag)){
					const auto& box1 = entity_boxes[h1], box2 = entity_boxes[h2];
					bool col = check_collision(box1,box2);
					if (col) {
						collide(h1, h2);
					}
				}
			}
		}
	}
#endif
	

#ifdef MASKS
	ITERATE_START
	for (Handle j = i + 1; j < max_entities; j++) {
		if (entities[j] 
			&& 
			(entity_masks[i] & entity_masks[j]).any()
			&&
			check_collision(entity_boxes[j],entity_boxes[i])) {
			//#define box1 entity_boxes[j]
			//#define box2 entity_boxes[i]
			//std::cout << "(" << box1.offset.x << "," << box1.offset.y << ") (" << box2.offset.x << "," << box2.offset.y << ")" << std::endl;
			//#undef box1
			//#undef box2
			//DO STUFF
			collide(i, j);
			
		}
	}
	ITERATE_END;
#endif
}




bool inline CollisionSystem::check_collision(Handle h1, Handle h2) const{
	
	const CollisionBox& box1 = mWorld.vec_CollisionBox[h1];
	const CollisionBox& box2 = mWorld.vec_CollisionBox[h2];
	sf::Vector2f&& pos1 = mWorld.vec_Position[h1].getPosition() + box1.offset;
	sf::Vector2f&& pos2 = mWorld.vec_Position[h2].getPosition() + box2.offset;

	return
		!(pos1.x > pos2.x + box2.size.x //leftmost side to the right of rightmost side
			||
			pos1.x + box1.size.x < pos2.x //rightmost side to the left of leftmost side
			||
			pos1.y > pos2.y + box2.size.y //upper side below lower side
			||
			pos1.y + box1.size.y < pos2.y); //lower side above upper side
}


bool inline CollisionSystem::check_collision(const CollisionBox& b1,const CollisionBox& b2){
	#define vec(vec) "("<<vec.x<<","<<vec.y<<")"
	//std::cout << "checking " << vec(b1.offset) << " of size " << vec(b1.size) << " against " << vec(b2.offset) << " of size " << vec(b2.size) << std::endl;
	#undef vec

	return
		!(b1.offset.x > b2.offset.x + b2.size.x //leftmost side to the right of rightmost side
			||
			b1.offset.x + b1.size.x < b2.offset.x //rightmost side to the left of leftmost side
			||
			b1.offset.y > b2.offset.y + b2.size.y //upper side below lower side
			||
			b1.offset.y + b1.size.y < b2.offset.y); //lower side above upper side
}


void CollisionSystem::translate_boxes_to_global(){
	ITERATE_START
	const CollisionBox& box= mWorld.vec_CollisionBox[i];
	sf::Vector2f&& p1 = mWorld.vec_Position[i].getPosition() + box.offset;
	sf::Vector2f&& p2 = sf::Vector2f(box.size);
	entity_boxes[i] = CollisionBox(std::move(p1), std::move(p2));
	ITERATE_END
}

inline void CollisionSystem::collide(Handle h1, Handle h2){
	Collision col(h1, h2);
	//if the collision wasn't already accounted for
	if (!collision_matrix[col.entities.first][col.entities.second]++) {
		mWorld.collision_queue.push(std::move(col));
	}
}
