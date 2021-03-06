#pragma once
#include "GameState.h"
#include "Container.h"
class MenuState : public GameState{
	public:
		MenuState(GameStateStack& stack, Context cont);
		virtual bool				handle_event(const sf::Event& e);
		virtual bool				update(sf::Time tiempo);
		virtual void				draw()const;

	private:
		GUI::Container	mContainer;
		sf::Sprite		mBackground;
};
