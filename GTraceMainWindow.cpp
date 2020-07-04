#include "GTraceMainWindow.h"

#include "Logger.h"

GTraceMainWindow::GTraceMainWindow()
	: wxFrame(nullptr, wxID_ANY, "GTrace", wxPoint(10, 10), wxSize(800, 600))
{
	LOGINFO("GTrace Main Window created.")
}
