#include "renderer/RendererOutput.h"
#include "renderer/Renderer.h"
#include "scene/Parser.h"
#include "scene/World.h"
#include "MemoryBench.h"
#include "Logger.h"

#include <stdio.h>

int main(int argc, char** argv) {
	if (argc != 3) {
		printf("ARG1 scene, ARG2 output file\n");
		return 1;
	}
	gtrace::MemoryBench::reset();
	{
		gtrace::World world;
		const char* inputPath = argv[1];
		const char* outputPath = argv[2];
		gtrace::RendererOutput output(world);
		output.init();
		gtrace::SceneParser p(world);
		p.parseFile(inputPath);
		world.buildBVH();
		auto noopUpdater = []() {};
		gtrace::Renderer renderer(noopUpdater, output, world);
		renderer.render();
		output.save(outputPath);
	}
	gtrace::MemoryBench::logData();
	return 0;
}
