#pragma once
#include <vector>
#include <SFML\System\Vector2.hpp>
enum Component{
	None	   = 0	 ,
	Velocity   = 1 << 0,
	Drawable   = 1 << 1,
	Transform  = 1 << 2,
	Controller = 1 << 3,
	Mass	   = 1 << 4,
};
class IComponent {
};
class Velocity_component:public IComponent{
	public:
		sf::Vector2f vector;
};
class Transform_component :public IComponent {
	public:
		sf::Vector2f vector;
};