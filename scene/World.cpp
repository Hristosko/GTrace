#include "World.h"

#include "../geometry/Triangle.h"
World::World() {}

World::~World() {
	this->clear();
}

void World::clear() {
	for (Shape* shape : this->shapes) delete shape;
	this->shapes.clear();
}

void World::addElemenet(SceneElement* el) {
	if (Shape* x = dynamic_cast<Shape*>(el)) {
		this->add(x);
	}
}

void World::add(Shape* shape) {
	this->shapes.push_back(shape);
}

World& getWorld() {
	static World w;
	return w;
}
