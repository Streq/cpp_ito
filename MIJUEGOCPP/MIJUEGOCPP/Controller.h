#pragma once
#include "Component.h"
#include "PlayerInput.h"
#include "Handle.h"
namespace controller {
	enum ID {
		Player,
		Enemy,
		size
	};
}
struct Controller :public Component {
	controller::ID controller;
	short unsigned player;
	Handle target=-1;
	bool actions[Input::size];
	sf::Vector2f facing_dir;
	sf::Vector2f moving_dir;
};
