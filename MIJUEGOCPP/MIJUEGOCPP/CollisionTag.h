#pragma once
#include "Component.h"
#include "Tag.h"
struct CollisionTag :public Component {

public:
	Tag::ID tag;



public:
	typedef bool Matrix[Tag::size*Tag::size];

	static const Matrix& get_matrix();
	static void init_matrix();
	static inline bool check_pair(Tag::ID a, Tag::ID b) { return matrix[a*Tag::size + b]; }
	

private:

	static Matrix matrix;

	static void pair_tags(Tag::ID a, Tag::ID b, bool val = true);

};
