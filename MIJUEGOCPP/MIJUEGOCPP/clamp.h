#pragma once
#include <SFML\System\Vector2.hpp>

float inline clamp(float val, float min, float max) {
	if (val < min)return min;
	if (val < max)return val;
	return max;
}

sf::Vector2f inline clamp_vec(const sf::Vector2f& val, const sf::Vector2f& min, const sf::Vector2f& max) {
	return sf::Vector2f(clamp(val.x, min.x, max.x), clamp(val.y, min.y, max.y));
}

//negative amounts will be passed as 0
float inline approach(float value, float target, float amount) {
	float distance = target-value;
	int dist_sign = SIGN(distance);
	distance *= dist_sign;//makes distance unsigned
	amount = dist_sign * clamp(amount,0,distance);//amount is never greater than distance
	return value + amount;
}
