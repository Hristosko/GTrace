#include "GTraceApp.h"

wxIMPLEMENT_APP(GTraceApp);

bool GTraceApp::OnInit() {
	this->frame = new GTraceMainWindow();
	this->frame->Show();
	return true;
}
