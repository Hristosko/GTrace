#include "Piramid.h"
#include "Triangle.h"

void Piramid::parse(std::unordered_map<std::string, std::string>& map) {
	SceneParser& p = getParser();
	this->mesh.vertices.reserve(4);
	p.parseVector3fAndStore(map, "vert1", this->mesh.vertices[0]);
	p.parseVector3fAndStore(map, "vert2", this->mesh.vertices[1]);
	p.parseVector3fAndStore(map, "vert3", this->mesh.vertices[2]);
	p.parseVector3fAndStore(map, "vert4", this->mesh.vertices[3]);
	p.parseMaterialAndStore(map, "mat", this->mesh.mat);

	this->faces[0] = { 0,1,2 };
	this->faces[1] = { 3,2,1 };
	this->faces[2] = { 1,0,3 };
	this->faces[3] = { 1,2,3 };
}

bool Piramid::hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const {
	bool isHit = false;
	for (int i = 0; i < facesCount; ++i) {
		const Triangle tr(this->mesh.vertices[this->faces[i].i],
			this->mesh.vertices[this->faces[i].j],
			this->mesh.vertices[this->faces[i].k]);
		if (tr.hit(ray, tmin, tmax, time, rec)) {
			isHit = true;
			tmax = rec.t;
			rec.mat = this->mesh.mat;
		}
	}
	return isHit;
}
