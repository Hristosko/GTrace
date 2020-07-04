#include "GTraceApp.h"

#include "wx/rawbmp.h"

#include "Logger.h"
#include "Events.h"
#include "renderer/Renderer.h"
#include "scene/Parser.h"

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
	this->mainMenu->Append(fileMenu, _("&File"));
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

void GTraceMainWindow::NewFile(wxCommandEvent& event) {
	wxFileDialog* openDialog = new wxFileDialog(
		this,	_("Choose a scene to load"),
		wxEmptyString, wxEmptyString, wxEmptyString,
		wxFD_OPEN, wxDefaultPosition
	);

	if (openDialog->ShowModal() == wxID_OK) {
		wxString path = openDialog->GetPath();
		SceneParser parser = getParser();
		parser.parseFile(path.c_str());

		this->output.init(DEFAULT_WIDTH, DEFAULT_HEIGHT);
		Renderer renderer(this->renderSurface, this->output);
		renderer.render();
	}
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
