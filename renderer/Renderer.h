#pragma once

#include "RendererOutput.h"
#include "wx/wx.h"

class Renderer {
public:
	Renderer(wxWindow* renderSurface, RendererOutput& output)
		: renderSurface(renderSurface), output(output) {}

	void render();
private:
	wxWindow* renderSurface;
	RendererOutput& output;
};

