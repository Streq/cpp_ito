#pragma once
#include "Component.h"
#include "Color.h"
namespace States {
	enum ID {
		Normal,
		Ducking,
		Teleporting,
		Hurt,
		Enemy_Normal,
		Enemy_Hurt,
		Teleport_Scope,
		Spawner,
		Hit_Box,
		size
	};

	const sf::Time Duration[size] = {
		sf::Time::Zero,   //Normal
		sf::seconds(0.5f),//Ducking
		sf::Time::Zero,	  //Teleporting
		sf::seconds(0.3f),//Hurt
		sf::Time::Zero,	  //Enemy_Normal
		sf::seconds(0.5f),//Enemy_Hurt
		sf::Time::Zero,	  //Teleport_Scope
		sf::seconds(5.f), //Spawner
		dt,//Hit_Box
	};

	const sf::Color Color[size] = {
		Color::Green,		//Normal
		Color::Dark_Green,	//Ducking
		Color::Green,		//Teleporting
		Color::Red,			//Hurt
		Color::Dark_Blue,	//Enemy_Normal
		Color::Red,			//Enemy_Hurt
		Color::Transparent,	//Teleport_Scope
		Color::Transparent,	//Spawner
		Color::Red,			//Hit_Box
	};
};

struct State: public Component{
	States::ID current;
	States::ID previous;
	sf::Time time_since_start;
	sf::Time duration;
	bool just_started;
	inline void update(States::ID new_State) { 
		previous = current;
		current = new_State;
		time_since_start = sf::Time::Zero;
		just_started = true;
		duration = States::Duration[current];
	}
};
