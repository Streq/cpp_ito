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
}

void RenderingSystem::draw(sf::RenderWindow & window){
	ITERATE_START
	const auto& pos= mWorld.vec_Position[i];
	if(pos.relative_to==MAX_ENTITIES)
		mWindow.draw(*mWorld.vec_Rendering[i].drawable, pos.getTransform());
	else {
		const auto& pos_relative_to = mWorld.vec_Position[pos.relative_to];
		mWindow.draw(*mWorld.vec_Rendering[i].drawable, sf::Transform(pos.getTransform()).translate(pos_relative_to.getPosition()));
	}
	ITERATE_END
}
