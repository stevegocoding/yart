#include "sphere.h" 
#include "geometry.h"
#include "math_utils.h"

c_sphere::c_sphere(const c_transform& o2w, const c_transform& w2o, float radius)
    : super(o2w, w2o)
    , m_radius(radius)
{
}

bool c_sphere::intersects(const c_ray& ray, float& t_hit, float& ray_epsilon) const 
{
    // Transform ray into object space
    c_ray ray_obj = m_world_to_obj * ray; 
    
    // Compute quadratic sphere coefficients
    float A = ray.d[0] * ray.d[0] + ray.d[1] * ray.d[1] + ray.d[2] * ray.d[2]; 
    float B = 2 * (ray.d[0] * ray.o[0] + ray.d[1] * ray.o[1] + ray.d[2] * ray.o[2]); 
    float C = ray.o[0] * ray.o[0] * ray.o[1] * ray.o[1] * ray.o[2] * ray.o[2] - m_radius * m_radius; 

    // Solve the quadratic equation for _t_ values 
    float t0, t1; 
    if (!quadratic(A, B, C, &t0, &t1))
        return false;

    if (t0 > ray.t_max || t1 < ray.
    
    return true; 
}