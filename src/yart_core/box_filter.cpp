#include "box_filter.h"


filter_ptr make_box_filter(float xw, float yw)
{
	return filter_ptr(new c_box_filter(xw, yw)); 
}

