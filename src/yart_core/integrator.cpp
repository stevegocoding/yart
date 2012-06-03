#include "integrator.h"

#include "ray.h"
#include "scene.h"

#include "sampler.h"
#include "monte_carlo.h"

c_spectrum estimate_direct_light_integral(const scene_ptr scene, 
	const c_renderer *render,
	const light_ptr light, 
	const point3f& p, 
	const vector3f& normal,
	const vector3f& wo, 
	float ray_eps,
	const c_bsdf *bsdf,
	c_rng& rng,
	const c_light_sample& light_sample,
	const c_bsdf_sample& bsdf_sample, 
	e_bxdf_type bsdf_flags)
{
	c_spectrum ld(0.0f); 
	
	// Sample light source with multiple importance sampling
	vector3f wi; 
	
	float light_pdf = 0.0f; 
	float bsdf_pdf = 0.0f; 
	
	c_occlusion_tester visibilty; 
	c_spectrum li = light->sample_l(p, ray_eps, light_sample, &wi, &light_pdf, &visibilty); 
	if (light_pdf > 0.0f && !li.is_black())
	{
		c_spectrum f = bsdf->f(wo, wi, bsdf_flags); 
		
		if (!f.is_black() && !visibilty.is_occluded(scene))
		{
			// Add light's contribution to reflected radiance
			// @TODO: Add light transimission
			
			if (light->is_delta())
			{
				ld += f * li * (abs(dot(wi, normal)) / light_pdf); 
			}	
			else 
			{
				bsdf_pdf = bsdf->eval_pdf(wo, wi, bsdf_flags); 
				float weight = power_heuristic(1, light_pdf, 1, bsdf_pdf); 
				ld += f * li *(abs(dot(wi, normal)) * weight / light_pdf); 
			}	
		}
		
		else
		{	
			if (f.is_black())
				ld += c_spectrum(0.0f, 0.0f, 1.0f); 
			else if (visibilty.is_occluded(scene)) 
				ld += c_spectrum(1.0f, 0.0f, 0.0f); 
		}		
		
	}
	
	// @TODO:
	// For non-delta light
	/*
	if (!light->is_delta())
	{
		e_bxdf_type sampled_type; 
		c_spectrum f = bsdf->sample_f(wo, &wi, bsdf_sample, &bsdf_pdf, bsdf_flags, &sampled_type); 
		if (!f.is_black() && bsdf_pdf > 0.0f)
		{
			float weight = 1.0f;
			if (!(sampled_type & bsdf_specular))
			{
				light_pdf = light->eval_pdf(p, wi); 
				if (light_pdf == 0.0f) 
					return ld;
				weight = power_heuristic(1, bsdf_pdf, 1, light_pdf); 
			}
			// Add light's contribution from BSDF sampling
			c_intersection light_intersect;
			c_spectrum li(0.0f); 
			c_ray ray(p, wi, ray_eps, INFINITY); 
			if (scene->query_intersection(ray, &light_intersect))
			{
			
			}	
		}
	}
	*/ 
	
	return ld;
}

c_spectrum uniform_all_lights(const scene_ptr scene, 
	const c_renderer *renderer, 
	const point3f& p, 
	const vector3f& normal, 
	const vector3f& wo, 
	float ray_eps, 
	const c_bsdf *bsdf, 
	const c_sample *sample, 
	c_rng& rng, 
	const light_sample_record_array_ptr light_samples_records, 
	const bsdf_sample_record_array_ptr bsdf_samples_records)
{
	c_spectrum lo(0.0f); 
	
	 for (uint32_t i = 0; i < scene->get_num_lights(); ++i)
	 {
		 light_ptr light = scene->get_light(i); 
		 int num_samples = light_samples_records[i].num_samples;
		 
		 c_spectrum ld(0.0f); 
		 for (uint32_t j = 0; j < (uint32_t)num_samples; ++j)
		 {
			 c_light_sample light_sample;
			 c_bsdf_sample bsdf_sample;

			 light_sample = c_light_sample(sample, light_samples_records[i], j);
			 bsdf_sample = c_bsdf_sample(sample, bsdf_samples_records[i], j); 
			 
			 ld += estimate_direct_light_integral(scene, renderer, light, p, normal, wo, ray_eps, bsdf, rng, light_sample, bsdf_sample, e_bxdf_type(bsdf_all & ~bsdf_specular));
		 } 
		 lo += ld / num_samples; 
	 }
	return lo;
}
