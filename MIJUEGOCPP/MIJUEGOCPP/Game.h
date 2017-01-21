#pragma once
extern long long colisiones;
#include<SFML/Graphics.hpp>
#include "PlayerInput.h"
#include "World.h"
#include "FontHolder.h"
#include "defines.h"

class Game {
	private:

		sf::RenderWindow			mWindow;
		
		
		sf::Clock					clock;
		PlayerInput					controller;
		
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



		void				stress_init();
		void				collision_init();
	public:
		int					run();
							Game();
};
