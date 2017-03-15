#include "HowToPlayState.h"
#include "Button.h"
HowToPlayState::HowToPlayState(GameStateStack & stack, Context cont):
	GameState(stack,cont),
	mText(cont.fonts->get(Font::consola),cont.texts->get(TextFile::HowToPlay),70,15U),
	mBackground(mContext.textures->get(Texture::MENU_BACKGROUND))

{
	GUI::Button::Ptr playButton(new GUI::Button(*mContext.fonts));
	playButton->setText("Volver al menú");
	playButton->setCallback([this](){
		this->requestStackPop();
		this->requestStackPush(STATE_MAIN_MENU);
	});
	playButton->setPosition(200,200);

	mContainer.pack(playButton);

	mText.setPosition(350,200);

}

bool HowToPlayState::handle_event(const sf::Event & e){
	mContainer.handle_event(e);
	return false;
}

bool HowToPlayState::update(sf::Time tiempo){
	return false;
}

void HowToPlayState::draw() const{
	mContext.window->draw(mBackground);
	mContext.window->draw(mContainer);
	mContext.window->draw(mText);
}