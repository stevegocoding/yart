#pragma once

#include "prerequisites.h"
#include "shape.h"

class c_sphere : public c_shape
{
    typedef c_shape super;
    
public:
    c_sphere(const c_transform& o2w, const c_transform& w2o, float radius, float z_min, float z_max, float phi_max_deg); 

    virtual bool intersects(const c_ray& ray, float *t_hit, float *ray_epsilon) const; 

private:
    float m_radius; 
    float m_phi_max;
    float m_z_min, m_z_max; 
    float m_theta_min, m_theta_max;
}; 