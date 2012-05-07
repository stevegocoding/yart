#pragma once

struct c_camera_sample
{
	float image_x, image_y;
	float lens_u, lens_v; 
	
	c_camera_sample()
		: image_x(0), image_y(0)
		, lens_u(0), lens_v(0)
	{}
	
};