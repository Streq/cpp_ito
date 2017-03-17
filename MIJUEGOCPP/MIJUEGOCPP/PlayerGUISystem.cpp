#include "PlayerGUISystem.h"
#include "system_defines.h"
#include "components_header.h"
#include "Text_Util.h"
PlayerGUISystem::PlayerGUISystem(World& world, FontHolder& fonts, const sf::Vector2f& pos, const sf::Vector2f& size):
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
	mFullLifeBar(size),
	mStats(),
	mWinLose(),
	mFonts(&fonts),
	mAmount_entities(mWorld.get_amount_entities())
{
	mCurrentLifeBar.setFillColor(Color::Green);
	mCurrentLifeBar.setPosition(pos);
	mFullLifeBar.setFillColor(Color::Red);
	mFullLifeBar.setPosition(pos);
	mStats.setPosition(pos.x, pos.y + 100);
	mStats.setFont(mFonts->get(Font::consola));
	mStats.setCharacterSize(12U);

	mWinLose.setCharacterSize(50U);
	mWinLose.setFont(mFonts->get(Font::consola));
	mWinLose.setPosition(WINDOW_SIZE_X/2.f,WINDOW_SIZE_Y/2.f);
}

void PlayerGUISystem::update(sf::Time t) {
	if(current_player!=MAX_ENTITIES){
		const auto& health = mWorld.vec_Health[current_player];
		mCurrentLifeBar.setScale(static_cast<float>(health.current)/health.max,1.f);
		mWinLose.setString("");
	}else{
		mWinLose.setString("CABIOOOOO");
		centerOrigin(mWinLose);
	}
	mStats.setString(
		std::string("enemies: ")+std::to_string(mAmount_entities[Tag::Enemy])+
		std::string("\nspawners: ")+std::to_string(mAmount_entities[Tag::Spawner])
		);
	if(mAmount_entities[Tag::Enemy]+mAmount_entities[Tag::Spawner]==0){
		mWinLose.setString("GANASTES");
		centerOrigin(mWinLose);
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
		target.draw(mStats);
	}

	target.draw(mWinLose);
}
