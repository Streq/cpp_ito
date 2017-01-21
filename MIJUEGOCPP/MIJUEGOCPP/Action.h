#pragma once
namespace Input {
	enum ID {
		up,
		down,
		left,
		right,
		size
	};
}

//MAN HAY QUE IMPLEMENTAR LAS ACCIONES
class Action {
	virtual void execute();
};