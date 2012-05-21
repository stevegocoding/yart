#pragma once

#include "prerequisites.h"

class c_filter
{

public:
	c_filter(float xw, float yw)
		: x_width(xw)
		, y_width(yw)
		, inv_x_width(1.0f/xw)
		, inv_y_width(1.0f/yw)
	{

	}

	virtual ~c_filter() {}

	virtual float evaluate(float x, float y) const = 0; 

	const float x_width, y_width; 
	const float inv_x_width, inv_y_width; 
};

filter_ptr make_box_filter(float xw, float yw); 
filter_ptr make_gaussian_filter(float xw, float yw, float a);