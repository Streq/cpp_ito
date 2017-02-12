#pragma once
#include<SFML/Graphics.hpp>
#include "PlayerInput.h"
#include "World.h"
#include "FontHolder.h"
#include "defines.h"

class Game {
	private:

		sf::RenderWindow			mWindow;
		
		
		sf::Clock					clock;
		PlayerInput					controller[2];
		
		std::vector<ptr<System>>	vec_System;
		World						mWorld;
		
		FontHolder					fonts;
		sf::Text					fps_text;

	private:
		void				process_events();
		void				update(const sf::Time& tiempo);
		void				init();
		void				render();
		void				update_fps(sf::Uint16 fps_count);


	private:
		void				stress_init();
		void				zombie_wave_init(unsigned zombies, unsigned spawners, unsigned walls);
		void				zombie_wave_init_2_players(unsigned zombies, unsigned spawners, unsigned walls);
		void				zombie_rush_init(unsigned zombies, unsigned walls);
		void				player_init();
	public:
		int					run();
							Game();
};
