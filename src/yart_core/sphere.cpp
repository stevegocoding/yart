#include "sphere.h" 
#include "geometry.h"
#include "math_utils.h"

c_sphere::c_sphere(const c_transform& o2w, const c_transform& w2o, float radius, float z_min, float z_max, float phi_max_deg) 
    : super(o2w, w2o)
    , m_radius(radius)
{
    m_z_min = clamp(min(z_min, z_max), -radius, radius); 
    m_z_max = clamp(max(z_min, z_max), -radius, radius); 
    
    m_theta_min = acosf(clamp(m_z_min/m_radius, -1.0f, 1.0f)); 
    m_theta_max = acosf(clamp(m_z_max/m_radius, -1.0f, 1.0f)); 
    
    m_phi_max = cml::rad(clamp(phi_max_deg, 0.0f, 360.0f)); 
}

bool c_sphere::intersects(const c_ray& ray, float *t_hit, float *ray_epsilon) const 
{
    float phi = 0; 
    vector3f hit_sphr; 
    
    // Transform ray into object space
    c_ray ray_obj = m_world_to_obj * ray; 
    
    // Compute quadratic sphere coefficients
    float A = ray.d[x] * ray.d[x] + ray.d[y] * ray.d[y] + ray.d[z] * ray.d[z]; 
    float B = 2 * (ray.d[x] * ray.o[x] + ray.d[y] * ray.o[y] + ray.d[z] * ray.o[z]); 
    float C = ray.o[x] * ray.o[x] * ray.o[y] * ray.o[y] * ray.o[z] * ray.o[z] - m_radius * m_radius; 

    // Solve the quadratic equation for _t_ values 
    float t0, t1; 
    if (!quadratic(A, B, C, &t0, &t1))
        return false;

    if (t0 > ray_obj.t_max || t1 < ray_obj.t_min)
        return false; 
    float _t_hit = t0; 
    if (t0 < ray.t_min) 
    {
        _t_hit = t1; 
        if (_t_hit > ray.t_max) 
            return false;
    }
   
    // Compute sphere hit position
    /* 
        According the definition of sphere equation 
        tan(phi) = y / x = (r * sin(theta) * sin(phi)) / (r * sin(theta) * cos(phi))
        so phi = arctan(y/x)
        The atan2f function in C std lib returns value in [-PI, PI], need to shift it to [0, 2*PI]
    */
    hit_sphr = ray.evaluate_t(_t_hit); 
    if (hit_sphr[x] == 0.0f && hit_sphr[y] == 0.0f)
        hit_sphr[x] = 1e-5f * m_radius;
    phi = atan2f(hit_sphr[y], hit_sphr[x]);
    if (phi < 0.0f) 
        phi += 2.0f * M_PI; 
    
    // Test sphere intersection against clipping parameters
    if ((m_z_min > -m_radius && hit_sphr[z] < m_z_min) || 
        (m_z_max < m_radius && hit_sphr[z] > m_z_max) || 
        (phi > m_phi_max)
       )
    {
        if (_t_hit == t1)
            return false;
        if (t1 > ray.t_max) 
            return false; 
        _t_hit = t1; 
        hit_sphr = ray.evaluate_t(_t_hit); 
        if (hit_sphr[x] == 0.0f && hit_sphr[y] == 0.0f)
            hit_sphr[x] = 1e-5f * m_radius; 
        phi = atan2f(hit_sphr[y], hit_sphr[x]); 
        if (phi < 0.0f) 
            phi += 2.0f * M_PI; 
        if ((m_z_min > -m_radius && hit_sphr[z] < m_z_min) ||
            (m_z_max < m_radius && hit_sphr[z] > m_z_max) ||
            phi > m_phi_max
            )
            return false; 
    }

    // Find parametric representation of sphere hit
    /*
    float u = phi / m_phi_max; 
    float theta = acosf(clamp(hit_sphr[z] / m_radius, -1.0f, 1.0f));
    float v = (theta - m_theta_min) / (m_theta_max - m_theta_min);
    */
    
     // Update _tHit_ for quadric intersection
    *t_hit = _t_hit;

    // Compute _rayEpsilon_ for quadric intersection
    *ray_epsilon = 5e-4f * *t_hit; 
    
    return true;
}
