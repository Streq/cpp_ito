#pragma once
#include "System.h"
#include "system_defines.h"
#include "SFML\Graphics.hpp"
class RenderingSystem :public System{
public:
	virtual void							update(sf::Time);
	void									draw(sf::RenderWindow& window);
	RenderingSystem(World&,sf::RenderTarget&);
	
	sf::RenderTarget&						mWindow;

};