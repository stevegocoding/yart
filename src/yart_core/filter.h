#pragma once

#include "prerequisites.h"

class c_filter
{

public:
	c_filter(float xw, float yw); 

	virtual ~c_filter(); 

	virtual float evaluate(float x, float y) const = 0; 

	const float x_width, y_width; 
	const float inv_x_width, inv_y_width; 
};