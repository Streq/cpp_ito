#pragma once
#include<SFML/Graphics.hpp>
#include "PlayerInput.h"
#include "World.h"
#include "FontHolder.h"
#include "defines.h"
#include "GameState.h"
#include "RenderingSystem.h"
class Game : public GameState{
	public:
		//int					run();
		Game(GameStateStack& s, Context c);
	private:
		std::vector<ptr<System>>	vec_System;
		World						mWorld;
		RenderingSystem*			mRenderSystem;
		
	private:
		virtual bool		update(sf::Time tiempo);
		virtual void		draw() const;
		void				init();
		virtual bool		handle_event(const sf::Event& e);

	private:
		void				stress_init();
		void				zombie_wave_init(unsigned zombies, unsigned spawners, unsigned walls);
		void				zombie_wave_init_2_players(unsigned zombies, unsigned spawners, unsigned walls);
		void				zombie_rush_init(unsigned zombies, unsigned walls);
		void				player_init();
	
};
