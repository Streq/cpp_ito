#pragma once
#include<SFML\System\Vector2.hpp>
#include<math.h>

inline float vec_magn(float x, float y) {
	return powf(x*x + y*y, 0.5);
}

inline float vec_magn(const sf::Vector2f& v) {
	return vec_magn(v.x, v.y);
}

//Normalizes the vector, which means its module becomes 1 (or 0 if each of its members is 0)
inline sf::Vector2f& normalize(sf::Vector2f&& v) {
	if (v.x!=0||v.y!=0)
		v = v / vec_magn(v);
	return v;
}