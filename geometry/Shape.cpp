#include "Shape.h"

Ref<Transform> Shape::parseTranformComponents(std::unordered_map<std::string, std::string>& map) {
	Vector3f translation, rotation;
	SceneParser::parseVector3fAndStore(map, "position", translation);
	SceneParser::parseVector3fAndStore(map, "rotation", rotation);
	const Matrix4x4 m = Transform::makeTranslation(translation) *
		Transform::makeRotation(rotation.x(), rotation.y(), rotation.z());
	return new Transform(m);
}

void Shape::parseTransform(std::unordered_map<std::string, std::string>& map) {
	this->objectToWorld = this->parseTranformComponents(map);
}
