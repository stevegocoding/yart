// For compilers that support precompilation, includes "wx/wx.h".
#include "boost/make_shared.hpp"

#include "wx/wxprec.h"
#include "wx/sizer.h"

#include "../../yart_core/stratified_sampler.h"

class c_draw_panel : public wxPanel
{
public:
	c_draw_panel(wxFrame *parent); 
	
	void paint_event(wxPaintEvent& evt); 
	void paint_now();

	void render(wxDC& dc); 

	// some useful events
	/*
	void mouseMoved(wxMouseEvent& event);
	void mouseDown(wxMouseEvent& event);
	void mouseWheelMoved(wxMouseEvent& event);
	void mouseReleased(wxMouseEvent& event);
	void rightClick(wxMouseEvent& event);
	void mouseLeftWindow(wxMouseEvent& event);
	void keyPressed(wxKeyEvent& event);
	void keyReleased(wxKeyEvent& event);
	*/

	DECLARE_EVENT_TABLE() 	
};

class c_test_sampler_app : public wxApp
{
public: 
	bool OnInit(); 
	wxFrame *m_frame; 
	c_draw_panel *m_draw_panel; 

	stratified_sampler_ptr m_sampler;
};

IMPLEMENT_APP(c_test_sampler_app)

bool c_test_sampler_app::OnInit()
{
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL); 
	m_frame = new wxFrame((wxFrame*)NULL, -1, wxT("Hello World"), wxPoint(50, 50), wxSize(800, 600));
	m_draw_panel = new c_draw_panel((wxFrame*)m_frame); 
	sizer->Add(m_draw_panel, 1, wxEXPAND);
	m_frame->SetSizer(sizer);
	m_frame->SetAutoLayout(true); 
	m_frame->Show();

	// m_sampler = boost::make_shared<c_stratified_sampler>(0, 4, 0, 4, )
	// original sample 
	sample_ptr orig_sample = sample_ptr(new c_sample());
	
	return true;
}

BEGIN_EVENT_TABLE(c_draw_panel, wxPanel)
	// some useful events
	/*
	EVT_MOTION(BasicDrawPane::mouseMoved)
	EVT_LEFT_DOWN(BasicDrawPane::mouseDown)
	EVT_LEFT_UP(BasicDrawPane::mouseReleased)
	EVT_RIGHT_DOWN(BasicDrawPane::rightClick)
	EVT_LEAVE_WINDOW(BasicDrawPane::mouseLeftWindow)
	EVT_KEY_DOWN(BasicDrawPane::keyPressed)
	EVT_KEY_UP(BasicDrawPane::keyReleased)
	EVT_MOUSEWHEEL(BasicDrawPane::mouseWheelMoved)
	*/

	// catch paint events
	EVT_PAINT(c_draw_panel::paint_event)

END_EVENT_TABLE()

c_draw_panel::c_draw_panel(wxFrame *parent)
:wxPanel(parent)
{
	
}

void c_draw_panel::paint_event(wxPaintEvent& evt)
{
	wxPaintDC dc(this); 
	render(dc); 
}

void c_draw_panel::paint_now()
{
	wxClientDC dc(this); 
	render(dc); 
}

void c_draw_panel::render(wxDC& dc)
{
	dc.DrawText(wxT("Testing"), 40, 40); 
	
	dc.SetPen(*wxBLACK_PEN); 

	for (int x = 100; x < 500; x += 16)
		for (int y = 100; y < 500; y += 16)
			dc.DrawPoint(x, y); 	
} 