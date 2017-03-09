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
		this->requestStackPush(STATE_GAME);
	});
	playButton->setPosition(200,200);

	GUI::Button::Ptr exitButton(new GUI::Button(*mContext.fonts));
	exitButton->setText("Salir");
	exitButton->setCallback([this](){
		this->requestStackClear();
	});
	exitButton->setPosition(200,300);
	mContainer.pack(playButton);
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
