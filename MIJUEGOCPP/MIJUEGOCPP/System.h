#pragma once
#include <SFML\System.hpp>
#include "Handle.h"
#include "World.h"
#include "Requeriments.h"
#include <set>
#include <array>
#include <vector>

typedef std::array<bool, MAX_ENTITIES> ENTITIES;
#define ITERATE_START for (size_t i = 0; i < MAX_ENTITIES; i++) { if (entities[i]) {
#define ITERATE_END }}


class World;
class System
{
	
	public:
		virtual void							update(sf::Time)=0;
		virtual void							update_entity(const Flagset&, Handle);
		System(World&, const Requeriments&);

	protected:
		World&									mWorld;
		const Requeriments						requeriments;
		ENTITIES								entities;
		
};


