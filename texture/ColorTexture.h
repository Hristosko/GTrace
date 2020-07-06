#pragma once

#include "Texture.h"

__declspec(align(16)) class ColorTexture : public Texture {
public:
	virtual void parse(std::unordered_map<std::string, std::string>& map) override {
		SceneParser& p = getParser();
		p.parseVector3fAndStore(map, "color", this->color);
	}

	virtual Vector3f getValue(const Vector2f& uv, const Vector3f& p) const override {
		return this->color;
	}

	void* operator new(size_t s) {
		return _mm_malloc(s, 16);;
	}
	void operator delete(void* p) {
		_mm_free(p);
	}
private:
	Vector3f color;
};