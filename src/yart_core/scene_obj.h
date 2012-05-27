#pragma once

#include "prerequisites.h"
#include "geometry.h"

class c_scene_object
{
public:
	c_scene_object(const c_transform& w2o, const c_transform& o2w, shape_ptr geometry); 
	
	//virtual aabb_ptr get_world_aabb() const;
	virtual c_transform get_world_to_obj() const { return m_world_to_obj; }
	virtual c_transform get_obj_to_world() const { return m_obj_to_world; }

	shape_ptr get_geometry_shape() const { return m_shape; }
	
	c_bsdf *get_bsdf(const diff_geom_ptr dg, const c_transform& o2w) const;
	
protected:
	shape_ptr m_shape;
	aabb_ptr m_aabb;
	material_ptr m_material; 

	c_transform m_world_to_obj;
	c_transform m_obj_to_world;
};

class c_instanced_scene_obj : public c_scene_object
{
	typedef c_scene_object super; 

	c_instanced_scene_obj(scene_object_ptr obj_ref, const c_transform& w2o, const c_transform& o2w)
		: super(w2o, o2w, obj_ref->get_geometry_shape()) 
		, m_scene_obj_ref(obj_ref)
	{} 

private: 
	scene_object_ptr m_scene_obj_ref; 

}; 
 
scene_object_ptr make_simple_scene_obj(const c_transform& w2o, const c_transform& o2ws, shape_ptr geo);
