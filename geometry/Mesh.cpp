#include "Mesh.h"
#include "../Logger.h"
#include "../Errors.h"
#include "../StringUtils.h"
#include "../FileUtils.h"

#define OBJ_FILE_EXTENSION ".obj"

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

bool Mesh::hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const {
	return false;
}

void Mesh::loadFromObjFile(const char* path, bool useNormals) {
	FILE* fp;
	if (0 != fopen_s(&fp, path, "r")) {
		LOGERROR("Cannot open file: ", path);
		throw FileError();
	}
	FileRAII fr(fp);
	LOGINFO("Start parsing obj file: ", path);

	LOGINFO("Finish parsing obj file: ", path);
}
