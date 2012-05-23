#pragma once 

#include "prerequisites.h"
#include "transform.h"
#include "color.h"
#include "math_utils.h" 

namespace 
{
	enum e_bxdf_type
	{
		bsdf_relection = 1<<0, 
		bsdf_transmission = 1<<1, 
		bsdf_diffuse = 1<<2, 
		bsdf_specular = 1<<3
	};

	static const uint32_t MAX_BxDFS = 8; 
}

class c_bsdf
{
public:
	~c_bsdf() {}
	
private:
	std::vector<bxdf_ptr> m_bxdf_vec; 
};

class c_bxdf_base
{
public:
	virtual ~c_bxdf_base() {}
	
	c_bxdf_base(e_bxdf_type t)
		: m_type(t)
	{}

	virtual c_spectrum f(const vector3f& wo, const vector3f& wi) const = 0; 
	virtual c_spectrum sample_f(const vector3f& wo, vector3f& wi, float u1, float u2, float *pdf) const; 
	virtual c_spectrum rho(const vector3f& wo, int num_samples, const samples_array_ptr& samples) const; 
	virtual c_spectrum rho(int num_samples, const samples_array_ptr& samples1, const samples_array_ptr& samples2) const; 
 
protected: 
	e_bxdf_type m_type; 
};

class c_lambertian : public c_bxdf_base
{
	typedef c_bxdf_base super;
public:
	c_lambertian(const c_spectrum& reflectance)
		: super (e_bxdf_type(bsdf_relection | bsdf_diffuse))
		, m_reflectance(reflectance) {}
	
	virtual c_spectrum f(const vector3f& wo, const vector3f& wi) const 
    {
        return m_reflectance * INV_PI;
    }
    virtual c_spectrum rho(const vector3f& wo, int num_samples, const samples_array_ptr& samples) const { return m_reflectance; }


private: 
	c_spectrum m_reflectance; 
};