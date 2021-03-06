#include "MenuState.h"
#include "Button.h"
#include <memory>
MenuState::MenuState(GameStateStack & stack, Context cont):
	GameState(stack,cont),
	mContainer()
{
	GUI::Button::Ptr playButton(new GUI::Button(*mContext.fonts));
	playButton->setText("Jugar");
	playButton->setCallback([this](){
		this->requestStackPop();
		this->requestStackPush(STATE_CHARACTER_SELECT);
	});
	playButton->setPosition(200,200);
	mContainer.pack(playButton);

	GUI::Button::Ptr howToPlay(new GUI::Button(*mContext.fonts));
	howToPlay->setText("C�mo jugar");
	howToPlay->setCallback([this](){
		this->requestStackPop();
		this->requestStackPush(STATE_HOW_TO_PLAY);
	});
	howToPlay->setPosition(200,250);
	mContainer.pack(howToPlay);

	GUI::Button::Ptr credits(new GUI::Button(*mContext.fonts));
	credits->setText("Cr�ditos");
	credits->setCallback([this](){
		this->requestStackPop();
		this->requestStackPush(STATE_CREDITS);

	});
	credits->setPosition(200,300);
	mContainer.pack(credits);



	GUI::Button::Ptr exitButton(new GUI::Button(*mContext.fonts));
	exitButton->setText("Salir");
	exitButton->setCallback([this](){
		this->requestStackClear();
	});
	exitButton->setPosition(200,350);
	mContainer.pack(exitButton);
	
	mBackground.setTexture(mContext.textures->get(Texture::MENU_BACKGROUND));
}

bool MenuState::handle_event(const sf::Event & e){
	mContainer.handle_event(e);
	return false;
}

bool MenuState::update(sf::Time tiempo){
	return false;
}

void MenuState::draw()const{
	mContext.window->draw(mBackground);
	mContext.window->draw(mContainer);
}
