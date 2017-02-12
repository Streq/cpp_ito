#include "PlayerInput.h"
void PlayerInput::update_key(const input_data& data, bool pressed) {
	for (size_t i = 0; i < Input::size; i++) {
		if (data == keys[i]) {
			just_updated_keys[i] = pressed_keys[i] != pressed;
			pressed_keys[i] = pressed;
		}
	}
}

void PlayerInput::set_key(const input_data& key, Input::ID action) {
	keys[action] = key;
}

bool PlayerInput::check_pressed(Input::ID i) const{ return pressed_keys[i]; }
bool PlayerInput::check_updated(Input::ID i) const{ return just_updated_keys[i]; }
void PlayerInput::clear_updated(){
	memset(just_updated_keys,0,Input::size);
}
const bool(&PlayerInput::get_pressed() const)[Input::size]{return pressed_keys;};
const bool(&PlayerInput::get_updated() const)[Input::size]{return just_updated_keys;};