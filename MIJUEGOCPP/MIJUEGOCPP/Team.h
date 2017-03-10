#pragma once
#include "Component.h"

struct Team : public Component{
	enum ID{
		None,
		HORDE,
		TEAM_1,
		TEAM_2,
		TEAM_3,
		TEAM_4
	};
	int id = None;
	Handle caster = MAX_ENTITIES;
};