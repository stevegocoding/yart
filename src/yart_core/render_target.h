#pragma once 

#include "boost/array.hpp"
#include "prerequisites.h"

struct c_render_pixel
{
	c_render_pixel()
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
typedef boost::shared_array<c_render_pixel> pixels_buf_ptr; 

class c_render_target
{
public:
	c_render_target(int res_x, int res_y)
		: m_resolution_x(res_x)
		, m_resolution_y(res_y) 
	{}
	
	virtual void add_sample(const c_camera_sample& sample, const c_spectrum& radiance) = 0; 

	virtual void get_sample_extent(int *x_start, int *x_end, int *y_start, int *y_end) const = 0; 
	virtual void get_pixel_extent(int *x_start, int *x_end, int *y_start, int *y_end) const = 0;
	virtual pixels_buf_ptr get_pixels() = 0;

	int res_x() const { return m_resolution_x; }
	int res_y() const { return m_resolution_y; }

protected:
	int m_resolution_x; 
	int m_resolution_y; 
};

class c_bitmap_render_target : public c_render_target
{
	typedef c_render_target super; 

public: 
	c_bitmap_render_target(int res_x, int res_y, filter_ptr filter, const float window[4]); 
	virtual void add_sample(const c_camera_sample& sample, const c_spectrum& radiance);

	virtual void get_sample_extent(int *x_start, int *x_end, int *y_start, int *y_end) const;
	virtual void get_pixel_extent(int *x_start, int *x_end, int *y_start, int *y_end) const;
	virtual pixels_buf_ptr get_pixels() { return m_pixels_buf; }
	filter_table_ptr get_filter_table() { return m_filter_table; }
	virtual void update_display() {}

protected: 
	filter_ptr m_filter;
	int m_x_pixel_start, m_y_pixel_start, m_x_pixel_count, m_y_pixel_count; 

	filter_table_ptr m_filter_table; 
	float m_window[4]; 
	pixels_buf_ptr m_pixels_buf; 
};

render_target_ptr make_bitmap_render_target(int res_x, int res_y, filter_ptr filter); 