#include "Settings.h"

void Settings::set_player(unsigned player, Character::ID character){
	player_characters[player]=character;
}

void Settings::set_players(unsigned players){
	this->players=players;
}

const Settings::characters & Settings::get_players(){
	return player_characters;
}

unsigned Settings::get_amount_players(){
	return players;
}


