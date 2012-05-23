#include "scene.h"
#include "scene_obj.h"
#include "shape.h"

c_scene::c_scene()
{

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
		
		isect->scene_obj = (*obj_it); 
		
		
	}

	
	
	return is_hit; 
}