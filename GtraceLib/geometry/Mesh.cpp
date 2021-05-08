#include "Mesh.h"
#include "BBox.h"
#include "../Logger.h"
#include "../Errors.h"
#include "../StringUtils.h"
#include "../FileUtils.h"
#include "Triangle.h"
#include "../scene/World.h"

#define OBJ_FILE_EXTENSION ".obj"

// OBJ file strings and names
#define OBJ_MAX_LINE_LENGTH 200
#define OBJ_COMMENT '#'
#define OBJ_VERTEX_FORMAT "v %f %f %f"
#define OBJ_NORMAL_FORMAT "vn %f %f %f"
#define OBJ_FACE_FORMAT "f %u//%u %u//%u %u//%u"

namespace gtrace {

void Mesh::parse(const SceneParser& parser, std::unordered_map<std::string, std::string>& map) {
	Ref<Transform> tr = Shape::parseTranformComponents(parser, map);
	parser.parseMaterialAndStore(map, "mat", this->mat);
	bool useNormals;
	parser.parseBoolAndStore(map, "use_normals", useNormals);
	auto it = map.find("file");
	if (it == map.end()) {
		LOGERROR("Mesh source file is not specified.");
		throw ParseError();
	}
	if (endsWith(it->second, OBJ_FILE_EXTENSION))
		this->loadFromObjFile(parser.getWorld(), it->second.c_str(), useNormals, tr);
}

static void checkIfVertexExists(const std::deque<Vector3f>& data, unsigned idx) {
	if (data.size() < idx)
		LOGWARNING("Unknown vertex: ", idx);
}

static void checkIfNormalExists(const std::deque<Vector3f>& data, unsigned idx) {
	if (data.size() < idx)
		LOGWARNING("Unknown normal: ", idx);
}

void Mesh::loadFromObjFile(World& w, const char* path, bool useNormals, Ref<Transform>& tr) {
	FILE* fp;
	if (0 != fopen_s(&fp, path, "r")) {
		LOGERROR("Cannot open file: ", path);
		throw FileError();
	}
	FileRAII fr(fp);
	char buffer[OBJ_MAX_LINE_LENGTH];
	LOGINFO("Start parsing obj file: ", path);
	int len;
	int res;
	float a, b, c;
	unsigned i, j, k, ni, nj, nk;

	uint32_t faces = 0, facesNormals = 0;
	while ((len = readLine(buffer, OBJ_MAX_LINE_LENGTH, fp)) >= 0) {
		if (len == 0 || buffer[0] == OBJ_COMMENT) continue;
		res = sscanf_s(buffer, OBJ_VERTEX_FORMAT, &a, &b, &c);
		if (res == 3) {
			this->vertices.push_back(Vector3f(a, b, c));
			continue;
		}

		res = sscanf_s(buffer, OBJ_NORMAL_FORMAT, &a, &b, &c);
		if (res == 3) {
			if (useNormals) this->normals.push_back(Vector3f(a, b, c));
			continue;
		}

		res = sscanf_s(buffer, OBJ_FACE_FORMAT, &i, &ni, &j, &nj, &k, &nk);
		if (res == 6) {
			++faces;
			checkIfVertexExists(this->vertices, i);
			checkIfVertexExists(this->vertices, j);
			checkIfVertexExists(this->vertices, k);

			if (useNormals) {
				++facesNormals;
				checkIfNormalExists(this->normals, ni);
				checkIfNormalExists(this->normals, nj);
				checkIfNormalExists(this->normals, nk);

				MeshElementWithNormal* el = new
					MeshElementWithNormal(tr, this,
						i-1, j-1, k-1,
						ni-1, nj-1, nk-1);
				w.add(el);
				continue;
			}
			// add the new triangle to the world
			// The indexing in the obj file starts from 1
			MeshElement* el = new MeshElement(tr, this, i-1, j-1, k-1);
			w.add(el);
			continue;
		}

		LOGWARNING("Unrecognized obj data: ", buffer);
	}
	LOGINFO("Parsed OBJ file ", path,
		" Vertecies: ", this->vertices.size(),
		" Normals: ", this->normals.size(),
		" Faces: ", faces,
		" Faces Normals: ", facesNormals);
}

bool MeshElement::hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const {
	float tval, beta, gamma;
	bool hasHit = false;
	const Vector3f a(this->mesh->vertices[tr.i]);
	const Vector3f b(this->mesh->vertices[tr.j]);
	const Vector3f c(this->mesh->vertices[tr.k]);
	if (Triangle::hit(a, b, c, this->objectToWorld.get(), ray, tmin, tmax, beta, gamma, tval)) {
		hasHit = true;
		tmax = rec.t = tval;
		rec.normal = this->objectToWorld->transformDirection((cross(b - a, c - a)));
	}
	if (hasHit) rec.mat = this->mesh->mat;
	return hasHit;
}

BBox MeshElement::bbox() const {
	const Vector3f a(this->mesh->vertices[tr.i]);
	const Vector3f b(this->mesh->vertices[tr.j]);
	const Vector3f c(this->mesh->vertices[tr.k]);
	return Triangle::triangleBBox(a, b, c, this->objectToWorld.get());
}

bool MeshElementWithNormal::hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const {
	float tval, beta, gamma;
	bool hasHit = false;
	const Vector3f a(this->mesh->vertices[tr.i]);
	const Vector3f b(this->mesh->vertices[tr.j]);
	const Vector3f c(this->mesh->vertices[tr.k]);
	if (Triangle::hit(a, b, c, this->objectToWorld.get(), ray, tmin, tmax, beta, gamma, tval)) {
		hasHit = true;
		tmax = rec.t = tval;
		const float alpha = 1.f - beta - gamma;
		// interpolate the normals of the model
		rec.normal = this->mesh->normals[normals.i] * alpha
			+ this->mesh->normals[normals.j] * beta
			+ this->mesh->normals[normals.k] * gamma;
		rec.normal = this->objectToWorld->transformDirection(rec.normal);
	}
	if (hasHit) rec.mat = this->mesh->mat;
	return hasHit;
}
}
