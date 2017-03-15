#include "Settings.h"

void Settings::set_player(unsigned player, Character::ID character){
	mPlayerCharacters[player]=character;
}

void Settings::set_players(unsigned players){
	this->players=players;
}

const Settings::player_characters & Settings::get_players(){
	return mPlayerCharacters;
}

unsigned Settings::get_amount_players(){
	return players;
}


