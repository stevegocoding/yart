// For compilers that support precompilation, includes "wx/wx.h".
#include "boost/make_shared.hpp"

#include "wx/wxprec.h"
#include "wx/sizer.h"

#include "../../yart_core/rng.h"
#include "../../yart_core/integrator.h"
#include "../../yart_core/stratified_sampler.h"




class c_draw_panel : public wxPanel
{
public:
	c_draw_panel(wxFrame *parent); 
	
	void paint_event(wxPaintEvent& evt); 
	void paint_now();

	void render(wxDC& dc); 

	stratified_sampler_ptr m_sampler;
	surface_integrator_ptr m_dummy_integrator; 
	samples_array_ptr m_samples_array; 
	c_rng m_rng;
	unsigned int m_num_pixels;
	unsigned int m_spp;

	std::vector<c_sample> m_samples_vec; 

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
	m_num_pixels = 16; 
	m_spp = 16; 
	m_sampler = boost::make_shared<c_stratified_sampler>(0, m_num_pixels, 0, m_num_pixels, m_spp, m_spp, true, 0.0f, 0.0f);
	m_dummy_integrator = boost::make_shared<c_surface_integrator>();
	// original sample 
	sample_ptr orig_sample = sample_ptr(new c_sample(m_sampler, m_dummy_integrator, volume_integrator_ptr(), scene_ptr()));

	int sc = 0; 
	for (unsigned int i = 0; i < m_num_pixels; ++i)
	{
		samples_array_ptr samples_array = orig_sample->duplicate(m_spp*m_spp); 
		while ((sc = m_sampler->get_current_pixel_samples(samples_array, m_rng)) > 0)
		{
			for (uint32_t j = 0; j < m_spp * m_spp; ++j)
				m_samples_vec.push_back(samples_array[j]);
		}
	}
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
	dc.DrawText(wxT("Samples"), 40, 40); 
	
	dc.SetPen(wxPen(wxColour(0,0,0),1)); 

	for (unsigned int i = 0; i < m_samples_vec.size(); ++i)
	{
		wxCoord x = m_samples_vec[i].image_x * 30 + 100;
		wxCoord y = m_samples_vec[i].image_y * 30 + 100;
		dc.DrawPoint(x, y);
	}
} 

class c_test_sampler_app : public wxApp
{
public: 
	bool OnInit(); 
	wxFrame *m_frame; 
	c_draw_panel *m_draw_panel; 
};

IMPLEMENT_APP(c_test_sampler_app)

	bool c_test_sampler_app::OnInit()
{
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL); 
	m_frame = new wxFrame((wxFrame*)NULL, -1, wxT("Test Stratified Sampler"), wxPoint(50, 50), wxSize(800, 600));
	m_draw_panel = new c_draw_panel((wxFrame*)m_frame); 
	sizer->Add(m_draw_panel, 1, wxEXPAND);
	m_frame->SetSizer(sizer);
	m_frame->SetAutoLayout(true); 
	m_frame->Show();

	return true;
}