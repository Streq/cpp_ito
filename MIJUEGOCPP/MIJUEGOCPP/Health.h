#pragma once
#include "Component.h"
struct Health : public Component {
	int current;
	int max;
	int incoming_damage;
	bool invulnerable = false;

	void inline init(int Max) { max = Max; current = max; };
	void inline init(int Max, int Cur) { max = Max; current = Cur; }
};