#pragma once

#include "prerequisites.h" 
#include "material.h" 

class c_matte_material : public c_material
{
public: 
    c_matte_material(const c_spectrum& kd, float sig)
		: m_kd(kd)
		, m_sig(sig)
	{}
	
	virtual c_bsdf *get_bsdf(const c_differential_geometry& geom_dg, const c_differential_geometry& shading_dg) const; 
				
private:
 
    c_spectrum m_kd; 
	float m_sig;
    
}; 

material_ptr make_matte_material(const c_spectrum& kd, float sig); 