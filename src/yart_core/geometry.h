#pragma once 

#include <math.h>
#include "prerequisites.h"
#include "transform.h"

// ---------------------------------------------------------------------
/*
	Encapsulate the general local information of a point on the surface of a geometry
	Assuming the surface has a parametric representation. 
*/ 
// ---------------------------------------------------------------------
class c_differential_geometry 
{
public:
    c_differential_geometry()
        : u(0), v(0), dudx(0), dudy(0), dvdx(0), dvdy(0) 
    {} 

	c_differential_geometry(const point3f& _p, const 
		vector3f& _dpdu, const vector3f& _dpdv, 
		vector3f& _dndu, const vector3f& _dndv, 
		float _u,
		float _v,
		shape_ptr& _shape); 

	// void calc_differentials();
    
    point3f p;							// point on the surface 
    vector3f nn;						// normalized normal 
    float u, v;							// parametric description
    shape_ptr shape;					// the shape associated 
    vector3f dpdu, dpdv;				// partial derivatives of the position (u, v)
	vector3f dpdx, dpdy;				// partial derivatives of the position (x, y)
    vector3f dndu, dndv;				// partial derivatives of the normal 
    float dudx, dudy, dvdx, dvdy;		// partial derivatives of (u, v) 
};