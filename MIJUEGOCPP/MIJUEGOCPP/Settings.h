#pragma once
#include "Character.h"
#include <array>
#include "defines.h"
class Settings{
	private:
	typedef std::array<Character::ID, MAX_PLAYERS>	characters;
	
	private:
	characters										player_characters;
	unsigned										players;

	public:
	void											set_player(unsigned player, Character::ID character);
	void											set_players(unsigned players);
	const characters&								get_players();
	unsigned										get_amount_players();
};