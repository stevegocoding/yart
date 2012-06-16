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

c_aabb union_aabb(const c_aabb& b1, const c_aabb& b2)
{
	c_aabb ret;
	ret.pt_min[x] = min(b1.pt_min[x], b2.pt_min[x]);
	ret.pt_min[y] = min(b1.pt_min[y], b2.pt_min[y]);
	ret.pt_min[z] = min(b1.pt_min[z], b2.pt_min[z]); 

	ret.pt_max[x] = max(b1.pt_max[x], b2.pt_max[x]);
	ret.pt_max[y] = max(b1.pt_max[y], b2.pt_max[y]);
	ret.pt_max[z] = max(b1.pt_max[z], b2.pt_max[z]); 
	return ret;
}