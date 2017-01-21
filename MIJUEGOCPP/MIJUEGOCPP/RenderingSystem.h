#pragma once
#include "System.h"
#include "system_defines.h"
class RenderingSystem :public System{
public:
	virtual void							update(sf::Time);
	RenderingSystem(World&,sf::RenderTarget&);
	
	sf::RenderTarget&						mWindow;

};