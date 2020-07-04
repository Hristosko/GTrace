#pragma once
#include <vector>

#include "../geometry/Shape.h"

class World {
public:
	World();
	~World();

	void add(Shape* shape);
	const std::vector<Shape*>& getShapes() { return this->shapes; }
private:
	std::vector<Shape*> shapes;
};

World& getWorld();