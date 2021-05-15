#pragma once
#include <vector>

#include "geometry/Shape.h"
#include "geometry/Mesh.h"
#include "texture/Texture.h"
#include "material/Material.h"
#include "light/Light.h"

#include "camera/Camera.h"

#include "SceneSettings.h"
#include "Parser.h"

namespace gtrace {

class World {
public:
	template<typename T>
	using Elements = std::vector<std::unique_ptr<T>>;

	template<typename T>
	using NamedElements = std::unordered_map<std::string, std::unique_ptr<T>>;

	World();
	void clear();

	void addElemenet(std::unique_ptr<SceneElement>&& el, const std::unordered_map<std::string, std::string>& map);
	void add(std::unique_ptr<Shape>&& shape) { this->shapes.push_back(std::move(shape)); }

	const Elements<Shape>& getShapes() const { return this->shapes; }
	const Elements<Light>& getLights() const { return this->lights; }
	SceneSettings& getSettings() { return *this->settings.get(); }
	const SceneSettings& getSettings() const { return *this->settings.get(); }

	const Camera& getCamera() const { return *this->camera.get(); }
	void setCamera(std::unique_ptr<Camera>&& camera) { this->camera = std::move(camera); }

	Texture* getTextureByName(const std::string& name);
	void addTexture(const std::string& name, std::unique_ptr<Texture>&& text);

	Material* getMaterialByName(const std::string& name);
	void addMaterial(const std::string& name, std::unique_ptr<Material>&& mat);

	Shape* getBVH() { return this->bvh.get(); }
	void buildBVH();

	bool intersect(const Ray& ray, HitRecord& rec, float time = 0.f) const;
	bool intersect(const Ray& ray) const;
private:
	std::unique_ptr<Shape> bvh;
	NamedElements<Texture> textures;
	NamedElements<Material> materials;
	Elements<Shape> shapes;
	Elements<Light> lights;
	Elements<Mesh> meshes;
	std::unique_ptr<SceneSettings> settings;
	std::unique_ptr<Camera> camera;
};
}
