#pragma once
#include "ResourceHolder.h"
namespace Texture {
	enum ID {
		MENU_BACKGROUND,
		SPRITE_JUANI,
		SPRITE_GORDI,
		SPRITE_TORO,
		SPRITE_PEREZ,
	};
}


typedef ResourceHolder<sf::Texture, Texture::ID> TextureHolder;
