#pragma once 

#include "prerequisites.h"
#include "transform.h"

class c_shape
{
public:
    c_shape(const c_transform& o2w, const c_transform& w2o)
        : m_obj_to_world(o2w)
        , m_world_to_obj(w2o)
    {} 
    
    virtual ~c_shape() {}

    virtual bool intersects(const c_ray& ray, float& t_hit, float& ray_epsilon) const { return false; }

protected:
    c_transform m_obj_to_world;
    c_transform m_world_to_obj; 

};