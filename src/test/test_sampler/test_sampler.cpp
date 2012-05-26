// For compilers that support precompilation, includes "wx/wx.h".
#include "boost/make_shared.hpp"

#include "wx/wxprec.h"
#include "wx/sizer.h"

#include "rng.h"
#include "integrator.h"
#include "stratified_sampler.h"
#include "monte_carlo.h"
#include "dummy_integrator.h"
#include "scene.h"
#include "point_light.h"
#include "reflection.h"
 
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
	std::vector<c_light_sample> m_light_samples_vec;
	std::vector<c_bsdf_sample> m_bsdf_samples_vec;

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
	uint32_t res_x = 1; 
	uint32_t res_y = 1; 
	uint32_t sppx = 1; 
	uint32_t sppy = 1; 
	m_sampler = boost::make_shared<c_stratified_sampler>(0, res_x, 0, res_y, sppx, sppy, true, 0.0f, 0.0f);
	m_dummy_integrator = surface_integrator_ptr(new c_dummy_integrator());
	// original sample 
	
	scene_ptr scene = scene_ptr(new c_scene()); 
	
	c_transform l2w = make_translate(vector3f(9.9f, 9.9f, 9.9f));
	uint32_t num_light_samples = 16; 
	light_ptr light = light_ptr(new c_point_light(l2w, c_spectrum(0.9f, 0.9f, 0.9f), num_light_samples)); 
	scene->add_light(light); 
	sample_ptr orig_sample = sample_ptr(new c_sample(m_sampler, m_dummy_integrator, volume_integrator_ptr(), scene));

	int sc = 0; 
	c_rng rng(2047); 
	samples_array_ptr samples_array = orig_sample->duplicate(sppx*sppy); 
    
	// Generate camera samples
	while ((sc = m_sampler->get_current_pixel_samples(samples_array, rng)) > 0)
	{
		for (uint32_t j = 0; j < sppx * sppy; ++j)
		{
			m_samples_vec.push_back(samples_array[j]);

			// Get BSDF samples
			c_bsdf_sample_record *bsdf_sample_rec = &(dynamic_pointer_cast<c_dummy_integrator>(m_dummy_integrator)->m_bsdf_samples_records[0]);
			uint32_t num_bsdf_samples = bsdf_sample_rec->num_samples; 
			for (uint32_t bs = 0; bs < num_bsdf_samples; ++bs)
			{
				c_bsdf_sample bsdf_sample(&samples_array[j], *bsdf_sample_rec, bs);
				m_bsdf_samples_vec.push_back(bsdf_sample);
			}
		}
	} 

    int a = 0; 

	
    // Unit Disk Sampling
	/*
    int count = 0; 
    while ((sc = m_sampler->get_current_pixel_samples(samples_array, rng)) > 0)
	{
        count++; 
        // map the samples to unit disk
        for (uint32_t j = 0; j < sppx * sppy; ++j)
        {
            float disk_sx = 0;
            float disk_sy = 0; 
            float sx = samples_array[j].image_x / res_x; 
            float sy = samples_array[j].image_y / res_y; 
            concentric_sample_disk(sx, sy, &disk_sx, &disk_sy); 

            samples_array[j].image_x = disk_sx; 
            samples_array[j].image_y = disk_sy; 
           
			m_samples_vec.push_back(samples_array[j]);
        }
	}*/
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