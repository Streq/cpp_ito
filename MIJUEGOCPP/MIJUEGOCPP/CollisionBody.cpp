#include "CollisionBody.h"

CollisionBody::CollisionBody():type(BoxType::Box){
}

CollisionBody::CollisionBody(sf::Vector2f && offset, sf::Vector2f && size, BoxType::Type type) : offset(offset), size(size), type(type) {}
CollisionBody::CollisionBody(const sf::Vector2f & offset, const sf::Vector2f & size, BoxType::Type type) : offset(offset), size(size), type(type) {}


