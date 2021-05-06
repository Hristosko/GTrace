#include "renderer/RendererOutput.h"
#include "renderer/Renderer.h"
#include "scene/Parser.h"
#include "scene/World.h"
#include <string>

int main(int argc, char** argv) {
	const char* inputPath = argv[1];
	const char* outputPath = argv[2];
	RendererOutput output;
	output.init();
	SceneParser::parseFile(inputPath);
	getWorld().buildBVH();
	auto noopUpdater = []() {};
	Renderer renderer(noopUpdater, output);
	renderer.render();
	output.save(outputPath);
	return 0;
}
