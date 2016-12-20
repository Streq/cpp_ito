#pragma once
#include<SFML/Graphics.hpp>

const float PLAYER_SPEED = 200.0f;
const unsigned FPS = 60;
const sf::Time dt = sf::seconds(1.0f / FPS);

class Game {
	private:
		enum input { UP, DOWN, LEFT, RIGHT, size };

		
		
		sf::CircleShape		mCirculo;
		sf::RenderWindow	mWindow;
		sf::Vector2f		direction;
		
		bool				pressed_keys[input::size];		
		bool				just_updated_keys[input::size];

		sf::Keyboard::Key	keys[input::size];
		
		sf::Clock			clock;
		

	private:
		void				process_events();
		void				update(const sf::Time& tiempo);
		void				render();
		void				update_key(sf::Keyboard::Key key, bool pressed);

	public:
		void				run();
							Game();
};
