#pragma once
#include <SFML\Window.hpp>
#include "Action.h"

class PlayerInput
{
public:
	typedef sf::Keyboard::Key Key;
	
private:
	bool					pressed_keys[Input::size];
	bool					just_updated_keys[Input::size];
	Key						keys[Input::size];

public:
	void					update_key(Key key, bool pressed);
	void					set_keys(const Key (&ks)[Input::size]);
	void					set_key(const Key& key, Input::ID action);
	bool					check_pressed(Input::ID action)const;//check if currently pressed
	bool					check_updated(Input::ID action)const;//check if just updated
	const bool				(&get_pressed() const)[Input::size];
	const bool 				(&get_updated() const)[Input::size];
};