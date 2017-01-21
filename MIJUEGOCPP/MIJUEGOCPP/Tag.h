#pragma once
#define DO_X_FOR_TAG_LIST \
X(Building)\
X(Bullet)\
X(Enemy)\
X(Player)\
X(Drop)


namespace Tag {
	enum ID {
		#define X(tag) tag,
		DO_X_FOR_TAG_LIST
		#undef X
		size,
	};

	namespace flag {
		enum ID {
			#define X(tag) tag = 1 << Tag::##tag,
			DO_X_FOR_TAG_LIST
			X(size)
			#undef X
		};
	}
}

