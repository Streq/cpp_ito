#pragma once
#include "Component.h"
#include "Handle.h"
#include "defines.h"
struct Owner :public Component {
	Handle owner=max_entities;
	bool lost_on_wall=false;
private:
	static matriz_padre_hijo_t matriz_padre_hijo;
public:
	inline static const matriz_padre_hijo_t& get_matriz_padre_hijo(){ 
		return matriz_padre_hijo;
	};
	inline static void set(Handle padre, Handle hijo, Relacion::ID val=Relacion::composition) {
		matriz_padre_hijo[padre][hijo]=val;
	};
	inline void set_owner(Handle padre, Handle hijo, Relacion::ID val) {
		matriz_padre_hijo[padre][hijo] = val;
		owner = padre;
	}
	
};