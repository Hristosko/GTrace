#include "gtest/gtest.h"
#include "Utils.h"
#include "math/Utils.h"

using namespace gtrace;

TEST(SphericalCoordinates, Theta) {
	// The spherical coordinates are: (r=1, theta = 30, phi = 60)
	Vector3f vec(0.25f, 0.4330127019f, 0.8660254038f);
	vec = normalize(vec);

	float theta = degToRad(30);
	ASSERT_TRUE(compare(cosf(theta), cosTheta(vec)));
	ASSERT_TRUE(compare(sinf(theta), sinTheta(vec)));
	ASSERT_TRUE(compare(sinf(theta)*sinf(theta), sin2Theta(vec)));

	// The spherical coordinates are: (r=1, theta = 30, phi = 120)
	vec = normalize(Vector3f(-0.25f, 0.4330127019f, 0.8660254038f));
	theta = degToRad(30);
	ASSERT_TRUE(compare(cosf(theta), cosTheta(vec)));
	ASSERT_TRUE(compare(sinf(theta), sinTheta(vec)));
	ASSERT_TRUE(compare(sinf(theta) * sinf(theta), sin2Theta(vec)));
}

TEST(SphericalCoordinates, Theta2ndQuad) {
	// The spherical coordinates are: (r=1, theta = 150, phi = 60)
	Vector3f vec(0.25f, 0.4330127019f, -0.8660254038f);
	vec = normalize(vec);

	const float theta = degToRad(150);
	ASSERT_TRUE(compare(cosf(theta), cosTheta(vec)));
	ASSERT_TRUE(compare(sinf(theta), sinTheta(vec)));
	ASSERT_TRUE(compare(sinf(theta) * sinf(theta), sin2Theta(vec)));
}

TEST(SphericalCoordinates, Phi) {
	// The spherical coordinates are: (r=1, theta = 30, phi = 60)
	Vector3f vec(0.25f, 0.4330127019f, 0.8660254038f);
	vec = normalize(vec);

	const float theta = degToRad(60);
	ASSERT_TRUE(compare(cosf(theta), cosPhi(vec)));
	ASSERT_TRUE(compare(sinf(theta), sinPhi(vec)));
}

TEST(SphericalCoordinates, Phi2ndQuad) {
	// The spherical coordinates are: (r=1, theta = 30, phi = 120)
	Vector3f vec(-0.25f, 0.4330127019f, 0.8660254038f);
	vec = normalize(vec);

	const float theta = degToRad(120);
	ASSERT_TRUE(compare(cosf(theta), cosPhi(vec)));
	ASSERT_TRUE(compare(sinf(theta), sinPhi(vec)));
}

TEST(SphericalCoordinates, Phi3rdQuad) {
	// The spherical coordinates are: (r=1, theta = 30, phi = 240)
	Vector3f vec(-0.25f, -0.4330127019f, 0.8660254038f);
	vec = normalize(vec);

	const float theta = degToRad(240);
	ASSERT_TRUE(compare(cosf(theta), cosPhi(vec)));
	ASSERT_TRUE(compare(sinf(theta), sinPhi(vec)));
}

TEST(SphericalCoordinates, Phi4rdQuad) {
	// The spherical coordinates are: (r=1, theta = 30, phi = 300)
	Vector3f vec(0.25f, -0.4330127019f, 0.8660254038f);
	vec = normalize(vec);

	const float theta = degToRad(300);
	ASSERT_TRUE(compare(cosf(theta), cosPhi(vec)));
	ASSERT_TRUE(compare(sinf(theta), sinPhi(vec)));
}
