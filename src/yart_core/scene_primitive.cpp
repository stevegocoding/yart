#include "boost/make_shared.hpp"

#include "scene_primitive.h"
#include "triangle_mesh.h"
#include "material.h"
#include "intersection.h"

c_geometric_primtive::c_geometric_primtive(const c_transform& o2w, shape_ptr shape, material_ptr mat, uint32_t prim_id)
	: super(prim_id)
	, m_object_to_world(o2w)
	, m_shape(shape)
	, m_material(mat) 
{
}

c_aabb c_geometric_primtive::get_world_aabb() const 
{
	return m_shape->get_world_aabb(m_object_to_world); 
}

bool c_geometric_primtive::intersects(const c_ray& ray, PARAM_OUT c_intersection *isect) const
{
	float t_hit = 0.0f; 
	float ray_eps = 0.0f;

	bool is_hit = m_shape->intersects(ray, &t_hit, &ray_eps, &(isect->geom_dg)); 
	if (!is_hit)
		return false; 
	
	isect->scene_primitive = this; 
	isect->obj_to_world = m_object_to_world; 
	isect->ray_eps = ray_eps; 

	return true; 
}

c_bsdf *c_geometric_primtive::get_bsdf(const c_differential_geometry& geom_dg, const c_transform& o2w) const 
{
	c_differential_geometry shading_dg; 
	m_shape->get_shading_geometry(o2w, geom_dg, &shading_dg); 
	return m_material->get_bsdf(geom_dg, shading_dg); 
}


uint32_t make_triangle_mesh_primitives(triangle_mesh_ptr mesh, const c_transform& o2w, material_ptr mat, PARAM_OUT std::vector<scene_primitive_ptr>& primitives)
{
	mesh->apply_transform(o2w); 

	uint32_t num_tris = mesh->get_num_faces(); 
	for (uint32_t i = 0; i < num_tris; ++i)
	{
		triangle_face_ptr face = mesh->get_triangle_face(i);
		scene_primitive_ptr prim = scene_primitive_ptr(new c_geometric_primtive(o2w, face, mat, 1));
		primitives.push_back(prim); 
	}
	
	return num_tris; 
}

//////////////////////////////////////////////////////////////////////////

/*

c_scene_object::c_scene_object(const c_transform& w2o, const c_transform& o2w, shape_ptr geometry, material_ptr material)
	: m_world_to_obj(w2o)
	, m_obj_to_world(o2w) 
	, m_shape(geometry)
	, m_material(material)
{
	if (geometry)
	{
		geometry->apply_transform(o2w);
	}
}

c_scene_object::c_scene_object(const c_transform& w2o, const c_transform& o2w, triangle_mesh_ptr mesh, material_ptr material)
{
	
}

c_bsdf *c_scene_object::get_bsdf(const diff_geom_ptr dg, const c_transform& o2w) const
{
	c_differential_geometry shading_dg;
	m_shape->get_shading_geometry(o2w, *dg, &shading_dg); 
	return m_material->get_bsdf(dg, shading_dg);
}

scene_object_ptr make_simple_scene_obj(const c_transform& w2o, const c_transform& o2w, shape_ptr geo, material_ptr material)
{
	scene_object_ptr obj = boost::make_shared<c_scene_object>(w2o, o2w, geo, material); 
	return obj; 
}

scene_object_ptr make_mesh_scene_obj(const c_transform& w2o, const c_transform& o2w, triangle_mesh_ptr mesh, material_ptr material)
{
	scene_object_ptr obj = boost::make_shared<c_scene_object>(w2o, o2w, mesh, material); 
	return obj;
}

*/ 