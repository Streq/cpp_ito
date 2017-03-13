#pragma once
#include "GameState.h"
#include "Container.h"
#include "TextBox.h"
class SelectionState : public GameState{
	public:
	SelectionState(GameStateStack& stack, Context cont);
	virtual bool										handle_event(const sf::Event& e);
	virtual bool										update(sf::Time tiempo);
	virtual void										draw()const;
	void												load_descriptions();
	private:

	GUI::Container										mContainer;
	std::array<std::string, Character::Playables>		mDescriptions;
	int													mCurrentDescription;
	//sf::Text											mDescription;
	TextBox												mDescription;
	sf::Sprite											mBackground;
	
};
