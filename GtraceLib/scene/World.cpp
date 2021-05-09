#include "World.h"

#include "Logger.h"
#include "geometry/Triangle.h"
#include "geometry/Mesh.h"
#include "geometry/BVH.h"

#include <limits>

namespace gtrace {

World::World() : bvh(nullptr) {}

World::~World() {
	this->clear();
}

void World::clear() {
	delete this->bvh;
	this->bvh = nullptr;

	for (Mesh* mesh : this->meshes) delete mesh;
	this->meshes.clear();

	for (Shape* shape : this->shapes) delete shape;
	this->shapes.clear();

	for (Light* light : this->lights) delete light;
	this->lights.clear();

	for (auto& it : this->materials) delete it.second;
	this->materials.clear();

	for (auto& it : this->textures) {
		delete it.second;
	}
	this->textures.clear();
	camera = nullptr;
}

void World::addElemenet(SceneElement* el, const std::unordered_map<std::string, std::string>& map) {
	// The mesh will store its elements as shapes, the mesh is only kept alive here
	if (Mesh* x = dynamic_cast<Mesh*>(el)) {
		this->meshes.push_back(x);
	}
	else if (Shape* x = dynamic_cast<Shape*>(el)) {
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
			LOGWARNING("Unnamed material.");
		}
		else {
			this->addMaterial(it->second, x);
		}
	} else if(Light* x = dynamic_cast<Light*>(el)) {
		this->lights.push_back(x);
	} else if (Camera* x = dynamic_cast<Camera*>(el)) {
		this->setCamera(x);
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

void World::buildBVH() {
	delete this->bvh;
	this->bvh = BVH::build(this->shapes.data(), this->shapes.size());
	this->shapes.clear();
}

bool World::intersect(const Ray& ray, HitRecord& rec, float time) const {
	if (this->bvh == nullptr) return false;
	bool res =  this->bvh->hit(ray, 0.f, std::numeric_limits<float>::max(), time, rec);
	if (res) rec.position = ray.pointAt(rec.t);
	return res;
}

bool World::intersect(const Ray& ray) const {
	HitRecord rec;
	return this->intersect(ray, rec);
}
}
