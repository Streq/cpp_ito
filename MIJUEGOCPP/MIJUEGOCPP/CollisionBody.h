#pragma once
#include "Component.h"
#include <SFML\Window.hpp>
#define COLLISION_BODY_DIAMETER(CIRCLE) CIRCLE.size.x
namespace BoxType {
	enum Type {
		Box,
		Circle,
	};

};

struct CollisionBody :public Component {
	//Position (relative to the Position component)
	//set it to (-width/2,-height/2) to center
	sf::Vector2f offset;
	sf::Vector2f size;//ending of the line
	BoxType::Type type;

	CollisionBody();
	CollisionBody(sf::Vector2f&& offset, sf::Vector2f&& size, BoxType::Type type=BoxType::Box);
	CollisionBody(const sf::Vector2f& offset, const sf::Vector2f& size, BoxType::Type type=BoxType::Box);

	
};