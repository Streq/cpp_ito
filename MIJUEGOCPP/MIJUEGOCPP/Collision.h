#pragma once
#include "Handle.h"
#include "defines.h"
#include <tuple>
#include <algorithm>
class Collision
{
	private:
		

	public:
		typedef std::pair<Handle, Handle> Pair;
		Pair entities;
		Collision(Handle h1, Handle h2);

};