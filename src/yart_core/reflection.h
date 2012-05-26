#pragma once 

#include "prerequisites.h"
#include "memory.h"
#include "transform.h"
#include "color.h"
#include "math_utils.h" 
#include "sampler.h"

namespace 
{
	enum e_bxdf_type
	{
		bsdf_reflection = 1<<0, 
		bsdf_transmission = 1<<1, 
		bsdf_diffuse = 1<<2,
        bsdf_glossy = 1<<3,
		bsdf_specular = 1<<4, 
        bsdf_all_types = bsdf_diffuse | bsdf_glossy | bsdf_specular, 
        bsdf_all_reflection = bsdf_reflection | bsdf_all_types,
        bsdf_all_transmission = bsdf_transmission | bsdf_all_types,
        bsdf_all = bsdf_all_reflection | bsdf_all_transmission
	};

	static const uint32_t MAX_BxDFS = 8; 

    bool is_same_hemisphere(const vector3f& wi, const vector3f& wo)
    {
        return wi[z] * wo[z] > 0.0f;
    }

    float abs_cos_theta(const vector3f& w)
    {
        return fabs(w[z]);
    }
}

struct c_bsdf_sample_record
{ 
	c_bsdf_sample_record()
		: num_samples(0) 
		, comp_sample_buf_idx(0)
		, dir_sample_buf_idx(0)
	{}
	
	c_bsdf_sample_record(uint32_t ns, c_sample *sample) 
	{
		num_samples = ns;
		comp_sample_buf_idx = sample->add_1D(ns);
		dir_sample_buf_idx = sample->add_2D(ns);
	}
	
	uint32_t num_samples;
	uint32_t comp_sample_buf_idx;
	uint32_t dir_sample_buf_idx; 
};

struct c_bsdf_sample
{
    c_bsdf_sample(float _u_dir[2], float u_comp)
    {
        assert(_u_dir[0] >= 0.0f && _u_dir[0] < 1.0f);
        assert(_u_dir[1] >= 0.0f && _u_dir[1] < 1.0f); 
        assert(u_comp >= 0.0f && u_comp < 1.0f); 
        u_dir[0] = _u_dir[0]; 
        u_dir[1] = _u_dir[1];
        u_component = u_comp; 
    }
	
	c_bsdf_sample(const c_sample *sample, c_bsdf_sample_record& rec, uint32_t sample_idx)
	{
		assert(sample_idx < sample->get_2D_samples_size(rec.dir_sample_buf_idx));
		assert(sample_idx < sample->get_1D_samples_size(rec.comp_sample_buf_idx)); 
		
		u_dir[0] = sample->get_2D_samples_buf(rec.dir_sample_buf_idx)[2*sample_idx];
		u_dir[1] = sample->get_2D_samples_buf(rec.dir_sample_buf_idx)[2*sample_idx+1];
		u_component = sample->get_1D_samples_buf(rec.comp_sample_buf_idx)[sample_idx]; 
		
		assert(u_dir[0] >= 0.0f && u_dir[0] < 1.0f);
		assert(u_dir[1] >= 0.0f && u_dir[1] < 1.0f);
	} 

    float u_component; 
    float u_dir[2];
}; 

class c_bsdf 
{
public:
    c_bsdf(const diff_geom_ptr& shading_dg, const vector3f& ngeom, float eta = 1.0f); 
	~c_bsdf() {}

    float eval_pdf(const vector3f& world_wo, const vector3f& world_wi, e_bxdf_type flags = bsdf_all) const; 

    c_spectrum f(const vector3f& world_wo, const vector3f& world_wi, e_bxdf_type flags = bsdf_all) const;

    c_spectrum sample_f(const vector3f& world_wo, PARAM_OUT vector3f *world_wi, 
        const c_bsdf_sample& bsdf_sample, PARAM_OUT float *pdf, 
        e_bxdf_type flags = bsdf_all, PARAM_OUT e_bxdf_type *sampled_type = NULL) const; 
    
    c_spectrum rho(c_rng& rng, e_bxdf_type flags = bsdf_all, int sqrt_samples = 6) const; 
    c_spectrum rho(const vector3f& wo, c_rng& rng, e_bxdf_type flags, int sqrt_samples) const;

	void add_bxdf(c_bxdf_base *bxdf);
	
    /** 
        Transform vector from world space to local shading space
    */ 
    vector3f world_to_local(const vector3f& local_v) const 
    {
        return vector3f(dot(local_v, m_sn), dot(local_v, m_tn), dot(local_v, m_nn)); 
    }
    
    /**
        Transform vector from local shading space to world space 
    */ 
    vector3f local_to_world(const vector3f& world_v) const
    {
        return vector3f(m_sn[0] * world_v[0] + m_tn[0] * world_v[1] + m_nn[0] * world_v[2], 
                        m_sn[1] * world_v[0] + m_tn[1] * world_v[1] + m_nn[1] * world_v[2], 
                        m_sn[2] * world_v[0] + m_tn[2] * world_v[1] + m_nn[2] * world_v[2]);
    }
	
private:

    int num_bxdf_by_type(e_bxdf_type type) const; 

	std::vector<c_bxdf_base*> m_bxdf_vec; 

    // Shading differential geometry
    diff_geom_ptr m_shading_dg; 
    
    vector3f m_ng;
    vector3f m_nn; 
    vector3f m_sn; 
    vector3f m_tn; 

    float m_eta; 
};

class c_bxdf_base  
{
public:
	virtual ~c_bxdf_base() {}
	
	c_bxdf_base(e_bxdf_type t)
		: m_type(t)
	{}

    bool match_flags(e_bxdf_type type_flags) const 
    {
        return (m_type & type_flags) == m_type; 
    }

    e_bxdf_type get_type() const
    {
        return m_type; 
    }

	virtual c_spectrum f(const vector3f& wo, const vector3f& wi) const = 0; 
    
    /**
        In default, a cosine-hemisphere distribution is used for sampling the bxdf
    */
	virtual c_spectrum sample_f(const vector3f& wo, vector3f *wi, float u1, float u2, float *pdf) const; 

    /** 
        Estimate the hemispherical-directional reflectance with monte-carlo estimator.
    */ 
	virtual c_spectrum rho(const vector3f& wo, int num_samples, samples_buf_ptr samples) const; 
    
    /**
        Estimate the hemispherical-hemispherical reflectance with monte-carlo estimator. 
    */ 
	virtual c_spectrum rho(int num_samples, samples_buf_ptr samples1, samples_buf_ptr samples2) const; 

    
    virtual float eval_pdf(const vector3f& wo, const vector3f& wi) const;
 
protected: 
	e_bxdf_type m_type; 
};

////////////////////////////////////////////////////////////////////////// 

class c_lambertian : public c_bxdf_base 
{
	typedef c_bxdf_base super;
public:
	c_lambertian(const c_spectrum& reflectance)
		: super (e_bxdf_type(bsdf_reflection | bsdf_diffuse))
		, m_reflectance(reflectance) 
	{}
	
	virtual c_spectrum f(const vector3f& wo, const vector3f& wi) const 
    {
        return m_reflectance * INV_PI;
    }
    virtual c_spectrum rho(const vector3f& wo, int num_samples, const samples_array_ptr& samples) const { return m_reflectance; }


private: 
	c_spectrum m_reflectance; 
};
