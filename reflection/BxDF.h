#pragma once

enum class BxDFType {
	Reflection = 1 << 0,
	Transmission = 1 << 1,
	Diffuse = 1 << 2,
	Glossy = 1 << 3,
	Specular = 1 << 4,
	AllTypes = Diffuse | Glossy | Specular,
	AllReflection = AllTypes | Reflection,
	AllTransmission = AllTypes | Transmission,
	All = AllReflection | AllTransmission
};

class BxDF {
public:
	BxDF(BxDFType t) : type(t) {}
protected:
	BxDFType type;
};
