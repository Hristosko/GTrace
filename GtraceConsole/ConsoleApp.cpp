#include "renderer/RendererOutput.h"
#include "renderer/Renderer.h"
#include "scene/Parser.h"
#include "scene/World.h"
#include "MemoryBench.h"
#include "Logger.h"
#include <string>

int main(int argc, char** argv) {
	MemoryBench::reset();
	{
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
		getWorld().clear();
	}
	const MemoryBench::Data mb = MemoryBench::get();
	LOGSTAT("Total alocated memory: ", mb.totalAllocatedMemory, "B ", (float)mb.totalAllocatedMemory / (1024 * 1024), "MB");
	LOGSTAT("Peak memory usage: ", mb.peakMemoryUsage, "B ", (float)mb.peakMemoryUsage / (1024 * 1024), "MB");
	LOGSTAT("Alocations count: ", mb.allocationCount);
	LOGSTAT("Freed allocations count: ", mb.freeCount);
	return 0;
}
