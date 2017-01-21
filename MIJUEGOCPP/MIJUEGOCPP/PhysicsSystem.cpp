#include "PhysicsSystem.h"
#include "system_defines.h"

PhysicsSystem::PhysicsSystem(World& world)
	:System(
		world,
		Requeriments(
			Flagset(
				#define X(C) flag::##C |
				COMPONENTS_PHYSICS
				#undef X
				flag::none
				),
			Flagset(flag::none)
			)
		)
{}

void PhysicsSystem::update(sf::Time){
	auto& cqueue = mWorld.collision_queue;
	while (!cqueue.empty()) {
		const auto& col = cqueue.front();
		Handle h1 = col.entities.first;
		Handle h2 = col.entities.second;
		//DO STUFF
		Movement& m1 = mWorld.vec_Movement[h1];
		Movement& m2 = mWorld.vec_Movement[h2];
		Position& p1 = mWorld.vec_Position[h1];
		Position& p2 = mWorld.vec_Position[h2];
		//m1.velocity = normalize(p1.getPosition() - p2.getPosition())*15.f;
		//m2.velocity = -m1.velocity;
		std::swap(p1, p2);
		cqueue.pop();
	}
}