#pragma once 

#include "prerequisites.h"
#include "transform.h"
#include "geometry.h"
#include "shape.h" 

class c_triangle_face_impl
{
public: 
	virtual bool intersects_impl(const c_ray& ray, 
		PARAM_OUT float *t_hit, 
		PARAM_OUT float *ray_epsilon, 
		PARAM_OUT c_differential_geometry *geom_dg) const = 0; 

	virtual void get_shading_geometry_impl(const c_transform& o2w, 
		const c_differential_geometry& geom_dg, 
		PARAM_OUT c_differential_geometry *shading_dg) const = 0; 

}; 
typedef boost::shared_ptr<c_triangle_face_impl> triangle_face_impl_ptr; 

class c_triangle_mesh_impl
{
public:
	c_triangle_mesh_impl() {}
	virtual ~c_triangle_mesh_impl() {}

	virtual bool has_tengent() const = 0; 
    virtual bool has_normal() const = 0;
	virtual bool has_uv(uint32_t uv_set = 0) const = 0; 
    virtual uint32_t get_num_verts() const = 0;
    virtual uint32_t get_num_faces() const = 0;
	virtual void apply_transform(const c_transform& t) = 0;
	virtual const c_triangle_face_impl* get_face_impl(uint32_t face_idx) const = 0;
	
};
typedef boost::shared_ptr<c_triangle_mesh_impl> triangle_mesh_impl_ptr; 

////////////////////////////////////////////////////////////////////////// 

class c_triangle_mesh
{
	typedef std::vector<triangle_face_ptr> triangles_array;
	
public: 
    c_triangle_mesh(triangle_mesh_impl_ptr mesh_impl)
		: m_impl(mesh_impl)
    {
		allocate_triangles();
	} 

	virtual void apply_transform(const c_transform& t) 
	{
		m_impl->apply_transform(t); 
	}
 
	uint32_t get_num_faces() const 
	{ 
		return m_impl->get_num_faces(); 
	}
	
	triangle_face_ptr get_face(uint32_t face_idx) const 
	{
		assert(face_idx < get_num_faces());
		return m_triangles[face_idx]; 
	}

private: 

	void allocate_triangles(); 

    triangle_mesh_impl_ptr m_impl; 
	triangles_array m_triangles; 

};

typedef boost::shared_ptr<c_triangle_mesh> triangle_mesh_ptr; 

class c_triangle_face : public c_shape
{
public: 
    c_triangle_face(const c_triangle_face_impl *face_impl = NULL)
		: m_face_impl(face_impl)
	{}
    
	virtual void get_shading_geometry(const c_transform& o2w, 
		const c_differential_geometry& geom_dg, 
		PARAM_OUT c_differential_geometry *shading_dg) const; 
	
    virtual bool intersects(const c_ray& ray, 
		PARAM_OUT float *t_hit, PARAM_OUT float *ray_epsilon, PARAM_OUT c_differential_geometry *geom_dg) const; 
	
private:

	const c_triangle_face_impl *m_face_impl;
}; 