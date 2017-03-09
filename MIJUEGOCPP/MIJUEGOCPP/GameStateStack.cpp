#include "GameStateStack.h"
GameStateStack::Request::Request(Action action, GameState::ID state):
	action(action),
	state(state)
{}

GameStateStack::GameStateStack(GameState::Context mContext):
	mContext(mContext)
{}

bool GameStateStack::is_empty() const{
	return mStack.empty();
}

void GameStateStack::update(sf::Time dt){
	for(auto itr=mStack.rbegin(); itr!=mStack.rend(); itr++){
		if(!(*itr)->update(dt)) break;
	}
	apply_changes();
}

void GameStateStack::handle_event(const sf::Event & e){
	for(auto itr=mStack.rbegin(); itr!=mStack.rend(); itr++){
		if(!(*itr)->handle_event(e)) break;
	}
}

void GameStateStack::draw() const{
	for(auto itr=mStack.rbegin(); itr!=mStack.rend(); itr++){
		(*itr)->draw();
	}
}

void GameStateStack::push_state(GameState::ID state){
	mRequests.push_back(Request(Request::Push, state));
}

void GameStateStack::pop_state(){
	mRequests.push_back(Request(Request::Pop));
}

void GameStateStack::clear_stack(){
	mRequests.push_back(Request(Request::Clear));
}

void GameStateStack::apply_changes(){
	for(auto itr = mRequests.begin(); itr != mRequests.end(); itr++){
		switch(itr->action){
			case Request::Push:
				mStack.push_back(mFactories[itr->state]());
			break;

			case Request::Pop:
				mStack.pop_back();
			break;

			case Request::Clear:
				mStack.clear();
			break;
		}
	}
	mRequests.clear();
}

