#pragma once
#include <memory>
#include <SFML\System\Time.hpp>
enum {	
	max_entities = 100,
	WINDOW_SIZE_X = 600,
	WINDOW_SIZE_Y = 600,
	FPS = 60,
};
const sf::Time dt = sf::seconds(1.0f / FPS);
const sf::Time fps_update_time = sf::seconds(1.f);



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
X(CollisionBox)\
X(CollisionTag)


#define TESTMASK(a,b) ((Flagset(a) & Flagset(b))==b)

