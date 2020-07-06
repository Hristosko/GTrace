#include "World.h"

#include "../Logger.h"
#include "../geometry/Triangle.h"
World::World() {}

World::~World() {
	this->clear();
}

void World::clear() {
	for (Shape* shape : this->shapes) delete shape;
	this->shapes.clear();

	for (auto& it : this->textures) delete it.second;
	this->textures.clear();
}

void World::addElemenet(SceneElement* el, const std::unordered_map<std::string, std::string>& map) {
	if (Shape* x = dynamic_cast<Shape*>(el)) {
		this->add(x);
	}
	else if (Texture * x = dynamic_cast<Texture*>(el)) {
		auto it = map.find("name");
		if (it == map.end()) {
			LOGWARNING("Unnamed texture.");
		}
		else {
			this->addTexture(it->second, x);
		}
	}
}

void World::add(Shape* shape) {
	this->shapes.push_back(shape);
}

Texture* World::getTextureByName(const std::string& name) {
	auto it = this->textures.find(name);
	if (it == this->textures.end()) {
		LOGERROR("Searching for unexisting texture: ", name);
		throw std::out_of_range("texture not faund");
	}
	return it->second;
}

void World::addTexture(const std::string& name, Texture* text) {
	auto it = this->textures.find(name);
	if (it != this->textures.end()) {
		LOGWARNING("Overwriting texture: ", name);
	}
	this->textures[name] = text;
}

World& getWorld() {
	static World w;
	return w;
}
