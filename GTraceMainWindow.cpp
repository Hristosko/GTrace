#include "GTraceMainWindow.h"

#include "wx/rawbmp.h"

#include "Logger.h"
#include "Events.h"
#include "renderer/Renderer.h"
#include "scene/Parser.h"

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

GTraceMainWindow::GTraceMainWindow()
	: wxFrame(nullptr, wxID_ANY, "GTrace", wxPoint(10, 10), wxSize(DEFAULT_WIDTH, DEFAULT_HEIGHT)),
	renderSurface(nullptr),
	output(DEFAULT_WIDTH, DEFAULT_HEIGHT),
	display(output)
{
	this->renderSurface = new wxWindow(this, wxID_ANY, wxDefaultPosition, wxSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));
	this->renderSurface->SetBackgroundStyle(wxBG_STYLE_PAINT);
	
	this->renderSurface->Bind(wxEVT_PAINT, &GTraceMainWindow::OnPaint, this);
	this->renderSurface->Bind(GTRACE_RENDERED_ELEMENT, &GTraceMainWindow::OnElementRendered, this);
	LOGINFO("GTrace Main Window created.");

	SceneParser parser = getParser();
	parser.parseFile("simple.sc");

	Renderer renderer(this->renderSurface, this->output);
	renderer.render();
}

void GTraceMainWindow::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc(this->renderSurface);
	if (this->bitmap.IsOk()) {
		dc.DrawBitmap(this->bitmap, wxPoint(0, 0));
	}
}

void GTraceMainWindow::OnElementRendered(wxCommandEvent& event) {
	this->rebuildBufferAndRefresh();
}

void GTraceMainWindow::rebuildBufferAndRefresh() {
	char* pixelData = this->display.getPixels();
	wxBitmap b(DEFAULT_WIDTH, DEFAULT_HEIGHT, 24);
	wxNativePixelData data(b);

	if (!data) {
		LOGERROR("Failed to initialize wxNativePixelData.")
			return;
	}

	wxNativePixelData::Iterator p(data);

	int curPixelDataLoc = 0;
	for (int y = 0; y < DEFAULT_HEIGHT; ++y)
	{
		wxNativePixelData::Iterator rowStart = p;
		for (int x = 0; x < DEFAULT_WIDTH; ++x, ++p)
		{
			p.Red() = pixelData[curPixelDataLoc++];
			p.Green() = pixelData[curPixelDataLoc++];
			p.Blue() = pixelData[curPixelDataLoc++];
		}
		p = rowStart;
		p.OffsetY(data, 1);
	}
	this->bitmap = b;
	this->renderSurface->Refresh();
	this->renderSurface->Update();
}
