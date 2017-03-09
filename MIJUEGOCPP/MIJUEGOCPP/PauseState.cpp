#include "PauseState.h"
#include "Button.h"
#include "defines.h"
PauseState::PauseState(GameStateStack & stack, Context cont):
	GameState(stack,cont)
{
	
	GUI::Button::Ptr reanudar (new GUI::Button(*mContext.fonts));
	reanudar->setText("Reanudar");
	reanudar->setPosition(WINDOW_SIZE_X / 2.f - 40.f, WINDOW_SIZE_Y / 2.f - 100.f);
	reanudar->setCallback([this](){
		requestStackPop();
	});

	GUI::Button::Ptr volver_menu (new GUI::Button(*mContext.fonts));
	volver_menu->setText("Volver al menu principal");
	
	volver_menu->setPosition(WINDOW_SIZE_X / 2.f - 40.f, WINDOW_SIZE_Y / 2.f - 50);
	volver_menu->setCallback([this](){
		requestStackClear();
		requestStackPush(GameState::STATE_MAIN_MENU);
	});
	
	mContainer.pack(reanudar);
	mContainer.pack(volver_menu);
}

bool PauseState::handle_event(const sf::Event & e){
	switch(e.type){
		case sf::Event::KeyReleased:
			switch(e.key.code){
				case sf::Keyboard::Escape:
					requestStackPop();
				break;
			}
		break;
	}
	mContainer.handle_event(e);
	return false;
}

bool PauseState::update(sf::Time tiempo){

	return false;
}

void PauseState::draw() const{
	mContext.window->draw(mContainer);
}
