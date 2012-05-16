#pragma once 

#include "prerequisites.h"
#include "transform.h"

class c_shape
{
public:
    c_shape() {}
    c_shape(const c_transform& o2w, const c_transform& w2o)
        : m_obj_to_world(o2w)
        , m_world_to_obj(w2o)
    {} 
    
    virtual ~c_shape() {}

    virtual bool intersects(const c_ray& ray, 
        PARAM_OUT float *t_hit, PARAM_OUT float *ray_epsilon, PARAM_OUT diff_geom_ptr& diff_geom) const { return false; }

protected:
    c_transform m_obj_to_world;
    c_transform m_world_to_obj; 

};