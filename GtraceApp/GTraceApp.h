#pragma once

#include "wx/wx.h"
#include "renderer/RendererOutput.h"
#include "renderer/RendererDisplay.h"
#include "scene/World.h"
#include "MemoryBench.h"

class GTraceMainWindow : public wxFrame {
public:
	GTraceMainWindow();
	virtual ~GTraceMainWindow();

private:
	void OnPaint(wxPaintEvent& event);
	void OnElementRendered(wxCommandEvent& event);

	void NewFile(wxCommandEvent& event);
	void SaveFile(wxCommandEvent& event);
	void OpenFile(wxCommandEvent& event);
	void Image(wxCommandEvent& event);
	void StandardDeviation(wxCommandEvent& event);

	void rebuildBufferAndRefresh();

	DECLARE_EVENT_TABLE()
private:
	bool outputReady;
	wxWindow* renderSurface;
	wxMenuBar* mainMenu;
	wxBitmap bitmap;
	World world;
	RendererOutput output;
	RendererDisplay display;
};

enum {
	TEXT_Main = wxID_HIGHEST + 1,
	MENU_New,
	MENU_Save,
	MENU_Open,
	MENU_Image,
	MENU_StDev,
};

BEGIN_EVENT_TABLE(GTraceMainWindow, wxFrame)
	EVT_MENU(MENU_New, GTraceMainWindow::NewFile)
	EVT_MENU(MENU_Save, GTraceMainWindow::SaveFile)
	EVT_MENU(MENU_Open, GTraceMainWindow::OpenFile)
	EVT_MENU(MENU_Image, GTraceMainWindow::Image)
	EVT_MENU(MENU_StDev, GTraceMainWindow::StandardDeviation)
END_EVENT_TABLE()

class GTraceApp : public wxApp {
public:
	virtual ~GTraceApp() {}

	bool OnInit() override;
	int OnExit() override;

private:
	GTraceMainWindow* frame = nullptr;
};
