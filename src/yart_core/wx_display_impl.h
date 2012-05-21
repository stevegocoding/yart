#pragma once 

#include "prerequisites.h"
#include "render_target.h"
#include "wx/wx.h"

class c_wx_display_impl : public c_rt_display_impl
{
public:
	c_wx_display_impl(); 
	virtual void update_display_impl(int x0, int y0, int x1, int y1); 
	
private:
	
	
	wxBitmap *m_wx_bitmap; 
};