#pragma once
#include "common/Common.h"
#include "geometry/MeshData.h"

using namespace gtrace;

using UseNormals = Bool;

RawMesh generateMeshSphere(int subdiveCount = 0, UseNormals genNormals = UseNormals::False);
void generateMeshSohereNormals(RawMesh* mesh);
