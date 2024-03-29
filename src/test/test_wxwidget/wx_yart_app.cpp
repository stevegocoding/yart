#include "wx_yart_app.h"
#include "pch.h"
#include "color.h"
#include "camera.h"
#include "rng.h"
#include "integrator.h"
#include "stratified_sampler.h"
#include "filter.h"
#include "render_target.h"

#include "bg.xpm"

typedef boost::scoped_array<c_spectrum> spectrum_array;

void c_render_thread::set_pixel(int x, int y, int red, int green, int blue)
{
	
	
}

void *c_render_thread::Entry()
{
	//////////////////////////////////////////////////////////////////////////
	// Ray Tracing and Rendering
	//////////////////////////////////////////////////////////////////////////
	c_rng rng(2047); 
	c_rng rng2(1024); 
	
	int num_pixel_samples = 0; 
	int max_samples = m_main_sampler->get_max_num_samples(); 
	samples_array_ptr samples_array = m_origin_sample->duplicate(max_samples); 
	spectrum_array ls(new c_spectrum[max_samples]); 
	while ((num_pixel_samples = m_main_sampler->get_current_pixel_samples(samples_array, rng)) > 0)
	{
		for (int j = 0; j < num_pixel_samples; ++j)
		{
			c_ray r; 
			m_camera->generate_ray(samples_array[j], &r); 
			float red = rng2.random_float(); 
			float green = rng2.random_float(); 
			float blue = rng2.random_float();
			ls[j] = c_spectrum(red, green, blue); 
			m_camera->get_render_target()->add_sample(samples_array[j], ls[j]);
		}

		//////////////////////////////////////////////////////////////////////////
		// Update the window 
		////////////////////////////////////////////////////////////////////////// 
		m_render_window->update_display(m_render_target);
		
	} 
	

	
	return NULL; 
}

void c_render_thread::OnExit()
{
	int a = 0; 
}

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_APP(c_wx_yart_app)

bool c_wx_yart_app::OnInit()
{
	wxInitAllImageHandlers(); 

	frame = new c_wx_yart_frame(wxPoint(200, 200), wxSize(700, 500)); 
	frame->Center();
	frame->Show(true); 
	SetTopWindow(frame); 

	return true; 
}

int c_wx_yart_app::OnExit()
{
	return 0; 
}

void c_wx_yart_app::set_status_text(const wxString& text, int number /* = 0 */)
{
	frame->SetStatusText(text, number); 
}


//////////////////////////////////////////////////////////////////////////

DEFINE_EVENT_TYPE(wxEVT_RENDER)
	
BEGIN_EVENT_TABLE(c_wx_yart_frame, wxFrame)
	EVT_MENU(menu_render_start, c_wx_yart_frame::OnRenderStart)
	EVT_MENU(menu_render_pause, c_wx_yart_frame::OnRenderPause)
	EVT_MENU(menu_render_resume, c_wx_yart_frame::OnRenderResume)
	EVT_MENU(menu_file_quit, c_wx_yart_frame::OnQuit)
	EVT_COMMAND(ID_RENDER_COMPLETED, wxEVT_RENDER, c_wx_yart_frame::OnRenderCompleted)
END_EVENT_TABLE()

