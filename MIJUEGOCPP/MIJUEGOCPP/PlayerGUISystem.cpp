#include "PlayerGUISystem.h"
#include "system_defines.h"
#include "components_header.h"
PlayerGUISystem::PlayerGUISystem(World& world,const sf::Vector2f& pos, const sf::Vector2f& size):
	System(
		world,
		Requeriments(
			Flagset(
				#define X(C) flag::##C |
				COMPONENTS_PLAYER_GUI
				#undef X
				flag::none
				),
			Flagset(flag::none)
			)
		),
	mCurrentLifeBar(size),
	mFullLifeBar(size)
{
	mCurrentLifeBar.setFillColor(Color::Green);
	mCurrentLifeBar.setPosition(pos);
	mFullLifeBar.setFillColor(Color::Red);
	mFullLifeBar.setPosition(pos);
}

void PlayerGUISystem::update(sf::Time t) {
	if(current_player!=MAX_ENTITIES){
		const auto& health = mWorld.vec_Health[current_player];
		mCurrentLifeBar.setScale(static_cast<float>(health.current)/health.max,1.f);
	}
}




void PlayerGUISystem::update_entity(const Flagset &fl, Handle h){
	
	if(current_player!=MAX_ENTITIES && current_player!=h)
		return;
	
	current_player = (TESTMASK(fl, requeriments.needed) && TESTMASK(~requeriments.rejected, fl)) ? h : MAX_ENTITIES;
	return;
}

Handle PlayerGUISystem::getPlayer()const{
	return current_player;
}

void PlayerGUISystem::draw(sf::RenderTarget & target, sf::RenderStates states) const{
	if(current_player!=MAX_ENTITIES){
		target.draw(mFullLifeBar);
		target.draw(mCurrentLifeBar);
	}
}
