#include "MovementSystem.h"
#include "system_defines.h"
#include "components_header.h"
#include "vec_magn.h"
#include "clamp.h"
#include <cmath>
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
		
		if (m.friction > 0) {
			sf::Vector2f fric = normalized(m.velocity) * m.friction * time;
			m.velocity.x = approach(m.velocity.x, 0.f, abs(fric.x));
			m.velocity.y = approach(m.velocity.y, 0.f, abs(fric.y));
		}

		if (m.capped) {
			sf::Vector2f max = normalized(m.velocity) * m.maxspeed;
			m.velocity.x = clamp(m.velocity.x, -abs(max.x), abs(max.x));
			m.velocity.y = clamp(m.velocity.y, -abs(max.y), abs(max.y));
		}

		m.frame_start_velocity = m.velocity;
		p.frame_start_position = p.getPosition();
		p.move(m.velocity * time);
		p.rotate(m.rotation * time);

	ITERATE_END
}
