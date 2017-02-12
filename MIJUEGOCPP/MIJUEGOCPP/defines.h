#pragma once
#include <memory>
#include <SFML\System\Time.hpp>
#include <SFML\Graphics\Color.hpp>
enum {
	max_entities = 3000,
	WINDOW_SIZE_X = 1200,
	WINDOW_SIZE_Y = 600,
	FPS = 60,
	players = 2,
};
const sf::Time dt = sf::seconds(1.0f / FPS);
const sf::Time fps_update_time = sf::seconds(1.f);
const float joy_deadzone = 40.f;

#define SIGN(num) ((num>0)-(num<0))

//#define MASKS
#ifndef MASKS
#define LISTS
#endif
template<typename T>
using ptr = std::unique_ptr<T>;

#define DO_X_FOR_COMPONENT_LIST \
X(Position)\
X(Movement)\
X(Rendering)\
X(Controller)\
X(CollisionBody)\
X(CollisionTag)\
X(State)\
X(Team)\
X(TimeSpan)\
X(Health)\
X(Damage)
#define TESTMASK(a,b) ((Flagset(a) & Flagset(b))==b)

