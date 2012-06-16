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

	c_differential_geometry(const point3f& _p, 
		const vector3f& _dpdu, const vector3f& _dpdv, 
		const vector3f& _dndu, const vector3f& _dndv, 
		float _u,
		float _v,
		const c_shape *_shape); 

	// void calc_differentials();
    
    point3f p;							// point on the surface 
    vector3f nn;						// normalized normal 
    float u, v;							// parametric description
    const c_shape *shape;				// the shape associated 
    vector3f dpdu, dpdv;				// partial derivatives of the position (u, v)
	vector3f dpdx, dpdy;				// partial derivatives of the position (x, y)
    vector3f dndu, dndv;				// partial derivatives of the normal 
    float dudx, dudy, dvdx, dvdy;		// partial derivatives of (u, v) 
};


// ---------------------------------------------------------------------
/*
	Axis-aligned Bounding Box
*/ 
// ---------------------------------------------------------------------
class c_aabb
{
public:
	c_aabb()
		: pt_min(point3f(-INFINITY, -INFINITY, -INFINITY))
		, pt_max(point3f(INFINITY, INFINITY, INFINITY))
	{}

	c_aabb(const point3f& pmin, const point3f& pmax) 
	{
		pt_min = point3f(min(pmin[x], pmax[x]), min(pmin[y], pmax[y]), min(pmin[z], pmax[z]));
		pt_max = point3f(max(pmin[x], pmax[x]), max(pmin[y], pmax[y]), max(pmin[z], pmax[z]));
	}

	bool contains_point(const point3f& pt) const
	{        
		return (pt[x] >= pt_min[x] && pt[x] <= pt_max[x] &&
				pt[y] >= pt_min[y] && pt[y] <= pt_max[y] &&
				pt[z] >= pt_min[z] && pt[z] <= pt_max[z]);
	}
	
	float surface_area() const 
	{
		vector3f d = pt_max - pt_min; 
		return 2.0f * (d[x] * d[y] + d[x] * d[z] + d[y] * d[z]);
	}

	int max_extend() const 
	{
		vector3f d = pt_max - pt_min; 
		if (d[x] > d[y] && d[x] > d[z])
			return 0; 
		else if (d[y] > d[z])
			return 1; 
		else 
			return 2;
	}

	point3f pt_min, pt_max; 
};

c_aabb union_aabb(const c_aabb& b1, const c_aabb& b2); 