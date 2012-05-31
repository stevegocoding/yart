#pragma once 

#include "prerequisites.h"

class c_renderer_report
{
public:
	c_renderer_report(int total_work) 
		: m_total_work(total_work)
		, m_work_done(0)
	{
	}

	void update(); 
	
	void open_console_wnd(); 
	
	float percentage() const { return (float)(m_work_done/m_total_work); }

private:
	
	int m_total_work; 
	int m_work_done; 
};

typedef boost::shared_ptr<c_renderer_report> renderer_report_ptr; 