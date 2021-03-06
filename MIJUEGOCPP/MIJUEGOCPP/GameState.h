#pragma once
#include <SFML/System.hpp>
#include <SFML/System.hpp>
#include "FontHolder.h"
#include "TextureHolder.h"
#include "TextHolder.h"
#include <memory>
#include "Settings.h"
#include "PlayerInput.h"
class GameStateStack;
class GameState{
	public:
		typedef std::unique_ptr<GameState> Ptr;

		enum ID{
			STATE_NONE,
			STATE_MAIN_MENU,
			STATE_SETTINGS,
			STATE_GAME_MODE,
			STATE_GAME,
			STATE_CHARACTER_SELECT,
			STATE_HOW_TO_PLAY,
			STATE_CREDITS,
			STATE_PAUSE
		};


		struct Context
		{
			Context(sf::RenderWindow& window, FontHolder& fonts, TextureHolder& textures, TextHolder& texts, std::array<PlayerInput,2>& controllers, Settings& settings);

			sf::RenderWindow*			window;
			FontHolder*					fonts;
			TextureHolder*				textures;
			TextHolder*					texts;
			std::array<PlayerInput,2>*	controllers;
			Settings*					settings;
		};
		GameState(GameStateStack& s,Context c);


		virtual bool				handle_event(const sf::Event& e) = 0;
		virtual bool				update(sf::Time tiempo) = 0;
		virtual void				draw()const = 0;


	protected:
		void						requestStackPush(GameState::ID stateID);
		void						requestStackPop();
		void						requestStackClear();
		Context						getContext() const;

	protected:
		Context						mContext;
		GameStateStack*				mStack;
		
};