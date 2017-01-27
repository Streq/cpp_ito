#pragma once
#include "Component.h"
#include "Handle.h"
struct Team :public Component {
	Handle owner;
	Handle team;
};