#pragma once
#include "Component.h"
#include "Character.h"
#include "EntityClass.h"
#include "States.h"
#include "Skill.h"
struct State : public Component {
	Character::ID Class;
	States::ID current;
	States::ID previous;
	
	sf::Vector2f facing_dir;
	sf::Vector2f moving_dir;

	sf::Time time_since_start;
	sf::Time duration;
	
	Skill::ID current_skill;

	bool pressed[Skill::size];
	bool updated[Skill::size];

	//to use for any purpose of the current skill
	unsigned skill_counter;
	
	
	bool just_started;
	inline void update(States::ID new_State) {
		previous = current;
		current = new_State;
		time_since_start = sf::Time::Zero;
		just_started = true;
		skill_counter = 0;
	}

	inline void update(States::ID new_State, sf::Time duration) {
		previous = current;
		current = new_State;
		time_since_start = sf::Time::Zero;
		just_started = true;
		this->duration = duration;
	}
	inline void cast(Skill::ID sk) {
		update(States::Casting);
		current_skill = sk;
	}

	
	/*
	inline States::ID next_state(States::ID st, State_Event::ID ev) {
		return Table[st*State_Event::size + ev];
	};
	static void init_globals() {
		memset(Table, States::None, States::size*State_Event::size);
#define X(st0,ev,st1) Table[States::##st0 * State_Event::size + State_Event::##ev] = States::##st1
		X(Normal, Hit, Hurt);
		X(Ducking, Hit, Hurt);
		X(Melee, Hit, Hurt);
		X(Hurt, TimeUp, Normal);
		X(Ducking, TimeUp, Normal);
		X(Melee, TimeUp, Normal);
		X(Teleporting, Hit, Hurt);
#undef X
	}
private:
	static States::ID Table[States::size*State_Event::size];
	
	*/
};
