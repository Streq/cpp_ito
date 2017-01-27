#include "HealthSystem.h"


HealthSystem::HealthSystem(World& mWorld) :
	System(
		mWorld,
		Requeriments(
			Flagset(
				#define X(C) flag::##C |
				COMPONENTS_HEALTH
				#undef X
				flag::none),
			Flagset(flag::none)
		)
	)
{
}

void HealthSystem::update(sf::Time){
	ITERATE_START
	auto& health = mWorld.vec_Health[i];
	if (!health.invulnerable) {
		health.current -= health.incoming_damage;
	}
	health.incoming_damage = 0;
	if (health.current <= 0) {
		mWorld.remove_entity(i);
	}
	ITERATE_END
}

