#pragma once
#include "Entity.h"
struct Requeriments {
public:
	Flagset											needed;
	Flagset											rejected;
	Requeriments(const Flagset&, const Flagset&);
	Requeriments();
};