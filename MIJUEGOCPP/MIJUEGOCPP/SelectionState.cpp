#include "SelectionState.h"
#include "Button.h"
#include <memory>
SelectionState::SelectionState(GameStateStack & stack, Context cont):
	GameState(stack, cont),
	mContainer()
{
	GUI::Button::Ptr tenista(new GUI::Button(*mContext.fonts));
	tenista->setText("Juani el tenista mágico");
	tenista->setCallback([this](){
		this->mContext.settings->set_player(0,Character::Tennist);
		this->requestStackPop();
		this->requestStackPush(STATE_GAME);
	});
	tenista->setPosition(200,200);
	
	GUI::Button::Ptr time_traveler(new GUI::Button(*mContext.fonts));
	time_traveler->setText("Perez el sorfista espacial");
	time_traveler->setCallback([this](){
		this->mContext.settings->set_player(0,Character::TimeTraveler);
		this->requestStackPop();
		this->requestStackPush(STATE_GAME);
	});
	time_traveler->setPosition(200,250);
	
	GUI::Button::Ptr minotaur(new GUI::Button(*mContext.fonts));
	this->mContext.settings->set_player(0,Character::Minotaur);
	minotaur->setText("Toro el toro");
	minotaur->setCallback([this](){
		this->requestStackPop();
		this->requestStackPush(STATE_GAME);
	});
	minotaur->setPosition(200,300);

	GUI::Button::Ptr giant(new GUI::Button(*mContext.fonts));
	giant->setText("Gordi el gigante");
	giant->setCallback([this](){
		this->mContext.settings->set_player(0,Character::Giant);
		this->requestStackPop();
		this->requestStackPush(STATE_GAME);
	});
	giant->setPosition(200,350);

	
	
	mContainer.pack(tenista);
	mContainer.pack(time_traveler);
	mContainer.pack(minotaur);
	mContainer.pack(giant);

	mBackground.setTexture(mContext.textures->get(Texture::MENU_BACKGROUND));
}

bool SelectionState::handle_event(const sf::Event & e){
	mContainer.handle_event(e);
	return false;
}

bool SelectionState::update(sf::Time tiempo){
	return false;
}

void SelectionState::draw()const{
	mContext.window->draw(mBackground);
	mContext.window->draw(mContainer);
}
