#include "CollisionTag.h"
CollisionTag::Matrix CollisionTag::matrix = {};

void CollisionTag::pair_tags(Tag::ID a, Tag::ID b, bool val)
{
	matrix[a*Tag::size + b] = matrix[b*Tag::size + a] = val;
}
const CollisionTag::Matrix & CollisionTag::get_matrix() { return matrix; }

void CollisionTag::init_matrix()
{
	pair_tags(Tag::Building, Tag::Player);
	pair_tags(Tag::Building, Tag::Enemy);
	pair_tags(Tag::Building, Tag::Bullet);
	pair_tags(Tag::Building, Tag::Scope);
	pair_tags(Tag::Player, Tag::Enemy);
	pair_tags(Tag::Bullet, Tag::Enemy);
	pair_tags(Tag::Enemy, Tag::Enemy);
}
