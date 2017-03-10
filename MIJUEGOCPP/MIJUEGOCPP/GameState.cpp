#include "GameState.h"
#include "GameStateStack.h"

GameState::GameState(GameStateStack& s, Context c):
	mContext(c),
	mStack(&s)
{}

void GameState::requestStackPush(GameState::ID stateID){
	mStack->push_state(stateID);
}

void GameState::requestStackPop(){
	mStack->pop_state();
}

void GameState::requestStackClear(){
	mStack->clear_stack();
}

GameState::Context GameState::getContext()const{
	return mContext;
}

GameState::Context::Context(sf::RenderWindow & window, FontHolder& fonts, TextureHolder& textures, std::array<PlayerInput, 2>& controllers, Settings& settings):
	window(&window),
	fonts(&fonts),
	textures(&textures),
	controllers(&controllers),
	settings(&settings)
{
}
