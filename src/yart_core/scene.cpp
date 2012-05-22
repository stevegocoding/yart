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
		shape_ptr shape = (*obj_it)->get_geometry_shape();
		is_hit = shape->intersects(ray, )
		
	}




	
	
	return is_hit; 
}