#pragma once
#include<SFML\System\Vector2.hpp>
#include<math.h>

inline float vec_magn(float x, float y) {
	return powf(x*x + y*y, 0.5);
}

inline float vec_magn(const sf::Vector2f& v) {
	return vec_magn(v.x, v.y);
}
