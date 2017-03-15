#pragma once
#include "GameState.h"
#include "Container.h"
#include "TextBox.h"
class CreditsState : public GameState{

	public:
								CreditsState(GameStateStack& stack, Context cont);
	virtual bool				handle_event(const sf::Event& e);
	virtual bool				update(sf::Time tiempo);
	virtual void				draw()const;

	private:
	GUI::Container	mContainer;
	TextBox			mText;
	sf::Sprite		mBackground;


};