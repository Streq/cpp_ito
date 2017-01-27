#pragma once
#include "defines.h"


struct Component {
	//jaja no hago nada lero lero
	
};


namespace type{
	enum ID {
		Active=0,
		/*
		Component1,
		Component2,
		etc,
		*/
		#define X(comp) comp,
		DO_X_FOR_COMPONENT_LIST
		#undef X
		size
	};
}

namespace flag{	
	enum ID{
		none=0,
		Active=1,
		/*
		Component1 = 1<<type::Component1,
		Component2 = 1<<type::Component2,
		etc,
		*/
		#define X(comp) comp = 1<<type::##comp,
		DO_X_FOR_COMPONENT_LIST
		#undef X
	};

}
