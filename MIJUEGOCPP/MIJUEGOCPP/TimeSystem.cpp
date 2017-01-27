#include "TimeSystem.h"
#include "World.h"
TimeSystem::TimeSystem(World& mWorld) :
	System
		(mWorld
		,Requeriments
			(Flagset(
				#define X(C) flag::##C |
				COMPONENTS_TIME
				#undef X
				flag::none)
			,Flagset(flag::none)
		)
	)
{

}

void TimeSystem::update(sf::Time dt) {
	ITERATE_START
	auto& Timer=mWorld.vec_TimeSpan[i];
	Timer.time -= dt;
	if (Timer.time < sf::Time::Zero) {
		mWorld.remove_entity(i);
	}
	ITERATE_END
}