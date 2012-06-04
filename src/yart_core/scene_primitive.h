#pragma once

#include "prerequisites.h"
#include "geometry.h"

class c_scene_primtive
{
public:
	c_scene_primtive(uint32_t prim_id = 0)
		: m_prim_id(prim_id)
	{}

	virtual c_aabb get_world_aabb() const = 0;
	virtual bool intersects(const c_ray& ray, PARAM_OUT c_intersection *isect) const = 0; 
	virtual c_bsdf* get_bsdf(const c_differential_geometry& geom_dg, const c_transform& o2w) const = 0; 

protected: 
	uint32_t m_prim_id; 
};

class c_geometric_primtive : public c_scene_primtive 
{
	typedef c_scene_primtive super; 

public: 
	c_geometric_primtive(const c_transform& o2w, const c_shape *shape, material_ptr mat, uint32_t prim_id);

	virtual c_aabb get_world_aabb() const; 
	virtual bool intersects(const c_ray& ray, PARAM_OUT c_intersection *isect) const; 
	virtual c_bsdf* get_bsdf(const c_differential_geometry& geom_dg, const c_transform& o2w) const; 

private: 
	const c_shape *m_shape; 
	material_ptr m_material; 
	c_transform m_object_to_world; 
};

uint32_t make_triangle_mesh_primitives(triangle_mesh_ptr mesh, const c_transform& o2w, material_ptr mat, PARAM_OUT std::vector<scene_primitive_ptr>& primitives);

//////////////////////////////////////////////////////////////////////////

/*
class c_scene_object
{
public:
	c_scene_object(const c_transform& w2o, const c_transform& o2w, shape_ptr geometry, material_ptr material); 
	c_scene_object(const c_transform& w2o, const c_transform& o2w, triangle_mesh_ptr mesh, material_ptr material); 
	
	//virtual aabb_ptr get_world_aabb() const;
	virtual c_transform get_world_to_obj() const { return m_world_to_obj; }
	virtual c_transform get_obj_to_world() const { return m_obj_to_world; }

	shape_ptr get_geometry_shape() const { return m_shape; }
	
	c_bsdf *get_bsdf(const diff_geom_ptr dg, const c_transform& o2w) const;
	
protected:
	// shape_ptr m_shape;
	
	
	
	aabb_ptr m_aabb;
	material_ptr m_material; 

	c_transform m_world_to_obj;
	c_transform m_obj_to_world;
};

class c_instanced_scene_obj : public c_scene_object
{
	typedef c_scene_object super; 

	c_instanced_scene_obj(scene_object_ptr obj_ref, const c_transform& w2o, const c_transform& o2w, material_ptr material)
		: super(w2o, o2w, obj_ref->get_geometry_shape(), material) 
		, m_scene_obj_ref(obj_ref)
	{} 

private: 
	scene_object_ptr m_scene_obj_ref;  
}; 

class c_geometric_scene_obj : public c_scene_object
{
public
	c_geometric_scene_obj() {}
	

	
};

scene_object_ptr make_simple_scene_obj(const c_transform& w2o, const c_transform& o2ws, shape_ptr geo, material_ptr material);
scene_object_ptr make_mesh_scene_obj(const c_transform& w2o, const c_transform& o2w, triangle_mesh_ptr mesh, material_ptr material );
*/
