#include "reflection.h"

#include "monte_carlo.h"

c_spectrum c_bxdf_base::sample_f(const vector3f& wo, vector3f *wi, float u1, float u2, float *pdf) const
{
	// Cosine-sample the hemisphere, flipping the direction if necessary
    // p(w) = cos(theta) / pi
	*wi = cosine_sample_hemisphere(u1, u2); 

    // Flip wi's z if needed 
    if (wo[z] < 0.0f)
        (*wi)[z] *= -1.0f; 
    
    *pdf = eval_pdf(*wi, wo); 
    
    return f(wo, *wi); 
}

c_spectrum c_bxdf_base::rho(const vector3f& wo, int num_samples, samples_buf_ptr samples) const 
{
    c_spectrum r(0.0f); 
    for (int i = 0; i < num_samples; ++i)
    {
        vector3f wi; 
        float pdf = 0.0f; 
        c_spectrum f = sample_f(wo, &wi, samples[2*i], samples[2*i+1], &pdf); 
        if (pdf > 0)
            r += f * abs_cos_theta(wi) / pdf;
    }
    return r / (M_PI * num_samples); 
}

c_spectrum c_bxdf_base::rho(int num_samples, samples_buf_ptr samples1, samples_buf_ptr samples2) const
{
    c_spectrum r(0.0f); 
    for (int i = 0; i < num_samples; ++i)
    {
        vector3f wo, wi;
        wo = uniform_sample_hemisphere(samples1[2*i], samples1[2*i+1]); 
        float pdf_o = INV_TWOPI; 
        float pdf_i = 0.f;
        c_spectrum f = sample_f(wo, &wi, samples2[2*i], samples2[2*i+1], &pdf_i); 
        if (pdf_i > 0)
            r += f * abs_cos_theta(wi) * abs_cos_theta(wo) / (pdf_o * pdf_i); 
    }
	return r / (M_PI * num_samples); 
} 

float c_bxdf_base::eval_pdf(const vector3f& wo, const vector3f& wi) const
{
    float pdf = is_same_hemisphere(wo, wi) ? abs_cos_theta(wi) * INV_PI : 0.0f; 
    return pdf; 
}


//////////////////////////////////////////////////////////////////////////


c_bsdf::c_bsdf(const c_differential_geometry& shading_dg, const vector3f& ngeom, float eta)
    : m_shading_dg(shading_dg)
    , m_ng(ngeom)
{
    // Build shading coordinates system
    m_nn = shading_dg.nn; 
    m_sn = normalize(shading_dg.dpdu); 
    m_tn = cross(m_nn, m_sn);
}

float c_bsdf::eval_pdf(const vector3f& world_wo, const vector3f& world_wi, e_bxdf_type flags /* = bsdf_all */) const 
{
    if (m_bxdf_vec.size() == 0) 
        return 0.0f; 
    
    vector3f wo = world_to_local(world_wo); 
    vector3f wi = world_to_local(world_wi); 
    
    float pdf = 0.0f; 
    int num_matching = 0; 
    for (uint32_t i = 0; i < m_bxdf_vec.size(); ++i)
    {
        if (m_bxdf_vec[i]->match_flags(flags)) 
        {
            ++num_matching; 
            pdf += m_bxdf_vec[i]->eval_pdf(wo, wi); 
        }
    }
    float ret = num_matching > 0? pdf / num_matching : 0.0f; 
    return ret; 
}

c_spectrum c_bsdf::f(const vector3f& world_wo, const vector3f& world_wi, e_bxdf_type flags /* = bsdf_all */) const 
{
    vector3f wo = world_to_local(world_wo); 
    vector3f wi = world_to_local(world_wi); 
    
    // If the world wo and wi are in the same size of the surface, then ignore BTDF
    if (dot(world_wo, m_ng) * dot(world_wi, m_ng) > 0) 
        flags = e_bxdf_type(flags & ~bsdf_transmission);
    else        // ignore BRDF otherwise 
        flags = e_bxdf_type(flags & ~bsdf_reflection); 
    
    c_spectrum f(0.0f); 
    for (uint32_t i = 0; i < m_bxdf_vec.size(); ++i)
    {
        if (m_bxdf_vec[i]->match_flags(flags))
            f += m_bxdf_vec[i]->f(wo, wi); 
    }
    return f; 
}

