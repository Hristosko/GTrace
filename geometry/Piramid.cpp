#include "Piramid.h"
#include "Triangle.h"

void Piramid::parse(std::unordered_map<std::string, std::string>& map) {
	SceneParser& p = getParser();
	this->vertices.reserve(4);
	p.parseVector3fAndStore(map, "vert1", this->vertices[0]);
	p.parseVector3fAndStore(map, "vert2", this->vertices[1]);
	p.parseVector3fAndStore(map, "vert3", this->vertices[2]);
	p.parseVector3fAndStore(map, "vert4", this->vertices[3]);
	p.parseMaterialAndStore(map, "mat", this->mat);

	this->faces[0] = { 0,1,2 };
	this->faces[1] = { 3,2,1 };
	this->faces[2] = { 1,0,3 };
	this->faces[3] = { 1,2,3 };
}

bool Piramid::hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const {
	bool isHit = false;
	for (int i = 0; i < facesCount; ++i) {
		const Triangle tr(this->vertices[this->faces[i].i],
			this->vertices[this->faces[i].j],
			this->vertices[this->faces[i].k]);
		if (tr.hit(ray, tmin, tmax, time, rec)) {
			isHit = true;
			tmax = rec.t;
			rec.mat = this->mat;
		}
	}
	return isHit;
}
