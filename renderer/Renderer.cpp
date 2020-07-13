#pragma warning(disable : 4996)

#include "Renderer.h"
#include "../Events.h"
#include "../Logger.h"

#include "../scene/World.h"

void Renderer::render() {
	LOGINFO("Start rendering.");
	
	ThreadedRenderer tr(*this);
	ThreadManager tman(std::thread::hardware_concurrency());
	tman.run(&tr);

	LOGINFO("Finish rendering.");
	this->updateRenderSurface();
}

void Renderer::renderBucket(uint32_t offsetx, uint32_t offsety, uint32_t bucketWidth, uint32_t bucketHeight) {
	for (uint32_t iy = 0; iy < bucketHeight; ++iy) {
		const uint32_t realiy = iy + offsety;
		for (uint32_t ix = 0; ix < bucketWidth; ++ix) {
			const uint32_t realix = ix + offsetx;
			this->rayTrace(realix, realiy);
		}
	}
}

void Renderer::updateRenderSurface() {
	wxCommandEvent* event = new wxCommandEvent(GTRACE_RENDERED_ELEMENT);
	wxQueueEvent(this->renderSurface, event);
}

void Renderer::rayTrace(uint32_t ix, uint32_t iy) {
	Vector3f res;
	Vector3f var;

	Vector3f sum(0.f);
	Vector3f sumSqr(0.f);
	uint32_t totalSamples = 0;
	
	const uint32_t maxSubdivs = getWorld().getSettings().maxSubdivs;
	const Vector3f varTreshhold(getWorld().getSettings().stdTreshhold * getWorld().getSettings().stdTreshhold);
	uint32_t curSubdivs = 1;
	do {
		this->rayTraceWithSamples(ix, iy, curSubdivs + 1, sum, sumSqr, totalSamples);
		const float denom = 1.f / totalSamples;
		res = sum*denom; // the mean value
		var = sumSqr * denom - res * res;
		++curSubdivs;
	} while (curSubdivs >= maxSubdivs && varTreshhold < var);

	// Store the results
	{
		DataBuffer& buffer = this->output.getOutput(RendererOutputType::Image);
		ColorResult* ptr = reinterpret_cast<ColorResult*>(
			buffer.ptrByIdx((uint64_t)iy * getWorld().getSettings().width + (uint64_t)ix));
		ptr->r = static_cast<char>(255.f * res.x());
		ptr->g = static_cast<char>(255.f * res.y());
		ptr->b = static_cast<char>(255.f * res.z());
	}

	{
		DataBuffer& buffer = this->output.getOutput(RendererOutputType::Variance);
		VarianceResult* ptr = reinterpret_cast<VarianceResult*>(
			buffer.ptrByIdx((uint64_t)iy * getWorld().getSettings().width + (uint64_t)ix));
		ptr->x = var.x();
		ptr->y = var.y();
		ptr->z = var.z();
	}
}

void Renderer::rayTraceWithSamples(
	uint32_t ix, uint32_t iy, uint32_t samples,
	Vector3f& sum, Vector3f& sumSqr, uint32_t& totalSamples) {
	
	const int64_t camx = (int64_t)ix - getWorld().getSettings().width / 2;
	const int64_t camy = (int64_t)iy - getWorld().getSettings().height / 2;
	const float denom = 1.f / samples;
	Camera& camera = getWorld().getCamera();

	for (uint32_t sx = 0; sx < samples; ++sx) {
		for (uint32_t sy = 0; sy < samples; ++sy) {
			const float dx = ((float)sx + this->rng.get()) * denom - 0.5f;
			const float dy = ((float)sy + this->rng.get()) * denom - 0.5f;

			Ray ray = camera.castRay(camx + dx, -(camy + dy));
			ray.renderer = this;
			HitRecord rec;
			rec.mat = getWorld().getSettings().background;
			rec.t = 1000000.f;
			Shape * bvh = getWorld().getBVH();
			bvh->hit(ray, 0.f, rec.t, 0, rec);

			rec.position = ray.origin + rec.t * ray.direction;
			const Vector3f color = (rec.mat == nullptr) ?
				DEFAULT_TEXTURE_VALUE :
				rec.mat->shade(rec, ray);
			sum += color;
			sumSqr += (color * color);
		}
	}
	totalSamples += (samples * samples);
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
