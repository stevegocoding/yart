#include "wx_yart_app.h"
#include "pch.h"
#include "camera.h"
#include "rng.h"
#include "integrator.h"
#include "stratified_sampler.h"
#include "filter.h"
#include "render_target.h"

#include "bg.xpm"


void c_render_thread::set_pixel(int x, int y, int red, int green, int blue)
{
	
	
}

void *c_render_thread::Entry()
{
	//////////////////////////////////////////////////////////////////////////
	// Generate the rays
	//////////////////////////////////////////////////////////////////////////
	/*
	int sc = 0; 
	samples_array_ptr samples_array = orig_sample->duplicate(sppx*sppy); 
	while ((sc = sampler->get_current_pixel_samples(samples_array, rng)) > 0)
	{
		for (uint32_t j = 0; j < sppx * sppy; ++j)
		{
			c_ray r; 
			cam->generate_ray(samples_array[j], &r); 
		}
	}
	*/ 

	static int a = 0;
	a++; 
	
	
	return NULL; 
}

void c_render_thread::OnExit()
{
	
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
	, m_sppx(4)
	, m_sppy(4)
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
	
	m_render_thread = new c_render_thread(m_main_sampler, m_origin_sample, m_camera, m_render_target); 
	m_render_thread->Create();
	m_render_thread->SetPriority(20); 
	m_render_thread->Run(); 
}