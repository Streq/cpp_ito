#include "CollisionBody.h"

CollisionBody::CollisionBody(sf::Vector2f && offset, sf::Vector2f && size) : offset(offset), size(size) {}
CollisionBody::CollisionBody(const sf::Vector2f & offset, const sf::Vector2f & size) : offset(offset), size(size) {}
