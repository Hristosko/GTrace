#pragma once
#include <vector>

#include "../geometry/Shape.h"
#include "SceneSettings.h"
#include "Parser.h"

class World {
public:
	World();
	~World();

	void clear();

	void addElemenet(SceneElement* el);
	void add(Shape* shape);
	
	const std::vector<Shape*>& getShapes() const { return this->shapes; }
	SceneSettings& getSettings() { return this->settings; }
private:
	std::vector<Shape*> shapes;
	SceneSettings settings;
};

World& getWorld();