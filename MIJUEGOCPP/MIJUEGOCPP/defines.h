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
	skill_num = 4,
};
const sf::Time dt = sf::seconds(1.0f / FPS);
const sf::Time fps_update_time = sf::seconds(1.f);
const float joy_deadzone = 40.f;
typedef bool matriz_booleana[max_entities][max_entities];

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
X(CollisionInfo)\
X(State)\
X(Owner)\
X(TimeSpan)\
X(Health)\
X(Damage)
#define TESTMASK(a,b) ((Flagset(a) & Flagset(b))==b)

