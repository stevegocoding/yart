#include "matte_material.h"
#include "memory.h"
#include "reflection.h"
#include "geometry.h" 

c_bsdf *c_matte_material::get_bsdf(const diff_geom_ptr& geom_dg, const diff_geom_ptr& shading_dg) const 
{
	pool_ptr pool = get_pool(sizeof(c_bsdf)); 
	c_bsdf *bsdf = NEW_BSDF(c_bsdf, pool)(shading_dg, geom_dg->nn); 

	if (m_sig == 0.f)
	{
		pool = get_pool(sizeof(c_lambertian)); 
		c_bxdf_base *lambertian = NEW_BSDF(c_lambertian, pool)(m_kd); 
		bsdf->add_bxdf(lambertian); 
	}
	
	return bsdf; 
}

material_ptr make_matte_material(const c_spectrum& kd, float sig)
{
	material_ptr mat = material_ptr(new c_matte_material(kd, sig)); 
	return mat; 
}