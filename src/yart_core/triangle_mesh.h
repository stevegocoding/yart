#pragma once 

#include "boost/shared_array.hpp"
#include "prerequisites.h"
#include "transform.h"
#include "geometry.h"
#include "shape.h" 

typedef cml::vector<unsigned int, fixed<3> > triangle_face;
typedef cml::vector2f uv;

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
    virtual triangle_face get_face(uint32_t idx) const = 0; 
    virtual point3f get_vert(uint32_t idx) const = 0;
	virtual uv get_vert_uv(uint32_t vert_idx, uint32_t uv_set = 0) const = 0; 
	virtual vector3f get_vert_normal(uint32_t vert_idx) const = 0;
	virtual vector3f get_vert_tengent(uint32_t vert_idx) const = 0;
	virtual void apply_transform(const c_transform& t) = 0;
    
};
typedef boost::shared_ptr<c_triangle_mesh_impl> triangle_mesh_impl_ptr; 

class c_triangle_face_impl
{
    virtual bool intersects(const c_ray& ray, 
        PARAM_OUT float *t_hit, PARAM_OUT float *ray_epsilon, PARAM_OUT c_differential_geometry *geom_dg) const = 0; 
    
}; 
typedef boost::shared_ptr<c_triangle_face_impl> triangle_face_impl_ptr; 

class c_triangle_mesh : public c_shape
{
	typedef c_shape super; 
	typedef std::vector<triangle_face_ptr> triangles_array;
	typedef boost::shared_array<float> uv_array_ptr; 
	
public: 
    c_triangle_mesh(triangle_mesh_impl_ptr mesh_impl)
		: super()
        , m_impl(mesh_impl)
    {
		alloc_triangles_array();
	}
    
    triangle_face get_face(uint32_t idx) const 
    {
        return m_impl->get_face(idx); 
    }

	uint32_t get_num_faces() const 
	{
		return m_impl->get_num_faces();
	}

    point3f get_vert(uint32_t idx) const 
    {
        return m_impl->get_vert(idx);
    }

	uv get_vert_uv(uint32_t vert_idx, uint32_t uv_set = 0) const 
	{
		return m_impl->get_vert_uv(vert_idx, uv_set); 
	}

	vector3f get_vert_normal(uint32_t vert_idx) const 
	{
		return m_impl->get_vert_normal(vert_idx); 
	}

	vector3f get_vert_tengent(uint32_t vert_idx) const
	{
		return m_impl->get_vert_tengent(vert_idx); 
	}
    
    triangle_face_impl_ptr get_face_impl_ptr() const { return m_face_impl; } 

	virtual void apply_transform(const c_transform& t) 
	{
		m_impl->apply_transform(t); 
	}

	triangle_face_ptr get_triangle_face(uint32_t idx) const
	{
		assert(idx < m_triangles.size());
		return m_triangles[idx]; 
	}

	bool has_uvs(uint32_t uv_set = 0) const 
	{
		return m_impl->has_uv(uv_set);
	}

	bool has_normals() const 
	{
		return m_impl->has_normal(); 
	}

	bool has_tengent() const 
	{
		return m_impl->has_tengent();
	}
    
private:
	void alloc_triangles_array(); 

    triangle_mesh_impl_ptr m_impl; 
    triangle_face_impl_ptr m_face_impl;
	triangles_array m_triangles; 

	uv_array_ptr m_uvs;
};

typedef boost::shared_ptr<c_triangle_mesh> triangle_mesh_ptr; 

class c_triangle_face : public c_shape
{
public: 
    c_triangle_face(c_triangle_mesh *mesh, uint32_t face_idx)
        : m_mesh(mesh)
        , m_face_idx(face_idx)
    {}
    
	virtual void get_shading_geometry(const c_transform& o2w, 
		const c_differential_geometry& geom_dg, 
		PARAM_OUT c_differential_geometry *shading_dg) const; 
	
    virtual bool intersects(const c_ray& ray, 
		PARAM_OUT float *t_hit, PARAM_OUT float *ray_epsilon, PARAM_OUT c_differential_geometry *geom_dg) const; 
    
    void get_uv(float uv[3][2]) const;

	uint32_t get_face_idx() const { return m_face_idx; }  
	
private:
    c_triangle_mesh *m_mesh;

    uint32_t m_face_idx;
}; 