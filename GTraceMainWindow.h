#pragma once

#include "wx/wx.h"
#include "renderer/RendererOutput.h"
#include "renderer/RendererDisplay.h"

class GTraceMainWindow : public wxFrame {
public:
	GTraceMainWindow();
	virtual ~GTraceMainWindow() {}

private:
	void OnPaint(wxPaintEvent& event);

private:
	wxWindow* renderSurface;
	RendererOutput output;
	RendererDisplay display;
};

