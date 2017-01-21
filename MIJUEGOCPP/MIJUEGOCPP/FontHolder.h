#pragma once
#include "ResourceHolder.h"
namespace Font {
	enum ID {
		arial,
	};
}


typedef ResourceHolder<sf::Font, Font::ID> FontHolder;