c_wx_yart_frame::c_wx_yart_frame(const wxPoint& pos, const wxSize& size)
:wxFrame((wxFrame*)NULL, -1, wxT("YART Test"), pos, size)
{
	//////////////////////////////////////////////////////////////////////////
	// Create the menu 
	//////////////////////////////////////////////////////////////////////////
	wxMenu *menu_file = new wxMenu(); 
	// menu_file->Append(Menu_File_Open, wxT("&Open..."   ));
	menu_file->Append(menu_file_save, wxT("&Save As..."));
	menu_file->AppendSeparator();
	menu_file->Append(menu_file_quit, wxT("E&xit"));
	menu_file->Enable(menu_file->FindItem(wxT("&Save As...")), FALSE);

	wxMenu *menu_render = new wxMenu(); 
	menu_render->Append(menu_render_start, wxT("&Start"));
	menu_render->Append(menu_render_pause, wxT("&Pause")); 
	menu_render->Append(menu_render_resume, wxT("&Resume"));
	menu_render->Enable(menu_render->FindItem(wxT("&Start" )), TRUE );
	menu_render->Enable(menu_render->FindItem(wxT("&Pause" )), FALSE);
	menu_render->Enable(menu_render->FindItem(wxT("&Resume")), FALSE);

	wxMenuBar *menu_bar = new wxMenuBar; 
	menu_bar->Append(menu_file, wxT("&File"));
	menu_bar->Append(menu_render, wxT("&Render"));
	
	SetMenuBar(menu_bar); 

	//////////////////////////////////////////////////////////////////////////
	// Create Status Bar
	//////////////////////////////////////////////////////////////////////////
	CreateStatusBar(); 
	SetStatusText(wxT("Ready"));
	wxStatusBar* statusBar = GetStatusBar();
	int widths[] = {150,300};
	statusBar->SetFieldsCount(2, widths);
		
	//////////////////////////////////////////////////////////////////////////
	// Create render window
	//////////////////////////////////////////////////////////////////////////
	render_window = new c_wx_render_window(this);
	
}

void c_wx_yart_frame::OnQuit( wxCommandEvent& event )
{
	Close(); 
}

void c_wx_yart_frame::OnRenderStart( wxCommandEvent& event )
{
	wxMenu *menu = GetMenuBar()->GetMenu(1); 
	menu->Enable(menu->FindItem(wxT("&Start")), FALSE); 
	menu->Enable(menu->FindItem(wxT("&Pause")), TRUE); 
	menu->Enable(menu->FindItem(wxT("&Resume")), FALSE); 
	
	// Start rendering
	render_window->start_render();

}

void c_wx_yart_frame::OnRenderCompleted( wxCommandEvent& event )
{
	wxMenu *menu = GetMenuBar()->GetMenu(1); 
	menu->Enable(menu->FindItem(wxT("&Start")), TRUE); 
	menu->Enable(menu->FindItem(wxT("&Pause")), FALSE); 
	menu->Enable(menu->FindItem(wxT("&Resume")), FALSE); 

	wxGetApp().set_status_text(wxT("Rendering Complete"));
}

void c_wx_yart_frame::OnRenderPause( wxCommandEvent& event )
{
	
}

void c_wx_yart_frame::OnRenderResume( wxCommandEvent& event )
{

}

////////////////////////////////////////////////////////////////////////// 

c_wx_render_window::c_wx_render_window(wxWindow *parent)
	: wxScrolledWindow(parent)
	, m_res_x(64)
	, m_res_y(64)
	, m_sppx(1)
	, m_sppy(1)
	, m_bitmap(NULL)
	, m_timer(NULL)
	, m_render_thread(NULL)
{
	SetOwnBackgroundColour(wxColour(143,144,150));

	init_renderer(); 
}

c_wx_render_window::~c_wx_render_window()
{
	
}

void c_wx_render_window::init_renderer()
{
	//////////////////////////////////////////////////////////////////////////
	// Make the sampler
	//////////////////////////////////////////////////////////////////////////
	m_main_sampler.reset(new c_stratified_sampler(0, m_res_x, 0, m_res_y, m_sppx, m_sppy, true, 0.0f, 0.0f)); 
	m_origin_sample.reset(new c_sample(m_main_sampler, surface_integrator_ptr(), volume_integrator_ptr(), scene_ptr()));

	//////////////////////////////////////////////////////////////////////////
	// Make Render Target
	//////////////////////////////////////////////////////////////////////////
	m_filter = make_box_filter(0.5f, 0.5f); 
	if (m_render_target)
		m_render_target.reset();
	m_render_target = make_bitmap_render_target(m_res_x, m_res_y, m_filter); 

	//////////////////////////////////////////////////////////////////////////
	// Make Camera
	//////////////////////////////////////////////////////////////////////////
	matrix44f m;
	cml::matrix_translation(m, vector3f(0,0,0));
	c_transform world_to_cam(m); 
	c_transform cam_to_world(m);
	float wnd[4] = {-1.0f, 1.0f, -1.0f, 1.0f};
	m_camera.reset(new c_perspective_camera(cam_to_world, wnd, 0, 0, 90, m_render_target));
	
}

