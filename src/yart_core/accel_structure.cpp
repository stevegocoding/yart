#include "accel_structure.h"


c_naive_accel_structure::c_naive_accel_structure(const std::vector<scene_primitive_ptr>& prims)
{
	m_scene_primitives = prims; 
}

bool c_naive_accel_structure::intersects(const c_ray& ray, c_intersection *isect) const 
{
	for (uint32_t i = 0; i < m_scene_primitives.size(); ++i)
	{
		bool is_hit = m_scene_primitives[i]->intersects(ray, isect); 
		if (is_hit)
			return true;
	}

	return false;
}

c_aabb c_naive_accel_structure::get_world_aabb() const 
{
	return c_aabb(); 
}

accel_structure_ptr make_naive_accel_strcture(std::vector<scene_primitive_ptr>& prims)
{
	accel_structure_ptr accel = accel_structure_ptr(new c_naive_accel_structure(prims));
	return accel; 
}