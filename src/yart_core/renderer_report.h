#pragma once 

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <Windows.h>

#include "prerequisites.h"

struct c_render_sample_stats
{
	c_render_sample_stats(float time)
		: elapsed_time(time)
	{
	}
	
	float elapsed_time; 
};

class c_renderer_report
{
public:
	c_renderer_report(int total_work) 
		: m_total_work(total_work)
		, m_work_done(0)
	{
	}

	void update(); 
	void add_sample_stats(c_render_sample_stats& stats);
	
	void open_console_wnd(); 
	void clear_console_wnd();
	
	float percentage() const { return (float)(m_work_done/m_total_work); }

protected: 
	
	int m_total_work; 
	int m_work_done; 

	std::vector<c_render_sample_stats> m_sample_stats;
};

typedef boost::shared_ptr<c_renderer_report> renderer_report_ptr; 