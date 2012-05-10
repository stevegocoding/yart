#pragma once

class c_sampler 
{
public:
    c_sampler(int x_start, int x_end, int y_start, int y_end);

protected: 
    int m_x_pixel_start, m_x_pixel_end; 
    int m_y_pixel_start, m_y_pixel_end;
}; 

struct c_camera_sample_set
{
	float image_x, image_y;
	float lens_u, lens_v; 
	
	c_camera_sample()
		: image_x(0), image_y(0)
		, lens_u(0), lens_v(0)
	{}
	
};


class c_sample_set : public c_camera_sample_set
{
    
};


