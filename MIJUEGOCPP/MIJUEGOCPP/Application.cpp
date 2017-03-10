#include "Application.h"
#include "defines.h"
#include "MenuState.h"
#include "PauseState.h"
#include "SelectionState.h"
#include "Game.h"
#include "Color.h"
Application::Application():
	mWindow(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "mi juegazo", sf::Style::Default),
	mStateStack(GameState::Context(mWindow,mFonts,mTextures,mControllers,mSettings)),
	mSettings()
{

	mFonts.load(Font::arial, "Resources/arial.ttf");

	mTextures.load(Texture::MENU_BACKGROUND, "Resources/start_screen.png");
	mFps_text.setCharacterSize(15u);
	mFps_text.setFont(mFonts.get(Font::arial));
	mFps_text.setString("fps:");
	mFps_text.setFillColor(sf::Color::White);

	mWindow.setKeyRepeatEnabled(false);
	mWindow.setVerticalSyncEnabled(true);

	register_states();

	mStateStack.push_state(GameState::STATE_MAIN_MENU);
	mStateStack.apply_changes();

	#define W(player, action, input) mControllers[player].set_key(input_data(PlayerInput::Key::##input,input_data::keyboard),Input::##action);
		W(0,up, Up);
		W(0,down, Down);
		W(0,left, Left);
		W(0,right, Right);
		W(0,skill1, Z);
		W(0,skill2, X);
		W(0,skill3, C);
		W(0,skill4, V);

		W(1, up, W);
		W(1, down, S);
		W(1, left, A);
		W(1, right, D);
		W(1, skill1, Q);
		W(1, skill2, Q);
		W(1, skill3, Q);
		W(1, skill4, Q);
	#undef W

	mSettings.set_players(1);
}

void Application::handle_events(){
	sf::Event e;
	while(mWindow.pollEvent(e)){
		switch(e.type){
			case sf::Event::Closed:{
				mWindow.close();
				return;
			}
		}
		mStateStack.handle_event(e);
	}
	
}

void Application::update(sf::Time dt){
	mStateStack.update(dt);
}

void Application::render(){
	mWindow.clear();
	mStateStack.draw();
	mWindow.draw(mFps_text);
	mWindow.display();
}

void Application::register_states(){
	mStateStack.register_state<MenuState>(GameState::STATE_MAIN_MENU);
	mStateStack.register_state<Game>(GameState::STATE_GAME);
	mStateStack.register_state<PauseState>(GameState::STATE_PAUSE);
	mStateStack.register_state<SelectionState>(GameState::STATE_CHARACTER_SELECT);

}

void Application::run(){
	
	unsigned real_frames = 0;
	unsigned ideal_frames = 0;
	bool frame_updated;

	sf::Time elapsed_time = sf::Time::Zero;
	sf::Time delta_time = sf::Time::Zero;
	
	mClock.restart();
	while (mWindow.isOpen()&&!mStateStack.is_empty()){
		delta_time += mClock.restart();
		frame_updated = (delta_time >= dt);
		real_frames += frame_updated;

		while(delta_time >= dt){
			ideal_frames++;
			if(ideal_frames == FPS){
				mFps_text.setString("fps:" + std::to_string(real_frames));
				ideal_frames = 0;
				real_frames = 0;
			}
			delta_time -= dt;
			elapsed_time += dt;
			handle_events();
			
			update(dt);
			
			for(auto& controller:mControllers){
				controller.clear_updated();
			}
		}
		if(frame_updated){
			render();
		}
	}
}