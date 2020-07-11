#include "Mesh.h"
#include "../Logger.h"
#include "../Errors.h"
#include "../StringUtils.h"
#include "../FileUtils.h"
#include "Triangle.h"

#define OBJ_FILE_EXTENSION ".obj"

// OBJ file strings and names
#define OBJ_MAX_LINE_LENGTH 200
#define OBJ_COMMENT '#'
#define OBJ_VERTEX_FORMAT "v %f %f %f"
#define OBJ_NORMAL_FORMAT "vn %f %f %f"
#define OBJ_FACE_FORMAT "f %u//%u %u//%u %u//%u"

void Mesh::parse(std::unordered_map<std::string, std::string>& map) {
	SceneParser& p = getParser();
	p.parseMaterialAndStore(map, "mat", this->mat);
	bool useNormals;
	p.parseBoolAndStore(map, "use_normals", useNormals);
	auto it = map.find("file");
	if (it == map.end()) {
		LOGERROR("Mesh source file is not specified.");
		throw ParseError();
	}
	if (endsWith(it->second, OBJ_FILE_EXTENSION)) this->loadFromObjFile(it->second.c_str(), useNormals);
}

static void checkIfVertexExists(const std::deque<Vector3fData>& data, unsigned idx) {
	if (data.size() < idx)
		LOGWARNING("Unknown vertex: ", idx);
}

static void checkIfNormalExists(const std::deque<Vector3fData>& data, unsigned idx) {
	if (data.size() < idx)
		LOGWARNING("Unknown normal: ", idx);
}

bool Mesh::hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const {
	float tval, beta, gamma;
	bool hasHit = false;
	for (const MeshTriangle& tr : this->faces) {
		const Vector3f a(this->vertices[tr.i]);
		const Vector3f b(this->vertices[tr.j]);
		const Vector3f c(this->vertices[tr.k]);
		if (Triangle::hit(a, b, c, ray, tmin, tmax, beta, gamma, tval)) {
			hasHit = true;
			tmax = rec.t = tval;
			rec.normal = normalize(cross(b - a, c - a));
		}
	}
	if (hasHit) rec.mat = this->mat;
	return hasHit;
}

void Mesh::loadFromObjFile(const char* path, bool useNormals) {
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
	while ((len = readLine(buffer, OBJ_MAX_LINE_LENGTH, fp)) >= 0) {
		if (len == 0 || buffer[0] == OBJ_COMMENT) continue;
		res = sscanf_s(buffer, OBJ_VERTEX_FORMAT, &a, &b, &c);
		if (res == 3) {
			this->vertices.push_back(Vector3fData(a, b, c));
			continue;
		}

		res = sscanf_s(buffer, OBJ_NORMAL_FORMAT, &a, &b, &c);
		if (res == 3) {
			if (useNormals) this->normals.push_back(Vector3fData(a, b, c));
			continue;
		}

		res = sscanf_s(buffer, OBJ_FACE_FORMAT, &i, &ni, &j, &nj, &k, &nk);
		if (res == 6) {
			if (useNormals) {
				checkIfNormalExists(this->normals, ni);
				checkIfNormalExists(this->normals, nj);
				checkIfNormalExists(this->normals, nk);

				this->facesNormals.push_back({ ni-1, nj-1, nk-1 });
			}
			checkIfVertexExists(this->vertices, i);
			checkIfVertexExists(this->vertices, j);
			checkIfVertexExists(this->vertices, k);
			
			this->faces.push_back({ i-1, j-1, k-1 });
			continue;
		}

		LOGWARNING("Unrecognized obj data: ", buffer);
	}
	LOGINFO("Parsed OBJ file ", path,
		" Vertecies: ", this->vertices.size(),
		" Normals: ", this->normals.size(),
		" Faces: ", this->faces.size(),
		" Faces Normals: ", this->facesNormals.size());
}
