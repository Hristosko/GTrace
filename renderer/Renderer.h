#pragma once

#include "RendererOutput.h"
#include "../Threaded.h"
#include "../math/Vector3f.h"
#include "../Random.h"

#include "wx/wx.h"

class Renderer {
private:
	class ThreadedRenderer : public Threaded {
	public:
		ThreadedRenderer(Renderer& r) : renderer(r) {}
		void run(unsigned threadIdx, unsigned numThreads) override;
	private:
		Renderer& renderer;
	};
public:
	Renderer(wxWindow* renderSurface, RendererOutput& output)
		: renderSurface(renderSurface), output(output) {}

	void render();
private:
	void rayTrace(uint32_t ix, uint32_t iy);
	void renderBucket(uint32_t offsetx, uint32_t offsety, uint32_t bucketWidth, uint32_t bucketHeight);
	
	void updateRenderSurface();
private:
	wxWindow* renderSurface;
	RendererOutput& output;
	RandomGenerator rng;
};

