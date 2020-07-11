#include "Mesh.h"

void Mesh::parse(std::unordered_map<std::string, std::string>& map) {
	SceneParser& p = getParser();
	p.parseMaterialAndStore(map, "mat", this->mat);

}

bool Mesh::hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const {
	return false;
}
