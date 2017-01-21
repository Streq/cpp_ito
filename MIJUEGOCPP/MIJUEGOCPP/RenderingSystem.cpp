#include "RenderingSystem.h"
#include "system_defines.h"
RenderingSystem::RenderingSystem(World& world, sf::RenderTarget& window)
	:System(
		world,
		Requeriments(
			Flagset(
				#define X(C) flag::##C |
				COMPONENTS_RENDERING
				#undef X
				flag::none
				),
			Flagset(flag::none)
		)
	),
	mWindow(window)
{}

/*
void RenderingSystem::update(sf::Time t) {
	//mWindow.clear(sf::Color(rand(), rand(), rand()));//jaja
	mWindow.clear(sf::Color::Black);
	ITERATE_START
		Rendering& r = mWorld.vec_Rendering[i];
		Position& p = mWorld.vec_Position[i];
		r.setPosition(p.position);
		r.setRotation(p.angle);
		mWindow.draw(*r.drawable, r.getTransform());
	ITERATE_END
}
*/
/*
void RenderingSystem::update(sf::Time t) {
	//mWindow.clear(sf::Color(rand(), rand(), rand()));//jaja
	mWindow.clear(sf::Color::Black);
	update_transform();
	draw();
}
*/
void RenderingSystem::update(sf::Time t) {
	mWindow.clear(sf::Color::Black);
	ITERATE_START
		mWindow.draw(*mWorld.vec_Rendering[i].drawable, mWorld.vec_Position[i].getTransform());
	ITERATE_END
}