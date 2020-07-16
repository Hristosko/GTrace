#include "Shape.h"

void Shape::parseTransform(std::unordered_map<std::string, std::string>& map) {
	Vector3f translation, rotation;
	SceneParser& p = getParser();
	p.parseVector3fAndStore(map, "position", translation);
	p.parseVector3fAndStore(map, "rotation", rotation);
	const Matrix4x4 m = Transform::makeTranslation(translation) *
		Transform::makeRotation(rotation.x(), rotation.y(), rotation.z());
	this->objectToWorld = Ref<Transform>(new Transform(m));
}
