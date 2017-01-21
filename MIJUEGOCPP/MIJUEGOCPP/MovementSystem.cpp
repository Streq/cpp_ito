#include "MovementSystem.h"
#include "system_defines.h"
MovementSystem::MovementSystem(World& world)
	:System(
		world,
		Requeriments(
			Flagset(
				#define X(C) flag::##C |
				COMPONENTS_MOVEMENT
				#undef X
				flag::none
				),
			Flagset(flag::none)
			)
		)
{}

void MovementSystem::update(sf::Time t) {
	float time = t.asSeconds();
	ITERATE_START
		Movement& m = mWorld.vec_Movement[i];
		Position& p = mWorld.vec_Position[i];
		m.velocity += m.acceleration * time;
		p.move(m.velocity * time);
		p.rotate(m.rotation * time);
	ITERATE_END
}
