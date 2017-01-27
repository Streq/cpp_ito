#pragma once
#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include "Color.h"
namespace Player {
	namespace stats{
		const float	max_speed = 150.f;
		const float friction = 600.f;
		const float acceleration = 1200.f;
		const float size = 10.f;
		const int health = 10;
	}
}

namespace Bullet{
	namespace stats {
		const float radius = 2.5f;
		const float speed = 300.f;
		const sf::Time time = sf::seconds(2.5f);
		const int damage = 1;
	}
}

namespace Enemy {
	namespace stats {
		const float max_speed = 100.f;
		const float friction = 400.f;
		const float acceleration = 1000.f;
		const float size = 10.f;
		const int health = 3;
		const int damage = 1;
	}
}

namespace Teleport_Scope {
	namespace stats {
		const float max_speed = 600.f;
		const float friction = 600.f;
		const float acceleration = 3600.f;
	}
}