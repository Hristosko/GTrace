#pragma warning(disable : 4996)

#include "Renderer.h"
#include "../Logger.h"
#include "../scene/World.h"

void Renderer::render() {
	LOGINFO("Start rendering.");
	this->stat.renderBegin();
	ThreadedRenderer tr(*this);
	ThreadManager tman(std::thread::hardware_concurrency());
	tman.run(&tr);

	this->stat.renderFinish(this->world);
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
	this->updateRenderSurfaceFunc();
}

void Renderer::rayTrace(uint32_t ix, uint32_t iy) {
	Vector3f res;
	Vector3f var;

	Vector3f sum(0.f);
	Vector3f sumSqr(0.f);
	uint32_t totalSamples = 0;

	const uint32_t maxSubdivs = this->world.getSettings().maxSubdivs;
	float tresh = this->world.getSettings().stdTreshhold;
	tresh *= tresh; // we compare it to the variance
	uint32_t curSubdivs = 1;
	do {
		this->rayTraceWithSamples(ix, iy, curSubdivs + 1, sum, sumSqr, totalSamples);
		const float denom = 1.f / totalSamples;
		res = sum*denom; // the mean value
		var = sumSqr * denom - res * res;
		++curSubdivs;
	} while (curSubdivs <= maxSubdivs && var.isAtLeastOneGreaterThan(tresh));

	// Store the results
	this->stat.updateStat(totalSamples, totalSamples);
	{
		DataBuffer& buffer = this->output.getOutput(RendererOutputType::Image);
		ColorResult* ptr = reinterpret_cast<ColorResult*>(
			buffer.ptrByIdx((uint64_t)iy * this->world.getSettings().width + (uint64_t)ix));
		ptr->r = static_cast<char>(255.f * res.x());
		ptr->g = static_cast<char>(255.f * res.y());
		ptr->b = static_cast<char>(255.f * res.z());
	}

	{
		DataBuffer& buffer = this->output.getOutput(RendererOutputType::Variance);
		VarianceResult* ptr = reinterpret_cast<VarianceResult*>(
			buffer.ptrByIdx((uint64_t)iy * this->world.getSettings().width + (uint64_t)ix));
		ptr->x = var.x();
		ptr->y = var.y();
		ptr->z = var.z();
	}
}

void Renderer::rayTraceWithSamples(
	uint32_t ix, uint32_t iy, uint32_t samples,
	Vector3f& sum, Vector3f& sumSqr, uint32_t& totalSamples) {

	const int64_t camx = (int64_t)ix - this->world.getSettings().width / 2;
	const int64_t camy = (int64_t)iy - this->world.getSettings().height / 2;
	const float denom = 1.f / samples;
	const Camera* camera = this->world.getCamera();

	for (uint32_t sx = 0; sx < samples; ++sx) {
		for (uint32_t sy = 0; sy < samples; ++sy) {
			const float dx = ((float)sx + this->rng.get()) * denom - 0.5f;
			const float dy = ((float)sy + this->rng.get()) * denom - 0.5f;

			Ray ray = camera->castRay(camx + dx, -(camy + dy));
			ray.px = ix;
			ray.py = iy;
			ray.renderer = this;
			const Vector3f color = this->integrator->Li(this->world, ray, this->rng, 0);
			sum += color;
			sumSqr += (color * color);
		}
	}
	totalSamples += (samples * samples);
}

void Renderer::ThreadedRenderer::run(unsigned threadIdx, unsigned numThreads) {
	uint32_t cx, cy;

	while (this->getNextBucket(cx, cy)) {
		const uint32_t curWidth = std::min(this->renderer.world.getSettings().width - cx, bucketWidth);
		const uint32_t curHeight = std::min(this->renderer.world.getSettings().height - cy, bucketHeight);
		this->renderer.renderBucket(cx, cy, curWidth, curHeight);
		this->renderer.updateRenderSurface();
	}
}

bool Renderer::ThreadedRenderer::getNextBucket(uint32_t& cx, uint32_t& cy) {
	std::lock_guard<std::mutex> lock(this->mut);
	if (this->curx >= this->renderer.world.getSettings().width) return false;
	if (this->cury < this->renderer.world.getSettings().height) {
		cy = this->cury;
		cx = this->curx;
		this->cury += bucketHeight;
		return true;
	}
	// go to the start of the next row of buckets
	cy = this->cury = 0;
	cx = this->curx = (this->curx + bucketWidth);
	return this->curx < this->renderer.world.getSettings().width;
}
