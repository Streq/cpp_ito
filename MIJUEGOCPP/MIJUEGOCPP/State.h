#pragma once
#include "Component.h"
#include "Color.h"
namespace State_Event {
	enum ID {
		Hit,
		TimeUp,
		Stun,
		Wall,
		size
	};
}


namespace States {
	enum ID {
		None=-1,
		Player_Normal,
		Player_Ducking,
		Player_Teleporting,
		Player_Melee,
		Player_Hurt,
		Enemy_Normal,
		Enemy_Hurt,
		Teleport_Scope,
		Spawner,
		Hit_Box,
		size
	};

	const sf::Time Duration[size] = {
		sf::Time::Zero,		//Player_Normal
		sf::seconds(0.5f),	//Player_Ducking
		sf::Time::Zero,		//Player_Teleporting
		dt*sf::Int64(60),	//Player_Melee
		sf::seconds(0.3f),	//Player_Hurt
		sf::Time::Zero,		//Enemy_Normal
		sf::seconds(0.5f),	//Enemy_Hurt
		sf::Time::Zero,		//Teleport_Scope
		sf::seconds(5.f),	//Spawner
		dt*sf::Int64(60),	//Hit_Box
	};

	const sf::Color Color[size] = {
		Color::Green,		//Player_Normal
		Color::Dark_Green,	//Player_Ducking
		Color::Green,		//Player_Teleporting
		Color::Green,		//Player_Melee
		Color::Red,			//Player_Hurt
		Color::Dark_Blue,	//Enemy_Normal
		Color::Red,			//Enemy_Hurt
		Color::Transparent,	//Teleport_Scope
		Color::Transparent,	//Spawner
		Color::Red,			//Hit_Box
	};
};

struct State : public Component {
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
	inline States::ID next_state(States::ID st, State_Event::ID ev) {
		return Table[st*State_Event::size + ev];
	};
	static void init_globals() {
		memset(Table, States::None, States::size*State_Event::size);
#define X(st0,ev,st1) Table[States::##st0 * State_Event::size + State_Event::##ev] = States::##st1
		X(Player_Normal, Hit, Player_Hurt);
		X(Player_Ducking, Hit, Player_Hurt);
		X(Player_Melee, Hit, Player_Hurt);
		X(Player_Hurt, TimeUp, Player_Normal);
		X(Player_Ducking, TimeUp, Player_Normal);
		X(Player_Melee, TimeUp, Player_Normal);
		X(Player_Teleporting, Hit, Player_Hurt);
#undef X
	}
private:
	static States::ID Table[States::size*State_Event::size];
	static sf::Color Color[States::size];
	static sf::Time Duration[States::size];

};
