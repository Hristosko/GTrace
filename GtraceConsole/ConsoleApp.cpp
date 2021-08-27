#include "renderer/RendererOutput.h"
#include "renderer/Renderer.h"
#include "scene/Parser.h"
#include "scene/World.h"
#include "MemoryBench.h"
#include "Logger.h"

#include <stdio.h>

static uint32_t totalBucketsCount = 0;

static void updateProgress() {
	static uint32_t renderedBucketsCount = 0;
	if (totalBucketsCount <= 0) return;

	++renderedBucketsCount;
	// we update the render surface once again when everything is rendered
	if (renderedBucketsCount > totalBucketsCount) return;

	printf("\r");
	printf("[%u/%u] %u%%",
		renderedBucketsCount, totalBucketsCount,
		renderedBucketsCount*100 / totalBucketsCount);

	if (renderedBucketsCount == totalBucketsCount) printf("\n");
}

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
		gtrace::Renderer renderer(updateProgress, output, world);
		totalBucketsCount = renderer.totalBucketsCount();
		renderer.render();
		output.save(outputPath);
	}
	gtrace::MemoryBench::logData();
	return 0;
}
