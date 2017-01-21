#pragma once
#include <vector>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <stdint.h>
#include <SFML\Graphics.hpp>
#include "defines.h"
#include <memory>
#include "Action.h"
#include "vec_magn.h"
#include "Tag.h"
struct Component {
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


struct Position : public Component, public sf::Transformable {
};

/*
struct Position : public Component {
	sf::Vector2f position;
	float angle;
};
*/
struct Movement : public Component {
	sf::Vector2f velocity;
	sf::Vector2f acceleration;
	float rotation;
};

struct Rendering :public Component, public sf::Transformable {
	std::unique_ptr<sf::Drawable> drawable;
};


namespace controller {
	enum ID {
		Player,
		size
	};
}
struct Controller :public Component {
	controller::ID controller;
	bool actions[Input::size];
};

struct MovementProperties :public Component {
	float friction;
	float max_speed;
};


namespace CollisionShape {
	enum ID {
		Circle,
		Rectangle,
		Ellipse,
		size
	};
}
struct CollisionBox :public Component {
	//Position (relative to the Position component)
	//set it to (-width/2,-height/2) to center
	sf::Vector2f offset;
	sf::Vector2f size;

	CollisionBox() {}
	CollisionBox(sf::Vector2f&& offset, sf::Vector2f&& size);
	CollisionBox(const sf::Vector2f& offset, const sf::Vector2f& size);

	
};


struct CollisionTag :public Component {
	
public:
	typedef bool Matrix[Tag::size*Tag::size];

	static const Matrix& get_matrix();
	static void init_matrix();
	static inline bool check_pair(Tag::ID a, Tag::ID b){ return matrix[a*Tag::size + b];}
	Tag::ID tag;

private:

	static Matrix matrix;

	static void pair_tags(Tag::ID a, Tag::ID b, bool val = true);

};


union Shape {
	//SHAPES//
	struct Rectangle {
		sf::Vector2f size;
	}rectangle;
	struct Circle {
		float radius;
	}circle;
	struct Ellipse {
		sf::Vector2f radius;
	}ellipse;
	/////////

	Shape() {};
};
