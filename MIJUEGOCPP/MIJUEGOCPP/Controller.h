#pragma once
#include "Component.h"
#include "PlayerInput.h"
namespace controller {
	enum ID {
		Player,
		Enemy,
		size
	};
}
struct Controller :public Component {
	controller::ID controller;
	bool actions[Input::size];
	sf::Vector2f facing_dir;
	sf::Vector2f moving_dir;
};