c_spectrum c_bsdf::sample_f(const vector3f& world_wo, PARAM_OUT vector3f *world_wi, 
    const c_bsdf_sample& bsdf_sample, PARAM_OUT float *pdf, 
    e_bxdf_type flags /* = bsdf_all */, PARAM_OUT e_bxdf_type *sampled_type /* = NULL */) const
{
    // Choose which bxdf to sample 
    int num_bxdf_matched = num_bxdf_by_type(flags); 
    if (num_bxdf_matched == 0)
    {
        *pdf = 0.f; 
        if (sampled_type) 
            *sampled_type = e_bxdf_type(0);
        return c_spectrum(0.0f); 
    }
    int matched_idx = min((int)std::floorf(bsdf_sample.u_component * num_bxdf_matched), num_bxdf_matched-1);
    
    c_bxdf_base *bxdf;
    for (uint32_t i = 0; i < m_bxdf_vec.size(); ++i)
    {
        if (m_bxdf_vec[i]->match_flags(flags) && matched_idx-- == 0) 
        {
            bxdf = m_bxdf_vec[i]; 
            break;
        }
    }
    assert(bxdf); 

    // Sample the chosen the bxdf
    vector3f wo = world_to_local(world_wo); 
    vector3f wi; 
    *pdf = 0.0f; 
    c_spectrum f = bxdf->sample_f(wo, &wi, bsdf_sample.u_dir[0], bsdf_sample.u_dir[1], pdf); 
    if (*pdf == 0.0f)
    {
        if (sampled_type) 
            sampled_type = e_bxdf_type(0); 
        return c_spectrum(0.0f); 
    }
    
    if (sampled_type)
        *sampled_type = bxdf->get_type(); 
    
    *world_wi = local_to_world(wi);
    
    // Compute overall pdf with all the matched bxdf 
    // ignore the specular bxdf since the pdf of delta distribution is 0
    if (!(bxdf->get_type() == bsdf_specular) && num_bxdf_matched > 1) 
    {
        for (uint32_t i = 0; i < m_bxdf_vec.size(); ++i)
        {
            if (m_bxdf_vec[i] != bxdf && m_bxdf_vec[i]->match_flags(flags))
                *pdf += m_bxdf_vec[i]->eval_pdf(wo, wi); 
        }
    }
    if (num_bxdf_matched > 1)
        *pdf /= num_bxdf_matched; 

    // Compute the value of bsdf for sampled direction 
    // ignore the specular bxdf 
    if (!(bxdf->get_type() == bsdf_specular))
    {
        f = c_spectrum(0.0f); 
        // If the world wo and wi are in the same size of the surface, then ignore BTDF
        if (dot(world_wo, m_ng) * dot(*world_wi, m_ng) > 0) 
            flags = e_bxdf_type(flags & ~bsdf_transmission);
        else        // ignore BRDF otherwise 
            flags = e_bxdf_type(flags & ~bsdf_reflection); 
        for (uint32_t i = 0; i < m_bxdf_vec.size(); ++i)
        {
            if (m_bxdf_vec[i]->match_flags(flags))
                f += m_bxdf_vec[i]->f(wo, wi); 
        }
    }
    return f;  
}

c_spectrum c_bsdf::rho(c_rng& rng, e_bxdf_type flags /* = bsdf_all */, int sqrt_samples /* = 6 */) const 
{
    int num_samples = sqrt_samples * sqrt_samples; 
    samples_buf_ptr samples1(new float[2 * num_samples]);
    samples_buf_ptr samples2(new float[2 * num_samples]);
    stratified_sample_2D(samples1, sqrt_samples, sqrt_samples, rng); 
    stratified_sample_2D(samples2, sqrt_samples, sqrt_samples, rng); 
    
    c_spectrum ret(0.0f); 
    for (uint32_t i = 0; i < m_bxdf_vec.size(); ++i)
    {
        if (m_bxdf_vec[i]->match_flags(flags))
            ret += m_bxdf_vec[i]->rho(num_samples, samples1, samples2); 
    }

    return ret; 
}

c_spectrum c_bsdf::rho(const vector3f& wo, c_rng& rng, e_bxdf_type flags, int sqrt_samples) const
{
    int num_samples = sqrt_samples * sqrt_samples; 
    samples_buf_ptr samples1(new float[2 * num_samples]);
    stratified_sample_2D(samples1, sqrt_samples, sqrt_samples, rng); 
    
    c_spectrum ret(0.0f); 
    for (uint32_t i = 0; i < m_bxdf_vec.size(); ++i)
    {
        if (m_bxdf_vec[i]->match_flags(flags))
            ret += m_bxdf_vec[i]->rho(wo, num_samples, samples1); 
    }
    return ret; 
}

int c_bsdf::num_bxdf_by_type(e_bxdf_type type) const 
{
    int num = 0; 
    for (uint32_t i = 0; i < m_bxdf_vec.size(); ++i)
    {
        if (m_bxdf_vec[i]->match_flags(type))
            ++num; 
    }
    return num;
}

void c_bsdf::add_bxdf(c_bxdf_base *bxdf) 
{
	assert(bxdf); 
	m_bxdf_vec.push_back(bxdf); 
}