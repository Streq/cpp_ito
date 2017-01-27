#include "PlayerInput.h"
void PlayerInput::update_key(sf::Keyboard::Key key, bool pressed) {
	for (size_t i = 0; i < Input::size; i++) {
		if (key == keys[i]) {
			just_updated_keys[i] = pressed_keys[i] != pressed;
			pressed_keys[i] = pressed;
		}
	}
}
void PlayerInput::set_keys(const Key (&ks)[Input::size]) {
	memcpy(keys,ks,Input::size);
};

void PlayerInput::set_key(const Key& key, Input::ID action) {
	keys[action] = key;
}

bool PlayerInput::check_pressed(Input::ID i) const{ return pressed_keys[i]; }
bool PlayerInput::check_updated(Input::ID i) const{ return just_updated_keys[i]; }
void PlayerInput::clear_updated(){
	memset(just_updated_keys,0,Input::size);
}
const bool(&PlayerInput::get_pressed() const)[Input::size]{return pressed_keys;};
const bool(&PlayerInput::get_updated() const)[Input::size]{return just_updated_keys;};