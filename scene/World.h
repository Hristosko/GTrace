#pragma once
#include <vector>

#include "../geometry/Shape.h"
#include "../texture/Texture.h"

#include "../renderer/Camera.h"

#include "SceneSettings.h"
#include "Parser.h"

class World {
public:
	World();
	~World();

	void clear();

	void addElemenet(SceneElement* el, const std::unordered_map<std::string, std::string>& map);
	void add(Shape* shape);
	
	const std::vector<Shape*>& getShapes() const { return this->shapes; }
	SceneSettings& getSettings() { return this->settings; }
	Camera& getCamera() { return this->camera; }

	Texture* getTextureByName(const std::string& name);
	void addTexture(const std::string& name, Texture* text);
private:
	// This works since the map never moves the data in memory
	// Has to be changed if I use diff. map
	std::unordered_map<std::string, Texture*> textures;
	std::vector<Shape*> shapes;
	SceneSettings settings;
	Camera camera;
};

World& getWorld();