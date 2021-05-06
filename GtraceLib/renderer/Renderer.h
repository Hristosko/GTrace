#pragma once

#include "RendererOutput.h"
#include "RendererStat.h"
#include "../Threaded.h"
#include "../math/Vector3f.h"
#include "../Random.h"

using UpdateRenderSurfaceFunc = void(*)();

class Renderer {
private:
	class ThreadedRenderer : public Threaded {
	public:
		ThreadedRenderer(Renderer& r) : renderer(r), curx(0), cury(0) {}
		void run(unsigned threadIdx, unsigned numThreads) override;
		bool getNextBucket(uint32_t& cx, uint32_t& cy);

	private:
		static constexpr uint32_t bucketWidth = 20;
		static constexpr uint32_t bucketHeight = 20;
		Renderer& renderer;
		// The next bucket that has to be rendered
		uint32_t curx, cury;
		// sync. the rendering threads, when asking for the next bucket
		std::mutex mut;
	};
public:
	Renderer(UpdateRenderSurfaceFunc updater, RendererOutput& output)
		: updateRenderSurfaceFunc(updater), output(output) {}

	void render();
private:
	void rayTrace(uint32_t ix, uint32_t iy);
	void rayTraceWithSamples(uint32_t ix, uint32_t iy, uint32_t samples,
							Vector3f& sum, Vector3f& sumSqr, uint32_t& totalSamples);
	void renderBucket(uint32_t offsetx, uint32_t offsety, uint32_t bucketWidth, uint32_t bucketHeight);
	
	void updateRenderSurface();
private:
	UpdateRenderSurfaceFunc updateRenderSurfaceFunc;
	RendererOutput& output;
	RandomGenerator rng;
	RendererStat stat;
};
