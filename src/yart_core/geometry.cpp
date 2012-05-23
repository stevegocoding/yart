#include "geometry.h"


c_differential_geometry::c_differential_geometry(const point3f& _p, 
	const vector3f& _dpdu, const vector3f& _dpdv, 
	const vector3f& _dndu, const vector3f& _dndv, 
	float _u, float _v, const c_shape *_shape)
	: p(_p)
	, dpdu(_dpdu), dpdv(_dpdv)
	, dndu(_dndu), dndv(_dndv)
	, u(_u), v(_v)
	, shape(_shape)
	, dudx(0), dudy(0), dvdx(0), dvdy(0)
	
{
	nn = normalize(cross(dpdu, dpdv)); 
	
	// Adjust normal based on orientation and handedness
} 