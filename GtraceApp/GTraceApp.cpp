#include "GTraceApp.h"

#include "wx/rawbmp.h"
#include <thread>
#include <memory>

#include "common/Logger.h"
#include "common/MemoryTracker.h"
#include "renderer/Renderer.h"
#include "parser/GTRFile.h"

#include "Events.h"

IMPLEMENT_APP(GTraceApp);

bool GTraceApp::OnInit()
{
    frame = new GTraceMainWindow();
    frame->Show();
    return true;
}

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

GTraceMainWindow::GTraceMainWindow() :
    outputReady(false),
    wxFrame(nullptr, wxID_ANY, "GTrace", wxPoint(10, 10), wxSize(DEFAULT_WIDTH, DEFAULT_HEIGHT)),
    renderSurface(nullptr)
{
    this->display.setOutput(&this->output);

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
}

GTraceMainWindow::~GTraceMainWindow() {}

void GTraceMainWindow::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this->renderSurface);
    if (this->bitmap.IsOk())
    {
        dc.DrawBitmap(this->bitmap, wxPoint(0, 0));
    }
}

void GTraceMainWindow::OnElementRendered(wxCommandEvent& event)
{
    this->display.updateDisplay();
    this->rebuildBufferAndRefresh();
}

static void renderNewScene(
    wxWindow* renderSurface,
    bool* setWhenReady,
    gtrace::Scene scene,
    gtrace::RendererOutput* output)
{
    gtrace::MemoryTracker::reset();
    auto frameUpdater = [renderSurface]() {
        wxCommandEvent* event = new wxCommandEvent(GTRACE_RENDERED_ELEMENT);
        wxQueueEvent(renderSurface, event);
    };
    gtrace::Renderer renderer(std::move(scene), output, frameUpdater);
    renderer.render();
    *setWhenReady = true;
    gtrace::MemoryTracker::logStatistic();
}

void GTraceMainWindow::NewFile(wxCommandEvent& event)
{
    outputReady = false;
    std::unique_ptr<wxFileDialog> openDialog(new wxFileDialog(
        this, _("Choose a scene to load"), wxEmptyString, wxEmptyString, wxEmptyString, wxFD_OPEN, wxDefaultPosition));

    if (openDialog->ShowModal() == wxID_OK)
    {
        wxString path = openDialog->GetPath();

        gtrace::Scene scene;
        scene.sceneSettings = {DEFAULT_WIDTH, DEFAULT_HEIGHT};

        this->display.setDisplayType(gtrace::RendererOutput::Image);

        std::thread th(renderNewScene, this->renderSurface, &this->outputReady, std::move(scene), &this->output);
        th.detach();
    }
}

void GTraceMainWindow::SaveFile(wxCommandEvent& event)
{
    if (this->outputReady == false)
        return;

    std::unique_ptr<wxFileDialog> saveDialog(new wxFileDialog(
        this, _("Save file as"), wxEmptyString, wxEmptyString, wxEmptyString, wxFD_SAVE,
        wxDefaultPosition  // add overwrite prompt?
        ));
    if (saveDialog->ShowModal() == wxID_OK)
    {
        wxString path = saveDialog->GetPath();
        gtrace::GTRFile::dump(this->output, path.c_str());
    }
}

void GTraceMainWindow::OpenFile(wxCommandEvent& event)
{
    std::unique_ptr<wxFileDialog> openDialog(new wxFileDialog(
        this, _("Open render output"), wxEmptyString, wxEmptyString, wxEmptyString, wxFD_OPEN, wxDefaultPosition));

    if (openDialog->ShowModal() == wxID_OK)
    {
        wxString path = openDialog->GetPath();
        this->outputReady = false;
        this->output = gtrace::GTRFile::parse(path.c_str());
        this->display.setOutput(&this->output);
        this->display.updateDisplay();
        this->outputReady = true;
        this->rebuildBufferAndRefresh();
    }
}

void GTraceMainWindow::Image(wxCommandEvent& event)
{
    if (this->outputReady == false)
        return;
    this->display.setDisplayType(gtrace::RendererOutput::Image);
    this->rebuildBufferAndRefresh();
}

void GTraceMainWindow::StandardDeviation(wxCommandEvent& event)
{
    if (this->outputReady == false)
        return;
    this->display.setDisplayType(gtrace::RendererOutput::ImageVariane);
    this->rebuildBufferAndRefresh();
}

void GTraceMainWindow::rebuildBufferAndRefresh()
{
    char* pixelData = this->display.getPixels();
    if (pixelData == nullptr)
        return;

    wxBitmap b(DEFAULT_WIDTH, DEFAULT_HEIGHT, 24);
    wxNativePixelData data(b);

    if (!data)
        return;

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