void c_wx_render_window::set_image(wxImage& image)
{
	if (m_bitmap)
		delete m_bitmap;

	m_bitmap = new wxBitmap(image); 
	
	SetScrollbars(10, 10, (int)(m_bitmap->GetWidth() / 10.0f), (int)(m_bitmap->GetHeight() / 10.0f), 0, 0, true); 

	Refresh(); 
}

void c_wx_render_window::OnDraw(wxDC& dc)
{
	if (m_bitmap && m_bitmap->IsOk())
	{
		wxBufferedDC bdc(&dc, *m_bitmap);
	}
	
}

void c_wx_render_window::start_render()
{
	//////////////////////////////////////////////////////////////////////////
	// Setup the scene
	//////////////////////////////////////////////////////////////////////////
	
	wxGetApp().set_status_text(wxT("Building scene ..."));
 
	//////////////////////////////////////////////////////////////////////////
	// Start the rendering
	//////////////////////////////////////////////////////////////////////////
	
	wxGetApp().set_status_text(wxT("Rendering ..."));

	// Set the background
	wxBitmap bg_bitmap(m_render_target->res_x(), m_render_target->res_y(), -1);
	wxMemoryDC dc; 
	dc.SelectObject(bg_bitmap); 
	dc.SetBackground(*wxGREY_BRUSH); 
	dc.Clear();

	wxBitmap tile(bg_xpm);
	for (int x = 0; x < m_render_target->res_x(); x+=16)
	{
		for (int y = 0; y < m_render_target->res_y(); y+=16)
			dc.DrawBitmap(tile, x, y, FALSE); 
	}

	dc.SelectObject(wxNullBitmap); 
	
	wxImage temp = bg_bitmap.ConvertToImage(); 
	set_image(temp); 
	

	//////////////////////////////////////////////////////////////////////////
	// Start the rendering thread and timer
	//////////////////////////////////////////////////////////////////////////
	
	m_update_timer.Start(250); 
	m_timer = new wxStopWatch(); 
	
	m_render_thread = new c_render_thread(this, m_main_sampler, m_origin_sample, m_camera, m_render_target); 
	m_render_thread->Create();
	m_render_thread->SetPriority(20); 
	m_render_thread->Run(); 
}

void c_wx_render_window::update_display(render_target_ptr& render_target)
{
	pixels_buf_ptr pixels = render_target->get_pixels(); 
	
	wxClientDC cdc(this);
	DoPrepareDC(cdc); 
	wxBufferedDC bufferedDC(&cdc, *m_bitmap);
	
	// Draw all the pixels
	int res_x = render_target->res_x(); 
	int res_y = render_target->res_y(); 
	for (int y = 0; y < res_y; ++y)
	{
		for (int x = 0; x < res_x; ++x)
		{
			c_render_pixel pixel = pixels[y * res_x + x];
			float inv_w = 1.0f / pixel.weighted_sum; 
			unsigned char r = pixel.l_rgb[0] * inv_w * 255; 
			unsigned char g = pixel.l_rgb[1] * inv_w * 255; 
			unsigned char b = pixel.l_rgb[2] * inv_w * 255;

			wxPen pen(wxColour(r, g, b));
			bufferedDC.SetPen(pen); 
			bufferedDC.DrawPoint(x, y); 
		}
	}
}