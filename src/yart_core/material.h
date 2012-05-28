#pragma once 

#include "prerequisites.h" 
#include "color.h" 

class c_material 
{
public:
    virtual c_bsdf *get_bsdf(const c_differential_geometry& geom_dg, 
                             const c_differential_geometry& shading_dg) const = 0; 

    virtual ~c_material() {}
};