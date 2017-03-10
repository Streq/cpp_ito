#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "GameStateStack.h"
#include "FontHolder.h"
#include "TextureHolder.h"
#include "PlayerInput.h"
#include <array>
#include "defines.h"
class Application{
	public:
		void								run();
											Application();
	private:
		void								handle_events();
		void								update(sf::Time dt);
		void								render();


		void								register_states();
		
		
		sf::Clock							mClock;
		sf::Text							mFps_text;
		sf::RenderWindow					mWindow;
		GameStateStack						mStateStack;
		FontHolder							mFonts;
		TextureHolder						mTextures;
		std::array<PlayerInput,MAX_PLAYERS>	mControllers;
		Settings							mSettings;
};