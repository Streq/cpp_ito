#pragma once
#include "Component.h"
#include "Handle.h"
#include "defines.h"
struct Owner :public Component {
	Handle owner;
private:
	static matriz_booleana matriz_padre_hijo;
public:
	inline static const matriz_booleana& get_matriz_padre_hijo(){ 
		return matriz_padre_hijo;
	};
	inline static void set(Handle padre, Handle hijo, bool val=true) {
		matriz_padre_hijo[padre][hijo]=val;
	};
	inline void set_owner(Handle padre, Handle hijo) {
		matriz_padre_hijo[padre][hijo] = true;
		owner = padre;
	}
	
};