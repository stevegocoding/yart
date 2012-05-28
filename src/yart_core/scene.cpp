#include "scene.h"
#include "scene_primitive.h"
#include "shape.h"
#include "accel_structure.h"

//////////////////////////////////////////////////////////////////////////

bool c_scene::query_intersection(const c_ray& ray) const 
{
	return query_intersection(ray, NULL); 
}

bool c_scene::query_intersection(const c_ray& ray, PARAM_OUT c_intersection *isect) const 
{	
	return m_accel_structure->intersects(ray, isect); 
}

