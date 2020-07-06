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

	for (auto& it : this->materials) delete it.second;
	this->materials.clear();

	for (auto& it : this->textures) {
		delete it.second;
	}
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
	} else if (Material * x = dynamic_cast<Material*>(el)) {
		auto it = map.find("name");
		if (it == map.end()) {
			LOGWARNING("Unnamed texture.");
		}
		else {
			this->addMaterial(it->second, x);
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

Material* World::getMaterialByName(const std::string& name) {
	auto it = this->materials.find(name);
	if (it == this->materials.end()) {
		LOGERROR("Searching for unexisting mateial: ", name);
		throw std::out_of_range("material not faund");
	}
	return it->second;
}

void World::addMaterial(const std::string& name, Material* mat) {
	auto it = this->materials.find(name);
	if (it != this->materials.end()) {
		LOGWARNING("Overwriting material: ", name);
	}
	this->materials[name] = mat;
}

World& getWorld() {
	static World w;
	return w;
}
