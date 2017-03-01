#pragma once
#include<SFML\System\Vector2.hpp>
#define _USE_MATH_DEFINES
#include<math.h>

inline float vec_magn(float x, float y) {
	return powf(x*x + y*y, 0.5);
}

inline float vec_magn(const sf::Vector2f& v) {
	return vec_magn(v.x, v.y);
}

//Normalizes the vector, which means its module becomes 1 (or 0 if each of its members is 0)
inline sf::Vector2f normalize(sf::Vector2f&& v) {
	if (v.x!=0||v.y!=0)
		v = v / vec_magn(v);
	return std::move(v);
}

inline sf::Vector2f& normalize(sf::Vector2f& v) {
	if (v.x != 0 || v.y != 0)
		v = v / vec_magn(v);
	return v;
}
inline sf::Vector2f normalized(const sf::Vector2f& v) {
	if (v.x != 0 || v.y != 0)
		return v / vec_magn(v);
	return v;
}
/*	
								
						|270
		SFML			|			 
			   180_____3|4______0
					   2|1
						|        
						|90

						|90
		math.h			|			math.h angle to SFML angle -> -angle
			   180_____2|1______0
					   3|4
						|
						|270
*/
inline sf::Vector2f from_angle(float angle) {
	//Transformation
	angle = -angle;
	return sf::Vector2f(cosf(angle),sinf(angle));
}
inline float get_angle(const sf::Vector2f& vec) {
	return (atan2f(vec.x, vec.y)-M_PI/2.f);
}

inline sf::Vector2f normal(const sf::Vector2f& vec) {
	return sf::Vector2f(-vec.y, vec.x);
}