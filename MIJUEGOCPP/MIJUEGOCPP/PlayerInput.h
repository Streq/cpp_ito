#pragma once
#include <SFML\Window.hpp>
#include "Action.h"
namespace JoyButtons {
	enum Button {
		A,
		B,
		X,
		Y,
		LB,
		RB,
		Back,
		Start,
		size
	};
}


struct input_data {
	unsigned code;
	enum Type { keyboard, joy_button, joy_axis } type;
	unsigned joy_ID;
	int pos_axis;
	input_data() {};
	input_data(unsigned code, Type type, unsigned joy_ID = 0, int pos_axis = 0) 
		:code(code)
		,type(type)
		,joy_ID(joy_ID)
		,pos_axis(pos_axis){
	}
	inline bool operator==(const input_data& r) const { 
		return 
			code == r.code &&
			type == r.type &&
			joy_ID == r.joy_ID &&
			pos_axis==r.pos_axis; }


};


class PlayerInput
{
public:
	typedef sf::Keyboard::Key Key;

	//typedef unsigned Key;
private:
	bool					pressed_keys[Input::size];
	bool					just_updated_keys[Input::size];
	input_data				keys[Input::size];
public:
	void					update_key(const input_data& data, bool pressed);
	void					set_key(const input_data& key, Input::ID action);
	bool					check_pressed(Input::ID action)const;//check if currently pressed
	bool					check_updated(Input::ID action)const;//check if just updated
	void					clear_updated();
	const bool				(&get_pressed() const)[Input::size];
	const bool 				(&get_updated() const)[Input::size];
};