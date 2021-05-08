#include "renderer/RendererOutput.h"
#include "renderer/Renderer.h"
#include "scene/Parser.h"
#include "scene/World.h"
#include "MemoryBench.h"
#include "Logger.h"

int main(int argc, char** argv) {
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
	const gtrace::MemoryBench::Data mb = gtrace::MemoryBench::get();
	LOGSTAT("Total alocated memory: ", mb.totalAllocatedMemory, "B ", (float)mb.totalAllocatedMemory / (1024 * 1024), "MB");
	LOGSTAT("Peak memory usage: ", mb.peakMemoryUsage, "B ", (float)mb.peakMemoryUsage / (1024 * 1024), "MB");
	LOGSTAT("Alocations count: ", mb.allocationCount);
	LOGSTAT("Freed allocations count: ", mb.freeCount);
	return 0;
}
