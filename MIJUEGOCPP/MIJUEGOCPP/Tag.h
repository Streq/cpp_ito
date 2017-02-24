#pragma once
#define DO_X_FOR_TAG_LIST \
X(Building)\
X(Bullet)\
X(Char_Body)\
X(Scope)\
X(Hit_Box)


namespace Tag {
	enum ID {
		Wall,//Interacts with Solid Entities
		Character_Entity,//Interacts with Buildings
		Projectile,
		Hit_Box,//Interacts with other Hit_Boxes
		size
		/*		
		#define X(tag) tag,
		DO_X_FOR_TAG_LIST
		#undef X
		size,
		*/
	};
	/*
	namespace flag {
		enum ID {
			#define X(tag) tag = 1 << Tag::##tag,
			DO_X_FOR_TAG_LIST
			X(size)
			#undef X
		};
	}
	*/
}

