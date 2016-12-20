#include "Game.h"
#include "vec_magn.h"

#include <math.h>
#include<iostream>
Game::Game()
	: mWindow(sf::VideoMode(600, 600), "mi juegazo", sf::Style::Default)
	, mCirculo(30, 30)
	, direction(0, 0)
{
	mCirculo.setFillColor(sf::Color::Red);
	mCirculo.setOrigin(mCirculo.getRadius(), mCirculo.getRadius());

	mWindow.setKeyRepeatEnabled(false);
	mWindow.setVerticalSyncEnabled(true);

	keys[UP] = sf::Keyboard::Up;
	keys[DOWN] = sf::Keyboard::Down;
	keys[LEFT] = sf::Keyboard::Left;
	keys[RIGHT] = sf::Keyboard::Right;
}

void Game::run() {
	clock.restart();
	sf::Time elapsed_time = sf::Time::Zero;
	sf::Time accum_time = sf::Time::Zero;
	while (mWindow.isOpen()) {
		process_events();
		sf::Time frame_time=clock.restart();
		accum_time += frame_time;
		while (accum_time >= dt) {
			update(dt);
			accum_time -= dt;
			elapsed_time += dt;

			//clear frame dependant data
			memset(just_updated_keys, false, input::size);
		}
		render();
	}
}

void Game::process_events() {
	
	sf::Event event;
	while (mWindow.pollEvent(event)) {
		switch (event.type) {

		case sf::Event::Closed:
			mWindow.close();
			break;
			
			
		case sf::Event::KeyReleased:
		case sf::Event::KeyPressed:
			bool pressed = (event.type == sf::Event::KeyPressed);//released=false, pressed=true
			update_key(event.key.code, pressed);
			break;
			

				
		}
	}

}

void Game::update(const sf::Time& tiempo) {
	direction.x =static_cast<float>(pressed_keys[RIGHT]- pressed_keys[LEFT]);
	direction.y = static_cast<float>(pressed_keys[DOWN] - pressed_keys[UP]);
	if (direction.x || direction.y)
		direction /= vec_magn(direction);
	mCirculo.move(PLAYER_SPEED * tiempo.asSeconds() * direction);
}

void Game::render() {
	mWindow.clear(sf::Color(60,60,60,255));//grey
	mWindow.draw(mCirculo);
	mWindow.display();
}


void Game::update_key(sf::Keyboard::Key key, bool pressed) {
	for (short int i = 0; i < input::size; i++){
		if (key==keys[i]){
			just_updated_keys[i] = true;	
			pressed_keys[i] = pressed;
			
		}
			
	}
	
}