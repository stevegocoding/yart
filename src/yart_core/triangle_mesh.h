#pragma once 

#include "boost/shared_array.hpp"
#include "prerequisites.h"
#include "transform.h"
#include "geometry.h"
#include "shape.h" 

typedef cml::vector<unsigned int, fixed<3> > triangle_face;

class c_triangle_mesh_impl
{
public:
    virtual bool has_normal() const = 0;
    virtual uint32_t get_num_verts() const = 0;
    virtual uint32_t get_num_faces() const = 0; 
    virtual triangle_face get_face(uint32_t idx) const = 0; 
    virtual point3f get_vert(uint32_t idx) const = 0;
	virtual void apply_transform(const c_transform& t) = 0;
    
};
typedef boost::shared_ptr<c_triangle_mesh_impl> triangle_mesh_impl_ptr; 

class c_triangle_face_impl
{
    virtual bool intersects(const c_ray& ray, 
        PARAM_OUT float *t_hit, PARAM_OUT float *ray_epsilon, PARAM_OUT diff_geom_ptr& diff_geom) const = 0; 
    
}; 
typedef boost::shared_ptr<c_triangle_face_impl> triangle_face_impl_ptr; 

class c_triangle_mesh : public c_shape
{
	typedef c_shape super; 
	typedef std::vector<triangle_face_ptr> triangles_array; 
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

    point3f get_vert(uint32_t idx) const 
    {
        return m_impl->get_vert(idx);
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
    
private:
	void alloc_triangles_array(); 

    triangle_mesh_impl_ptr m_impl; 
    triangle_face_impl_ptr m_face_impl;
	triangles_array m_triangles; 
	
};

typedef boost::shared_ptr<c_triangle_mesh> triangle_mesh_ptr; 

class c_triangle_face : public c_shape
{
public: 
    c_triangle_face(c_triangle_mesh *mesh, uint32_t face_idx)
        : m_mesh(mesh)
        , m_face_idx(face_idx)
    {}
    
    virtual bool intersects(const c_ray& ray, 
		PARAM_OUT float *t_hit, PARAM_OUT float *ray_epsilon, PARAM_OUT diff_geom_ptr& diff_geom) const; 
    
    void get_uv(float uv[3][2]) const 
    {
        
    }

	uint32_t get_face_idx() const { return m_face_idx; } 
    
private:
    c_triangle_mesh *m_mesh;

    uint32_t m_face_idx;
}; 