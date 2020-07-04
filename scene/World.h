#pragma once
#include <vector>

#include "../geometry/Shape.h"
#include "Parser.h"

class World {
public:
	World();
	~World();

	void addElemenet(SceneElement* el);
	void add(Shape* shape);
	const std::vector<Shape*>& getShapes() { return this->shapes; }
private:
	std::vector<Shape*> shapes;
};

World& getWorld();