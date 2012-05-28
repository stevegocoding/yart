#include "intersection.h"
#include "scene_primitive.h"

c_bsdf *c_intersection::get_bsdf(const c_ray& ray) const 
{
	assert(scene_primitive); 
	c_bsdf *bsdf = scene_primitive->get_bsdf(geom_dg, obj_to_world); 
	return bsdf; 
}