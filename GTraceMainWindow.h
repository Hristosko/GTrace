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
	void OnElementRendered(wxCommandEvent& event);

	void rebuildBufferAndRefresh();

private:
	wxWindow* renderSurface;
	wxBitmap bitmap;
	RendererOutput output;
	RendererDisplay display;
};

