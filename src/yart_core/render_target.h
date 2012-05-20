#pragma once 

#include "boost/array.hpp"
#include "prerequisites.h"

struct c_pixel
{
	c_pixel()
	{
		::memset(l_rgb, 0, 3*sizeof(float));
		::memset(splat_rgb, 0, 3*sizeof(float));
		weighted_sum = 0.0f; 
		unused = 0.0f; 
	}
	
	float l_rgb[3];				// the RGB representation of radiance
	float weighted_sum; 
	float splat_rgb[3]; 
	float unused;
};

class c_render_target
{
public:
	c_render_target(int res_x, int res_y)
		: m_resolution_x(res_x)
		, m_resolution_y(res_y) 
	{}
	
	virtual void add_sample(const c_camera_sample& sample, const c_spectrum& radiance) = 0; 

protected:
	int m_resolution_x; 
	int m_resolution_y; 
};

class c_bitmap_render_target : public c_render_target
{
	typedef c_render_target super; 

public:
	class c_bitmap_impl
	{
		virtual void plot_pixel(int x, int y, int rgb) {}		
	};
	typedef boost::shared_ptr<c_bitmap_impl> bitmap_impl_ptr; 
	
public: 
	c_bitmap_render_target(int res_x, int res_y, filter_ptr filter, const float window[4], bitmap_impl_ptr bitmap_impl); 
	virtual void add_sample(const c_camera_sample& sample, const c_spectrum& radiance);
	
	
protected: 
	filter_ptr m_filter; 
	int m_x_pixel_start, m_y_pixel_start, m_x_pixel_count, m_y_pixel_count; 

	filter_table_ptr m_filter_table; 
	
	float m_window[4]; 
	bitmap_impl_ptr m_bitmap_impl; 

	typedef boost::shared_array<c_pixel> pixels_buf_ptr; 
	pixels_buf_ptr m_pixels_buf; 
	
};