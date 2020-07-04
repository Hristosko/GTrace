#include "World.h"

#include "../geometry/Triangle.h"
World::World() {
	Triangle* tr = new Triangle(Vector3f(0, 0, 0), Vector3f(200, 0, 0), Vector3f(0, 200, 0), Vector3f(1, 0, 0));
	this->add(tr);
}

World::~World() {
	for (Shape* shape : this->shapes) delete shape;
}

void World::add(Shape* shape) {
	this->shapes.push_back(shape);
}

World& getWorld() {
	static World w;
	return w;
}
