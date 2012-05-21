#pragma once 

#include "prerequisites.h"
#include "filter.h"

class c_box_filter : public c_filter
{
public:
	c_box_filter(float xw, float yw)
		: c_filter(xw, yw)
	{}

	virtual float evaluate(float x, float y) const  
	{
		return 1.0f; 
	} 
};