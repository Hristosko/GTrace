#pragma once
#include <vector>

#include "../geometry/Shape.h"
#include "../texture/Texture.h"
#include "../material/Material.h"
#include "../light/Light.h"

#include "../camera/Camera.h"

#include "SceneSettings.h"
#include "Parser.h"

namespace gtrace {

class Mesh;

class World {
public:
	World();
	~World();

	void clear();

	void addElemenet(SceneElement* el, const std::unordered_map<std::string, std::string>& map);
	void add(Shape* shape);

	const std::vector<Shape*>& getShapes() const { return this->shapes; }
	const std::vector<Light*>& getLights() const { return this->lights; }
	SceneSettings& getSettings() { return this->settings; }
	const SceneSettings& getSettings() const { return this->settings; }

	Camera* getCamera() { return this->camera.get(); }
	void setCamera(Camera* camera) { this->camera = std::unique_ptr<Camera>(camera); }

	Texture* getTextureByName(const std::string& name);
	void addTexture(const std::string& name, Texture* text);

	Material* getMaterialByName(const std::string& name);
	void addMaterial(const std::string& name, Material* mat);

	Shape* getBVH() { return this->bvh; }
	void buildBVH();

	bool intersect(const Ray& ray, HitRecord& rec, float time = 0.f) const;
	bool intersect(const Ray& ray) const;
private:
	Shape* bvh;
	std::unordered_map<std::string, Texture*> textures;
	std::unordered_map<std::string, Material*> materials;
	std::vector<Shape*> shapes;
	std::vector<Light*> lights;
	std::vector<Mesh*> meshes;
	SceneSettings settings;
	std::unique_ptr<Camera> camera;
};
}
