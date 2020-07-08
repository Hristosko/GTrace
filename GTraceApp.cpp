#include "GTraceApp.h"

#include "wx/rawbmp.h"
#include <thread>

#include "Logger.h"
#include "Events.h"
#include "renderer/Renderer.h"
#include "scene/Parser.h"
#include "scene/World.h"

IMPLEMENT_APP(GTraceApp);

bool GTraceApp::OnInit() {
	this->frame = new GTraceMainWindow();
	this->frame->Show();
	return true;
}


#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

GTraceMainWindow::GTraceMainWindow()
	: wxFrame(nullptr, wxID_ANY, "GTrace", wxPoint(10, 10), wxSize(DEFAULT_WIDTH, DEFAULT_HEIGHT)),
	renderSurface(nullptr),
	display(output)
{
	this->renderSurface = new wxWindow(this, wxID_ANY, wxDefaultPosition, wxSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));
	this->renderSurface->SetBackgroundStyle(wxBG_STYLE_PAINT);

	this->renderSurface->Bind(wxEVT_PAINT, &GTraceMainWindow::OnPaint, this);
	this->renderSurface->Bind(GTRACE_RENDERED_ELEMENT, &GTraceMainWindow::OnElementRendered, this);

	CreateStatusBar(1);
	this->mainMenu = new wxMenuBar();

	wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(MENU_New, _("&New"), _("Open a new scene"));
	fileMenu->Append(MENU_Open, _("&Open"), _("Open a render output"));
	fileMenu->Append(MENU_Save, _("&Save"), _("Save render output"));
	this->mainMenu->Append(fileMenu, _("&File"));

	wxMenu* viewMenu = new wxMenu();
	viewMenu->Append(MENU_Image, _("&Image"), _("Load the image output"));
	viewMenu->Append(MENU_StDev, _("&StDev"), _("Load standard deviation"));
	this->mainMenu->Append(viewMenu, _("&View"));

	SetMenuBar(this->mainMenu);
	LOGINFO("GTrace Main Window created.");
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

static void renderNewScene(wxWindow* renderSurface, RendererOutput* output) {
		output->init();
		Renderer renderer(renderSurface, *output);
		renderer.render();
}

void GTraceMainWindow::NewFile(wxCommandEvent& event) {
	wxFileDialog* openDialog = new wxFileDialog(
		this,	_("Choose a scene to load"),
		wxEmptyString, wxEmptyString, wxEmptyString,
		wxFD_OPEN, wxDefaultPosition
	);

	if (openDialog->ShowModal() == wxID_OK) {
		wxString path = openDialog->GetPath();
		getWorld().clear();
		SceneParser parser = getParser();
		parser.parseFile(path.c_str());

		std::thread th(renderNewScene, this->renderSurface, &this->output);
		th.detach();
	}
}

void GTraceMainWindow::SaveFile(wxCommandEvent& event) {
	wxFileDialog* saveDialog = new wxFileDialog(
		this, _("Save file as"),
		wxEmptyString, wxEmptyString, wxEmptyString,
		wxFD_SAVE, wxDefaultPosition // add overwrite prompt?
	);
	if (saveDialog->ShowModal() == wxID_OK) {
		wxString path = saveDialog->GetPath();
		this->output.save(path.c_str());
	}
}

void GTraceMainWindow::OpenFile(wxCommandEvent& event) {
	wxFileDialog* openDialog = new wxFileDialog(
		this, _("Open render output"),
		wxEmptyString, wxEmptyString, wxEmptyString,
		wxFD_OPEN, wxDefaultPosition
	);

	if (openDialog->ShowModal() == wxID_OK) {
		wxString path = openDialog->GetPath();
		this->output.open(path.c_str());
		this->rebuildBufferAndRefresh();
	}
}

void GTraceMainWindow::Image(wxCommandEvent& event) {
	this->display.setDisplayType(RendererOutputType::Image);
	this->rebuildBufferAndRefresh();
}

void GTraceMainWindow::StandardDeviation(wxCommandEvent& event) {
	this->display.setDisplayType(RendererOutputType::Variance);
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
