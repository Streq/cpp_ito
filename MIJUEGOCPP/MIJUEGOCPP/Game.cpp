#include "Game.h"
#include "vec_magn.h"
#include <math.h>
#include <iostream>
#include "System_includes.h"
#include "components_header.h"
extern long long colisiones = 0;
Game::Game()
: mWindow(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "mi juegazo", sf::Style::Default)
, vec_System()
, mWorld(this->vec_System,sf::Vector2f(WINDOW_SIZE_X,WINDOW_SIZE_Y))	
{	
	srand(time(NULL));
	fonts.load(Font::arial, "Resources/arial.ttf");

	vec_System.push_back(ptr<System>(new TimeSystem(this->mWorld)));
	vec_System.push_back(ptr<System>(new ControllerSystem(this->mWorld, this->controller)));
	vec_System.push_back(ptr<System>(new StateSystem(this->mWorld)));
	vec_System.push_back(ptr<System>(new MovementSystem(this->mWorld)));
	vec_System.push_back(ptr<System>(new CollisionSystem(this->mWorld)));
	vec_System.push_back(ptr<System>(new HealthSystem(this->mWorld)));
	vec_System.push_back(ptr<System>(new RenderingSystem(this->mWorld, this->mWindow)));
	
	mWindow.setKeyRepeatEnabled(false);
	mWindow.setVerticalSyncEnabled(true);
	
}

int Game::run() {
	init();
	clock.restart();
	sf::Time elapsed_time = sf::Time::Zero;
	sf::Time accum_time = sf::Time::Zero;
	sf::Time fps_timer = sf::Time::Zero;
	uint16_t frame_count = 0;
	
	
	while (mWindow.isOpen()) {
		process_events();
		sf::Time frame_time=clock.restart();
		accum_time += frame_time;
		bool hubo_frame = false;
		while (accum_time >= dt) {
			hubo_frame = true;
			update(dt);
			accum_time -= dt;
			elapsed_time += dt;
			fps_timer += dt;
			controller.clear_updated();
		}
		if (fps_timer >= fps_update_time) {
			update_fps(frame_count);
			frame_count=0;
			fps_timer -= fps_update_time;
		}
		frame_count+=hubo_frame;
		if (hubo_frame) {
			mWindow.draw(fps_text);
			render();
		}
	}

	return 0;
}

inline void Game::update_fps(sf::Uint16 fps_count) {
	fps_text.setString("fps:" + std::to_string(fps_count));
}

void Game::process_events() {

	sf::Event event;
	while (mWindow.pollEvent(event)) {
		switch (event.type) {

		case sf::Event::Closed:
			mWindow.close();
			break;

		case sf::Event::KeyReleased:
			if (event.key.code == sf::Keyboard::R) {
				mWorld.clear();
				zombie_wave_init(10,5,50);
				//this->zombie_rush_init(2000, 50);
				break;
			}
			controller.update_key(event.key.code, false);
			break;

		case sf::Event::KeyPressed:
			controller.update_key(event.key.code, true);
			break;	
		}
	}

}

void Game::update(const sf::Time& dt) {
	
	for (ptr<System>& sys : vec_System) {
		sys->update(dt);
	}
}

void Game::render() {
	mWindow.display();
}

void Game::init() {
	#define W(action,input) controller.set_key(PlayerInput::Key::##input,Input::##action);
	W(up, Up)
	W(down, Down)
	W(left, Left)
	W(right, Right)
	W(shoot, Z)
	W(melee, X)
	W(duck, C)
	W(teleport, V)
	#undef W
	CollisionTag::init_matrix();
	fps_text=sf::Text(sf::String("fps"), fonts.get(Font::arial), 15u);

	//Entity initialization
	zombie_wave_init(10,5,50);
	//zombie_rush_init(100, 50);
}

