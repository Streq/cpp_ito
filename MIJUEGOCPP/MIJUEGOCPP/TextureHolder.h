#pragma once
#include "ResourceHolder.h"
namespace Texture {
	enum ID {
		MENU_BACKGROUND
	};
}


typedef ResourceHolder<sf::Texture, Texture::ID> TextureHolder;
