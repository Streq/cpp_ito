#include "Collision.h"

Collision::Collision(Handle h1, Handle h2) :entities(h1, h2) {
	if (h1 > h2) {
		std::swap<Handle>(entities.first, entities.second);
	}
}
