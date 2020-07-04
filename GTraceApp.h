#pragma once

#include "wx/wx.h"

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

	void NewFile(wxCommandEvent& event);
	void rebuildBufferAndRefresh();

	DECLARE_EVENT_TABLE()
private:
	wxWindow* renderSurface;
	wxMenuBar* mainMenu;
	wxBitmap bitmap;
	RendererOutput output;
	RendererDisplay display;
};

enum {
	TEXT_Main = wxID_HIGHEST + 1,
	MENU_New
};

BEGIN_EVENT_TABLE(GTraceMainWindow, wxFrame)
	EVT_MENU(MENU_New, GTraceMainWindow::NewFile)
END_EVENT_TABLE()

class GTraceApp : public wxApp {
public:
	virtual ~GTraceApp() {}

	bool OnInit() override;

private:
	GTraceMainWindow* frame = nullptr;
};

