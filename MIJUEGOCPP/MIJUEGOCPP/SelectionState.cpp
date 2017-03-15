#include "SelectionState.h"
#include "Button.h"
#include <memory>
#include <iostream>
SelectionState::SelectionState(GameStateStack & stack, Context cont):
	GameState(stack, cont),
	mContainer(),
	mCurrentDescription(-1),
	mDescription(cont.fonts->get(Font::consola),std::string(),70,15U)
{
	load_descriptions();
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

	mBackground.setTexture(mContext.textures->get(Texture::MENU_BACKGROUND));
	GUI::Button::Ptr volver(new GUI::Button(*mContext.fonts));
	volver->setText("Volver");
	volver->setCallback([this](){
		this->requestStackPop();
		this->requestStackPush(STATE_MAIN_MENU);
	});
	volver->setPosition(200,400);



	mContainer.pack(tenista);
	mContainer.pack(time_traveler);
	mContainer.pack(minotaur);
	mContainer.pack(giant);
	mContainer.pack(volver);
	
	mDescription.setPosition(350,200);

}

bool SelectionState::handle_event(const sf::Event & e){
	mContainer.handle_event(e);
	return false;
}

bool SelectionState::update(sf::Time tiempo){
	auto ind=mContainer.getIndex();
	if(mCurrentDescription != ind){
		mCurrentDescription = ind;
		if(mCurrentDescription >= 0 && mCurrentDescription < Character::Playables){
			mDescription.setText(mDescriptions[mCurrentDescription]);
		}
		else mDescription.setText(std::string());
		
	};

	return false;
}

void SelectionState::draw()const{
	mContext.window->draw(mBackground);
	mContext.window->draw(mContainer);
	mContext.window->draw(mDescription);
}

void SelectionState::load_descriptions(){
	const std::string& descriptions=mContext.texts->get(TextFile::CharacterAttacks);
	size_t begin;
	size_t end;
	std::string keyword;

	#define load_attacks(ch) \
	keyword=#ch+std::string(":");\
	begin=descriptions.find(keyword)+keyword.size();\
	end=descriptions.find("///",begin);\
	mDescriptions[Character::##ch]=descriptions.substr(begin,end-begin);
	
	#define info_attacks \
	//std::cout<<"keyword:"<<keyword<<std::endl<<"begin:"<<begin<<std::endl<<"end:"<<end<<std::endl;
	load_attacks(Minotaur);
	//info_attacks;
	load_attacks(Tennist);
	//info_attacks;
	load_attacks(Giant);
	//info_attacks;
	load_attacks(TimeTraveler);
	//info_attacks;
	//for(const auto& str : mDescriptions){
	//	std::cout<<str<<std::endl;
	//}

}
