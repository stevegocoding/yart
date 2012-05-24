#pragma once 

#include "prerequisites.h" 
#include "color.h" 

class c_material 
{
public:
    virtual c_bsdf *get_bsdf(const diff_geom_ptr& geom_dg, 
                             const diff_geom_ptr& shading_dg) const = 0; 

    virtual ~c_material() {}
};