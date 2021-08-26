#include "World.h"

#include "Logger.h"
#include "geometry/Triangle.h"
#include "geometry/BVH.h"

#include <limits>

namespace gtrace {

World::World() : settings(std::make_unique<SceneSettings>()) {}

void World::clear() {
	this->bvh = nullptr;
	this->textures.clear();
	this->materials.clear();
	this->shapes.clear();
	this->lights.clear();
	this->meshes.clear();
	this->settings = std::make_unique<SceneSettings>();
	this->camera = nullptr;
}

/**
 * Implement a dynamic_cast fuction for unique pointers.
 * NOTE: It will use a default deleter for the new poiter.
 */
template <typename To, typename From>
std::unique_ptr<To> dynamic_unique_cast(std::unique_ptr<From>&& p)
{
	if (To* cast = dynamic_cast<To*>(p.get()))
	{
		std::unique_ptr<To> result(cast);
		p.release();
		return result;
	}
	return std::unique_ptr<To>(nullptr);
}

void World::addElemenet(std::unique_ptr<SceneElement>&& el, const std::unordered_map<std::string, std::string>& map) {
	// The mesh will store its elements as shapes, the mesh is only kept alive here
	if (auto x = dynamic_unique_cast<Mesh>(std::move(el))) {
		this->meshes.push_back(std::move(x));
	}
	else if (auto x = dynamic_unique_cast<Shape>(std::move(el))) {
		this->shapes.push_back(std::move(x));
	}
	else if (auto x = dynamic_unique_cast<Texture>(std::move(el))) {
		auto it = map.find("name");
		if (it == map.end()) {
			LOGWARNING("Unnamed texture.");
		}
		else {
			this->addTexture(it->second, std::move(x));
		}
	} else if (auto x = dynamic_unique_cast<Material>(std::move(el))) {
		auto it = map.find("name");
		if (it == map.end()) {
			LOGWARNING("Unnamed material.");
		}
		else {
			this->addMaterial(it->second, std::move(x));
		}
	} else if (auto x = dynamic_unique_cast<Light>(std::move(el))) {
		this->lights.push_back(std::move(x));
	} else if (auto x = dynamic_unique_cast<Camera>(std::move(el))) {
		this->setCamera(std::move(x));
	}
}

Texture* World::getTextureByName(const std::string& name) {
	auto it = this->textures.find(name);
	if (it == this->textures.end()) {
		LOGERROR("Searching for unexisting texture: ", name);
		throw std::out_of_range("texture not faund");
	}
	return it->second.get();
}

void World::addTexture(const std::string& name, std::unique_ptr<Texture>&& text) {
	auto it = this->textures.find(name);
	if (it != this->textures.end()) {
		LOGWARNING("Overwriting texture: ", name);
	}
	this->textures[name] = std::move(text);
}

Material* World::getMaterialByName(const std::string& name) {
	auto it = this->materials.find(name);
	if (it == this->materials.end()) {
		LOGERROR("Searching for unexisting mateial: ", name);
		throw std::out_of_range("material not faund");
	}
	return it->second.get();
}

void World::addMaterial(const std::string& name, std::unique_ptr<Material>&& mat) {
	auto it = this->materials.find(name);
	if (it != this->materials.end()) {
		LOGWARNING("Overwriting material: ", name);
	}
	this->materials[name] = std::move(mat);
}

void World::buildBVH() {
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
