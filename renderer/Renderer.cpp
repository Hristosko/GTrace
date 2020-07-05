#pragma warning(disable : 4996)

#include "Renderer.h"
#include "../Events.h"
#include "../Logger.h"

#include "../scene/World.h"

#include<iostream>
void Renderer::render() {
	LOGINFO("Start rendering.");
	
	ThreadedRenderer tr(*this);
	ThreadManager tman(std::thread::hardware_concurrency());
	tman.run(&tr);

	LOGINFO("Finish rendering.");
	this->updateRenderSurface();
}

void Renderer::renderBucket(uint32_t offsetx, uint32_t offsety, uint32_t bucketWidth, uint32_t bucketHeight) {
	DataBuffer& buffer = this->output.getOutput(RendererOutputType::Image);
	for (uint32_t iy = 0; iy < bucketHeight; ++iy) {
		const uint32_t realiy = iy + offsety;
		for (uint32_t ix = 0; ix < bucketWidth; ++ix) {
			const uint32_t realix = ix + offsetx;
			const Vector3f col = this->rayTrace(realix, realiy);

			ColorResult* ptr = reinterpret_cast<ColorResult*>(
				buffer.ptrByIdx((uint64_t)realiy * getWorld().getSettings().width + (uint64_t)realix));
			ptr->r = static_cast<char>(255.f * col.x());
			ptr->g = static_cast<char>(255.f * col.y());
			ptr->b = static_cast<char>(255.f * col.z());
		}
	}
}

void Renderer::updateRenderSurface() {
	wxCommandEvent* event = new wxCommandEvent(GTRACE_RENDERED_ELEMENT);
	wxQueueEvent(this->renderSurface, event);
}

Vector3f Renderer::rayTrace(uint32_t ix, uint32_t iy) {
	const uint32_t samples = 4;
	Ray ray;
	ray.origin = Vector3f(ix, iy, -10);
	ray.direction = Vector3f(0, 0, 1);

	HitRecort rec;
	rec.color = getWorld().getSettings().background;
	rec.t = 1000000.f;
	for (Shape* shape : getWorld().getShapes()) {
		shape->hit(ray, 0.f, rec.t, 0, rec);
	}
	return rec.color;
}

void Renderer::ThreadedRenderer::run(unsigned threadIdx, unsigned numThreads) {
	const uint32_t bucketWidth = 50;
	const uint32_t bucketHeight = 50;

	uint32_t pos = 0;
	for (uint32_t curx = 0; curx < getWorld().getSettings().width; curx += bucketWidth) {
		for (uint32_t cury = 0; cury < getWorld().getSettings().height; cury += bucketHeight) {
			++pos;
			if ((pos - 1) % numThreads != threadIdx) continue;

			const uint32_t curWidth = std::min(getWorld().getSettings().width - curx, bucketWidth);
			const uint32_t curHeight = std::min(getWorld().getSettings().height - cury, bucketHeight);
			this->renderer.renderBucket(curx, cury, curWidth, curHeight);
			this->renderer.updateRenderSurface();
		}
	}
}
