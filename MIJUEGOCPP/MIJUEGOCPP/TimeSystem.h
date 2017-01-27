#pragma once
#include "System.h"
#include "system_defines.h"
class TimeSystem :
	public System
{
public:
	TimeSystem(World&);
	virtual void					update(sf::Time dt);
};

