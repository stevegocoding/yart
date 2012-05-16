#pragma once 

#include "prerequisites.h"
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
public: 
    c_triangle_mesh(triangle_mesh_impl_ptr& mesh_impl)
        : m_impl(mesh_impl)
    {}
    
    triangle_face get_face(uint32_t idx) const 
    {
        return m_impl->get_face(idx); 
    }

    point3f get_vert(uint32_t idx) const 
    {
        return m_impl->get_vert(idx);
    }
    
    triangle_face_impl_ptr get_face_impl_ptr() const { return m_face_impl; } 
    
private:
    triangle_mesh_impl_ptr m_impl; 
    triangle_face_impl_ptr m_face_impl;
};

typedef boost::shared_ptr<c_triangle_mesh> triangle_mesh_ptr; 

class c_triangle_face : public c_shape
{
public: 
    c_triangle_face(triangle_mesh_ptr& mesh, uint32_t face_idx)
        : m_mesh(mesh)
        , m_face_idx(face_idx)
        , m_impl(mesh->get_face_impl_ptr())
    {}
    
    virtual bool intersects(const c_ray& ray, 
        PARAM_OUT float *t_hit, PARAM_OUT float *ray_epsilon, PARAM_OUT diff_geom_ptr& diff_geom) const; 
    
    void get_uv(float uv[3][2]) const 
    {
        
    }
    
private:
    triangle_mesh_ptr m_mesh;
    triangle_face_impl_ptr m_impl;

    uint32_t m_face_idx;
}; 