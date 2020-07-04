#pragma once

#include "wx/wx.h"

#include "GTraceMainWindow.h"

class GTraceApp : public wxApp {
public:
	virtual ~GTraceApp() {}

	bool OnInit() override;

private:
	GTraceMainWindow* frame = nullptr;
};

