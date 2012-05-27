#include "scene.h"
#include "scene_obj.h"
#include "shape.h"

c_bsdf *c_intersection::get_bsdf(const c_ray& ray, pool_ptr bsdf_pool) const 
{
	 assert(scene_obj); 
	 c_bsdf *bsdf = scene_obj->get_bsdf(geom_dg, obj_to_world, bsdf_pool); 
	 return bsdf; 
}

//////////////////////////////////////////////////////////////////////////

c_scene::c_scene()
{

}

bool c_scene::query_intersection(const c_ray& ray) const 
{
	return query_intersection(ray, NULL); 
}

bool c_scene::query_intersection(const c_ray& ray, PARAM_OUT c_intersection *isect) const 
{
	// Brute force search 
	bool is_hit = false; 
	
	scene_objects_list::iterator obj_it = m_scene_objects.begin(); 
	for (; obj_it != m_scene_objects.end(); ++obj_it)
	{
		float t_hit = 0.0f; 
		float ray_eps = 0.0f; 
		shape_ptr shape = (*obj_it)->get_geometry_shape();
		is_hit = shape->intersects(ray, &t_hit, &ray_eps, diff_geom_ptr()); 
		
		if (!isect)
			return is_hit; 
		
		isect->scene_obj = (*obj_it); 
	}

	return is_hit; 
}

