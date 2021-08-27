#include "GTraceApp.h"

#include "wx/rawbmp.h"
#include <thread>
#include <memory>

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
	: outputReady(false),
	wxFrame(nullptr, wxID_ANY, "GTrace", wxPoint(10, 10), wxSize(DEFAULT_WIDTH, DEFAULT_HEIGHT)),
	renderSurface(nullptr),
	output(world),
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

GTraceMainWindow::~GTraceMainWindow() {}

void GTraceMainWindow::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc(this->renderSurface);
	if (this->bitmap.IsOk()) {
		dc.DrawBitmap(this->bitmap, wxPoint(0, 0));
	}
}

void GTraceMainWindow::OnElementRendered(wxCommandEvent& event) {
	this->display.updateDisplay(this->world);
	this->rebuildBufferAndRefresh();
}

static void renderNewScene(wxWindow* renderSurface, gtrace::RendererOutput* output, gtrace::World* world, bool* setWhenReady) {
	output->init();
	world->buildBVH();
	auto frameUpdater = [renderSurface]() {
		wxCommandEvent* event = new wxCommandEvent(GTRACE_RENDERED_ELEMENT);
		wxQueueEvent(renderSurface, event);
	};
	gtrace::Renderer renderer(frameUpdater, *output, *world);
	renderer.render();
	*setWhenReady = true;
	gtrace::MemoryBench::logData();
}

void GTraceMainWindow::NewFile(wxCommandEvent& event) {
	this->outputReady = false;
	std::unique_ptr<wxFileDialog> openDialog( new wxFileDialog(
		this,	_("Choose a scene to load"),
		wxEmptyString, wxEmptyString, wxEmptyString,
		wxFD_OPEN, wxDefaultPosition
	));

	gtrace::MemoryBench::reset();
	if (openDialog->ShowModal() == wxID_OK) {
		wxString path = openDialog->GetPath();
		this->world.clear();
		gtrace::SceneParser parser(this->world);
		parser.parseFile(path.c_str());

		this->display.setDisplayType(this->world, gtrace::RendererOutputType::Image);

		std::thread th(renderNewScene, this->renderSurface, &this->output,&this->world, &this->outputReady);
		th.detach();
	}
}

void GTraceMainWindow::SaveFile(wxCommandEvent& event) {
	if (this->outputReady == false) return;

	std::unique_ptr<wxFileDialog> saveDialog (new wxFileDialog(
		this, _("Save file as"),
		wxEmptyString, wxEmptyString, wxEmptyString,
		wxFD_SAVE, wxDefaultPosition // add overwrite prompt?
	));
	if (saveDialog->ShowModal() == wxID_OK) {
		wxString path = saveDialog->GetPath();
		this->output.save(path.c_str());
	}
}

void GTraceMainWindow::OpenFile(wxCommandEvent& event) {
	std::unique_ptr<wxFileDialog> openDialog (new wxFileDialog(
		this, _("Open render output"),
		wxEmptyString, wxEmptyString, wxEmptyString,
		wxFD_OPEN, wxDefaultPosition
	));

	if (openDialog->ShowModal() == wxID_OK) {
		wxString path = openDialog->GetPath();
		this->outputReady = false;
		this->output.open(path.c_str());
		this->display.updateDisplay(this->world);
		this->outputReady = true;
		this->rebuildBufferAndRefresh();
	}
}

void GTraceMainWindow::Image(wxCommandEvent& event) {
	if (this->outputReady == false) return;
	this->display.setDisplayType(this->world, gtrace::RendererOutputType::Image);
	this->rebuildBufferAndRefresh();
}

void GTraceMainWindow::StandardDeviation(wxCommandEvent& event) {
	if (this->outputReady == false) return;
	this->display.setDisplayType(this->world, gtrace::RendererOutputType::Variance);
	this->rebuildBufferAndRefresh();
}

void GTraceMainWindow::rebuildBufferAndRefresh() {
	char* pixelData = this->display.getPixels();
	if (pixelData == nullptr) return;

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
