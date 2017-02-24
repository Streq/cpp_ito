#pragma once
#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include "Color.h"


namespace Bullet{
	namespace stats {
		const float radius = 2.5f;
		const float speed = 400.f;
		const sf::Time time = sf::seconds(2.5f);
		const int damage = 1;
	}
}

/*
namespace Enemy {
	namespace stats {
		const float max_speed = 100.f;
		const float friction = 400.f;
		const float acceleration = 1000.f;
		const float size = 10.f;
		const int health = 3;
		const int damage = 1;
		const sf::Time inv_time=dt*sf::Int64(3);
	}
}

namespace Teleport_Scope {
	namespace stats {
		const float max_speed = 600.f;
		const float friction = 600.f;
		const float acceleration = 3600.f;
	}
}
namespace HitBox {
	namespace stats {
		const float size = Player::stats::size*3;
		const float offset = Player::stats::size;
		const int damage = 2;
	}
}
*/