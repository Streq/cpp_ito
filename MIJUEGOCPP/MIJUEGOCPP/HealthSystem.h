#pragma once
#include "System.h"
#include "system_defines.h"
class HealthSystem :public System{
	public:
		HealthSystem(World&);
		virtual void				update(sf::Time);
};

