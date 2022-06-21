#include "common/Logger.h"
#include "common/MemoryTracker.h"
#include "renderer/Renderer.h"

#include <stdio.h>

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

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
	gtrace::MemoryTracker::reset();
	{
		gtrace::RendererOutput output;
		gtrace::Scene scene;
        scene.sceneSettings = {DEFAULT_WIDTH, DEFAULT_HEIGHT};
		gtrace::Renderer renderer(std::move(scene), &output, updateProgress);
    	renderer.render();
	}
	gtrace::MemoryTracker::logStatistic();
	return 0;
}
