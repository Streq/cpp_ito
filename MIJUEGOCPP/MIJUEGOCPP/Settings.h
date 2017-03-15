#pragma once
#include "Character.h"
#include <array>
#include "defines.h"
#include "Handle.h"
class Settings{
	private:
	typedef std::array<Character::ID, MAX_PLAYERS>	player_characters;
	typedef std::array<Handle, MAX_PLAYERS>			player_handles;

	private:
	player_characters								mPlayerCharacters;
	unsigned										players;


	public:
	void											set_player(unsigned player, Character::ID character);
	void											set_players(unsigned players);
	const player_characters&						get_players();
	unsigned										get_amount_players();
};