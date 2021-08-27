#pragma once

#include "RendererOutput.h"
#include "RendererStat.h"
#include "Threaded.h"
#include "math/Vector3f.h"
#include "Random.h"
#include "integrator/Integrator.h"
#include "integrator/DirectLightIntegrator.h"
#include <functional>

namespace gtrace {

using UpdateRenderSurfaceFunc = std::function<void()>;

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
	Renderer(UpdateRenderSurfaceFunc updater, RendererOutput& output, World& w)
		: updateRenderSurfaceFunc(updater), output(output), world(w),
		integrator(std::make_unique<DirectLightIntegrator>()) {}

	void render();
private:
	void rayTrace(uint32_t realix, uint32_t realiy,
		RendererOutput::OutputBuffer& buff, uint32_t localix, uint32_t localiy, uint32_t localWidth);
	void rayTraceWithSamples(uint32_t ix, uint32_t iy, uint32_t samples,
							Vector3f& sum, Vector3f& sumSqr,
							uint32_t& totalSamples, SecondaryRaysStat& stat);
	void renderBucket(uint32_t offsetx, uint32_t offsety, uint32_t bucketWidth, uint32_t bucketHeight);

	void updateRenderSurface();
private:
	UpdateRenderSurfaceFunc updateRenderSurfaceFunc;
	std::mutex outputUpdateMutex;
	RendererOutput& output;
	World& world;
	std::unique_ptr<Integrator> integrator;
	RandomGenerator rng;
	RendererStat stat;
};
}
