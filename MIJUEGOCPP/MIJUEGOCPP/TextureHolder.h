#pragma once
#include "ResourceHolder.h"
namespace Texture {
	enum ID {
		Player,
		Enemy,
		jajax
	};
}


typedef ResourceHolder<sf::Texture, Texture::ID> TextureHolder;
