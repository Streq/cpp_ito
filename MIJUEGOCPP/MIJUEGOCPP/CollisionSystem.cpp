#include "CollisionSystem.h"
#include "system_defines.h"
#include "vec_magn.h"
#include <iostream>
#include "Game.h"
#include "components_header.h"
#include "clamp.h"
#include <cmath>
#include "EntityClass.h"
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
	section_boxes[0] = CollisionBody(sf::Vector2f(0, 0), mWorld.getSize());
	for (unsigned cur = 0; cur < grid_parent_nodes; cur++) {
		CollisionBody& cur_box = section_boxes[cur];
		sf::Vector2f size = cur_box.size;
		size.x /= grid_x;
		size.y /= grid_y;

		unsigned end = children_end(cur);
		for (unsigned count = 0, child = first_child(cur); child < end; child++,count++) {
			CollisionBody& child_box = section_boxes[child];
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
	float time = t.asSeconds();
	check_collisions(time);
	handle_collisions(time);
}



void CollisionSystem::check_collisions(float time){
	memset(collision_matrix,0,sizeof(CollisionMatrix));
	
	translate_boxes_to_global();
	//Clear entities
	//NOT MASK
	#ifdef LISTS
	for (auto i=0; i < grid_nodes; i++) {
		section_entities[i].clear();
	}
	//Fill main box with colliding entities
	const CollisionBody& main_box = section_boxes[0];
	auto& sec_entities = section_entities[0];
	#endif
	ITERATE_START
	
#ifdef MASKS
	entity_masks[i].reset();
#endif

	
	if (check_collision_box(section_boxes[0],entity_boxes[i])) {
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
				if (check_collision_box(section_boxes[child], entity_boxes[h])) {
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
				if(CollisionInfo::check_pair(mWorld.vec_CollisionInfo[h1].tag, mWorld.vec_CollisionInfo[h2].tag)){
					const auto& box1 = entity_boxes[h1], box2 = entity_boxes[h2];
					bool col = check_collision_box(box1,box2);
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

void CollisionSystem::handle_collisions(float time) {
	auto& cqueue = mWorld.collision_queue;
	while (!cqueue.empty()) {
		const auto& col = cqueue.front();
		Handle h1 = col.entities.first;
		Handle h2 = col.entities.second;
		if (mWorld.vec_CollisionInfo[h1].tag > mWorld.vec_CollisionInfo[h2].tag)std::swap(h1, h2);
		//DO STUFF
		CollisionInfo& t1 = mWorld.vec_CollisionInfo[h1];
		CollisionInfo& t2 = mWorld.vec_CollisionInfo[h2];

		Movement& mov1 = mWorld.vec_Movement[h1];
		Movement& mov2 = mWorld.vec_Movement[h2];

		Position& pos1 = mWorld.vec_Position[h1];
		Position& pos2 = mWorld.vec_Position[h2];

		CollisionBody& col1 = entity_boxes[h1];
		CollisionBody& col2 = entity_boxes[h2];
		switch (t1.tag) {
		case Tag::Wall: {
			if (t2.tag == Tag::Character_Entity || t2.tag == Tag::Projectile) {

				if (t2.on_wall == CollisionInfo::On_Wall::remove) {
					mWorld.remove_entity(h2);
					break;
				}

				mWorld.vec_Owner[h2].owner = h1;


				auto aux_col2 = col2;
				//if no longer colliding (from previous collision resolution) no handling is needed
				if (!check_collision_box(col2, col1)) { break; }
				//get the collision box tf outta the collision zone
				//aux_col2.offset -= mov2.velocity*time;
				aux_col2.offset = pos2.frame_start_position + mWorld.vec_CollisionBody[h2].offset;
				int mov_x = SIGN(mov2.frame_start_velocity.x);
				int mov_y = SIGN(mov2.frame_start_velocity.y);
				//approach horizontally
				if (mov_x) {
					while (aux_col2.offset.x != col2.offset.x) {
						auto prev_frame_x = aux_col2.offset.x;
						aux_col2.offset.x = approach(aux_col2.offset.x, col2.offset.x, 1);
						if (check_collision_box(aux_col2, col1)) {
							mov2.velocity.x = -mov2.velocity.x * (t2.on_wall == CollisionInfo::On_Wall::bounce);

							aux_col2.offset.x = prev_frame_x;
							break;
						}
					}

				}
				//approach vertically
				if (mov_y) {
					while (aux_col2.offset.y != col2.offset.y) {
						auto prev_frame_y = aux_col2.offset.y;
						aux_col2.offset.y = approach(aux_col2.offset.y, col2.offset.y, 1);
						if (check_collision_box(aux_col2, col1)) {
							mov2.velocity.y = -mov2.velocity.y * (t2.on_wall == CollisionInfo::On_Wall::bounce);
							aux_col2.offset.y = prev_frame_y;
							break;
						}
					}
				}

				//if we still fucked up
				//auto dir = normalized(box.offset - col1.offset);
				//while (check_collision_box(box, col1)) {
				//	box.offset += dir;
				//}
				col2.offset = aux_col2.offset;
				pos2.setPosition(aux_col2.offset - mWorld.vec_CollisionBody[h2].offset);


			}
		}break;
		case Tag::Character_Entity: {
			switch (t2.tag) {
			case Tag::Character_Entity: {
				auto dir = normalized(pos2.getPosition() - pos1.getPosition());
				auto vec = dir*time*100.f;
				mov1.velocity -= vec;
				mov2.velocity += vec;
			}break;
			case Tag::Projectile: {
				const auto& own1 = h1;//mWorld.vec_Owner[h1].owner;
				const auto& own2 = mWorld.vec_Owner[h2].owner;
				if (own1 != own2) {
					switch (t1.type) {
					case HitBoxType::Intangible: {
					}break;
					case HitBoxType::Invincible: {
						mWorld.remove_entity(h2);
					}break;
					case HitBoxType::Damageable: {
						mov1.velocity = normalized(mov2.velocity) * t2.knockback;
						mWorld.vec_State[h1].update(States::Hurt);
						mWorld.vec_Health[h1].incoming_damage += t2.damage;
						mWorld.vec_State[h1].duration = t2.stun_time;
						mWorld.remove_entity(h2);

					}break;
					}
				}
			}break;
			case Tag::Hit_Box: {
				if (t2.type == HitBoxType::Hit && t1.type == HitBoxType::Damageable) {
					const auto& own2 = mWorld.vec_Owner[h2].owner;
					if (h1 != own2) {
						auto owner_pos = mWorld.vec_Position[mWorld.vec_Owner[h2].owner];
						mov1.velocity = normalize(pos1.getPosition() - owner_pos.getPosition()) * t2.knockback;
						mWorld.vec_State[h1].update(States::Hurt);
						mWorld.vec_State[h1].duration = t2.stun_time;
						mWorld.vec_Health[h1].incoming_damage += t2.damage;
					}
				}
			}break;
			}
		}break;
		case Tag::Projectile: {
			auto& own1 = mWorld.vec_Owner[h1].owner;
			const auto& own2 = mWorld.vec_Owner[h2].owner;
			if (own1 != own2) {
				switch (t2.tag) {
					case Tag::Hit_Box: {
						switch (t2.type) {
							case HitBoxType::Reflect:{
								own1 = own2;
								mov1.velocity = -mov1.velocity;
							}break;
							case HitBoxType::Absorb: {
								mWorld.remove_entity(h1);
							}break;
						}

					}break;
				}
			}
		}break;
		}

		cqueue.pop();
	}
}


bool inline CollisionSystem::check_collision_box(const CollisionBody& b1,const CollisionBody& b2){
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
	const CollisionBody& box= mWorld.vec_CollisionBody[i];
	
	const auto& pos = mWorld.vec_Position[i];

	const Handle& h_rel = pos.relative_to;

	sf::Vector2f p1(pos.getPosition() + box.offset);
	if(h_rel!=max_entities){
		p1 += mWorld.vec_Position[h_rel].getPosition();
	}

	sf::Vector2f p2 (box.size);

	entity_boxes[i] = CollisionBody(std::move(p1), std::move(p2));
	ITERATE_END
}

inline void CollisionSystem::collide(Handle h1, Handle h2){
	Collision col(h1, h2);
	//if the collision wasn't already accounted for
	if (!collision_matrix[col.entities.first][col.entities.second]++) {
		mWorld.collision_queue.push(std::move(col));
	}
}

inline bool CollisionSystem::check_collision_circle(const CollisionBody& c1, const CollisionBody& c2) {
	auto dist = c1.offset - c2.offset;
	return vec_magn(dist) * 2.f > COLLISION_BODY_DIAMETER(c1) + COLLISION_BODY_DIAMETER(c2);
}

inline bool CollisionSystem::check_collision_circle_box(const CollisionBody& cir, const CollisionBody& box) {
	// clamp(value, min, max) - limits value to the range min..max
	auto cirpos = cir.offset;
	cirpos.x += cir.size.x / 2.f;
	cirpos.y += cir.size.x / 2.f;

	auto recmin = box.offset;
	auto recmax = recmin + box.size;
	
	sf::Vector2f closest_point(
		clamp(cirpos.x, recmin.x, recmax.x),
		clamp(cirpos.y, recmin.y, recmax.y)
	);

	return vec_magn(closest_point - cirpos) * 2.f < cirpos.x;
}