void Game::stress_init() {

	for (size_t i = 0; i < max_entities; i++)
	{
		Handle h = mWorld.new_entity();
		Rendering *r;
		if (r = mWorld.add_component<Rendering>(h)) {
			//sf::RectangleShape *c = new sf::RectangleShape(sf::Vector2f(10.f,100.f));
			sf::Text *c = new sf::Text("________HABIA UNA VEZ UN BARCO CHIQUITO QUE VIVIA EN PEHUAJO PERO UN DIA SE MARCHO NADIE SUPO BIEN POR QUE_______", fonts.get(Font::arial), 15u);
			c->setFillColor(sf::Color::Blue);//(rand(), rand(), rand()));
			//auto& rect=c->getSize();
			auto& rect = c->getGlobalBounds();
			c->setOrigin(sf::Vector2f(rect.width + rect.left, rect.height + rect.top) / 2.f);
			//c->setOrigin(sf::Vector2f(rect.x, rect.y) / 2.f);
			r->drawable.reset(c);
		}
		Position *p;
		if (p = mWorld.add_component<Position>(h)) {
			p->setPosition(sf::Vector2f(static_cast<float>((rand() - rand()) % int(mWindow.getSize().x * 1.2)), static_cast<float>((rand() - rand()) % int(mWindow.getSize().y * 1.2))));
		}
		Movement *m;
		if (m = mWorld.add_component<Movement>(h)) {
			m->rotation = 120.f;
			m->velocity = normalize(sf::Vector2f(float(rand() - rand()),float(rand() - rand())));
			m->velocity *= 0.f;
		}
		mWorld.add_component<Controller>(h);
	}
}





void Game::zombie_wave_init(unsigned zombies, unsigned spawners, unsigned walls) {
	
	mWorld.make_player(sf::Vector2f(WINDOW_SIZE_X / 2, WINDOW_SIZE_Y / 2));
	for (int j = 0; j < spawners; j++)
		mWorld.make_spawner(
			sf::Vector2f(
				50.f + (rand() % (WINDOW_SIZE_X - 100)),
				50.f + (rand() % (WINDOW_SIZE_Y - 100))),
			sf::seconds(5.f))
		; 
	mWorld.make_wall
		( sf::Vector2f(0, 0)
		, sf::Vector2f(WINDOW_SIZE_X, 25));
	mWorld.make_wall
		( sf::Vector2f(0, WINDOW_SIZE_Y-25)
		, sf::Vector2f(WINDOW_SIZE_X, 25));
	mWorld.make_wall
		( sf::Vector2f(0, 0)
		, sf::Vector2f(25, WINDOW_SIZE_Y));
	mWorld.make_wall
		( sf::Vector2f(WINDOW_SIZE_X-25, 0)
		, sf::Vector2f(25, WINDOW_SIZE_Y));
	for (int i = 0; i < walls; i++) {
		mWorld.make_wall
			( sf::Vector2f(rand() % WINDOW_SIZE_X, rand() % WINDOW_SIZE_Y)
			, sf::Vector2f(25.f, 25.f));

	}
	for (int i = 0; i < zombies; i++) {
		mWorld.make_enemy(sf::Vector2f(50.f + (rand() % (WINDOW_SIZE_X - 100)) , 50.f + (rand() % (WINDOW_SIZE_Y - 100))));
	}
}


void Game::zombie_rush_init(unsigned zombies, unsigned walls) {

	mWorld.make_player(sf::Vector2f(WINDOW_SIZE_X / 2, WINDOW_SIZE_Y / 2));
	
	
	mWorld.make_wall
		(sf::Vector2f(0, 0)
			, sf::Vector2f(WINDOW_SIZE_X, 25));
	mWorld.make_wall
		(sf::Vector2f(0, WINDOW_SIZE_Y - 25)
			, sf::Vector2f(WINDOW_SIZE_X, 25));
	mWorld.make_wall
		(sf::Vector2f(0, 0)
			, sf::Vector2f(25, WINDOW_SIZE_Y));
	mWorld.make_wall
		(sf::Vector2f(WINDOW_SIZE_X - 25, 0)
			, sf::Vector2f(25, WINDOW_SIZE_Y));
	for (int i = 0; i < walls; i++) {
		mWorld.make_wall
			(sf::Vector2f(rand() % WINDOW_SIZE_X, rand() % WINDOW_SIZE_Y)
				, sf::Vector2f(25.f, 25.f));

	}
	mWorld.make_spawner(
		sf::Vector2f(
			50.f + (rand() % (WINDOW_SIZE_X - 100)),
			50.f + (rand() % (WINDOW_SIZE_Y - 100))),
		dt,
		zombies)
		;
}


