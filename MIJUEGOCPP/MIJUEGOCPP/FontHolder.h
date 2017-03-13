#pragma once
#include "ResourceHolder.h"
namespace Font {
	enum ID {
		arial,
		consola,
	};
}


typedef ResourceHolder<sf::Font, Font::ID> FontHolder;
