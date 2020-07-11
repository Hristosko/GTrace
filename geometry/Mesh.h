#pragma once

#include "../math/Vector3f.h"
#include "../material/Material.h"
#include <vector>

struct Mesh {
	Material* mat;
	std::vector<Vector3fData> vertices;
};

struct MeshTriangle {
	uint32_t i, j, k;
};