#include "System.h"
#include "defines.h"
#include "Entity.h"
#include <iostream>
System::System(World& world, const Requeriments& req)
	:mWorld(world)
	, requeriments(req)
	, entities({})
{}


void System::check_entity(const Flagset& fl, Handle h) {
	bool antes = entities[h];
	bool despues = entities[h] = TESTMASK(fl, requeriments.needed) && TESTMASK(~requeriments.rejected, fl);
	//if (antes!=despues)std::cout << "se "<<(despues?"":"des")<<"copo un sistema con la bandera " << fl.to_string() <<" del handle "<< h << std::endl;
}
/*
void System::check_entity(const Flagset& fl, Handle h) {
	auto i = std::lower_bound (entities.begin(), entities.end(), h);
	bool found = i != entities.end() && h>=*i;
	bool check = TESTMASK(fl, requeriments.needed) && TESTMASK(~requeriments.rejected, fl);
	if (!found && check) {
		entities.insert(i, h);
		return;
	}

	if (found && !check) {
		entities.erase(i);
	}
}
*/