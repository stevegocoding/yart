#pragma once 

#include <math.h>
#include "prerequisites.h"
#include "transform.h"

class c_differential_geometry 
{
public:
    c_differential_geometry()
        : u(0), v(0), dudx(0), dudy(0), dvdx(0), dvdy(0) 
    {} 
    
    point3f p;
    vector3f nn; 
    float u, v; 
    shape_ptr shape; 
    vector3f dpdu, dpdv; 
    vector3f dndu, dndv; 
    vector3f dpdx, dpdy; 
    float dudx, dudy, dvdx, dvdy;    
};