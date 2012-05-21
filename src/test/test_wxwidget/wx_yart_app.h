#pragma once 

#include "wx/wx.h"
#include "wx/dcbuffer.h"

#include "prerequisites.h"

class c_render_thread; 
class c_wx_yart_frame;
class c_wx_render_window; 
class c_wx_yart_app; 

class c_render_thread : public wxThread
{
public: 
	c_render_thread(sampler_ptr main_sampler, sample_ptr origin_sample, camera_ptr cam, render_target_ptr render_target)
		: m_main_sampler(main_sampler)
		, m_origin_sample(origin_sample)
		, m_camera(cam)
		, m_render_target(render_target)
	{

	}
	
	virtual void* Entry(); 
	virtual void OnExit();
	virtual void set_pixel(int x, int y, int red, int green, int blue);

private:
	c_wx_render_window *window;

	wxStopWatch *m_timer; 
	long m_last_update_time; 

	sampler_ptr m_main_sampler;
	sample_ptr m_origin_sample;
	camera_ptr m_camera; 
	render_target_ptr m_render_target; 
};

enum e_menu_items
{
	menu_file_quit = 100, 
	menu_file_save,
	menu_render_start, 
	menu_render_pause,
	menu_render_resume
};

DECLARE_EVENT_TYPE(wxEVT_RENDER, -1)
#define ID_RENDER_COMPLETED 100
#define ID_RENDER_NEWPIXEL  101
#define ID_RENDER_UPDATE    102

class c_wx_yart_frame : public wxFrame
{
public: 
	c_wx_yart_frame(const wxPoint& pos, const wxSize& size); 

	//event handlers
	void OnQuit( wxCommandEvent& event ); 
	void OnRenderStart( wxCommandEvent& event );
	void OnRenderCompleted( wxCommandEvent& event );
	void OnRenderPause( wxCommandEvent& event );
	void OnRenderResume( wxCommandEvent& event );
	//void OnOpenFile( wxCommandEvent& event );
	//void OnSaveFile( wxCommandEvent& event );

private:
	c_wx_render_window *render_window; 

	DECLARE_EVENT_TABLE(); 
	
};

class c_wx_render_window : public wxScrolledWindow
{
public:
	c_wx_render_window(wxWindow *parent); 
	virtual ~c_wx_render_window(); 

	void init_renderer();
	
	void set_image(wxImage& image);
	void start_render();
	void pause_render(); 
	void resume_render(); 

	virtual void OnDraw(wxDC& dc);
	void OnRenderCompleted( wxCommandEvent& event );
	void OnTimerUpdate( wxTimerEvent& event );
	void OnNewPixel( wxCommandEvent& event );

protected:
	wxBitmap *m_bitmap; 
	wxStopWatch *m_timer; 
	wxTimer m_update_timer; 
	c_render_thread *m_render_thread; 
	
	// DECLARE_EVENT_TABLE()


	//////////////////////////////////////////////////////////////////////////
	// Renderer Objects 
	//////////////////////////////////////////////////////////////////////////
	sampler_ptr m_main_sampler; 
	sample_ptr m_origin_sample; 
	camera_ptr m_camera; 
	filter_ptr m_filter;
	render_target_ptr m_render_target;

	uint32_t m_res_x; 
	uint32_t m_res_y; 
	uint32_t m_sppx;
	uint32_t m_sppy; 
};

class c_wx_yart_app : public wxApp
{

public:
	virtual bool OnInit(); 
	virtual int OnExit(); 
	void set_status_text(const wxString& text, int number = 0); 

private:

	c_wx_yart_frame *frame; 
	//DECLARE_EVENT_TABLE()

};