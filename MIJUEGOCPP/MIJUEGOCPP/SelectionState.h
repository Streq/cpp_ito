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
	void												set_textures();
	private:

	GUI::Container										mContainer;
	std::array<std::string, Character::Playables>		mDescriptions;
	int													mCurrentDescription;
	TextBox												mDescription;
	sf::Sprite											mBackground;
	sf::Sprite											mCurrentCharacter;
	std::array<sf::Texture*, Character::Playables>		mCharacterTextures;
};
