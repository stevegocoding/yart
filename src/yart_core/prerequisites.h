#pragma once

#include "boost/shared_ptr.hpp"
#include "boost/shared_array.hpp"
#include "boost/scoped_array.hpp"
#include "boost/pool/object_pool.hpp"

#include "cml/cml.h"

using namespace boost;

//////////////////////////////////////////////////////////////////////////
// Class Forward Declarations
//////////////////////////////////////////////////////////////////////////

// Geometry Classes
class c_ray;
class c_transform;

class c_render_target;
class c_camera;
class c_perspective_camera;
class c_display;

class c_rng;
struct c_camera_sample;
struct c_sample;

class c_sampler; 
class c_stratified_sampler; 

class c_filter; 

class c_rgb_spectrum; 
typedef c_rgb_spectrum c_spectrum; 

class c_renderer; 
class c_integrator;
class c_surface_integrator; 
class c_volume_integrator;

class c_light; 
class c_material; 
class c_aabb; 
class c_scene;
class c_scene_primtive; 
class c_accel_structure; 
class c_shape;
class c_differential_geometry; 
class c_triangle_mesh; 
class c_triangle_face;
// Assimp implementation 
class c_assimp_mesh_impl; 
class c_assimp_face_impl; 
class c_instanced_scene_obj; 
struct c_intersection; 
struct c_light_sample_record; 
struct c_bsdf_sample_record; 

class c_bsdf;
class c_bxdf_base; 

typedef shared_ptr<c_transform> transform_ptr; 
typedef shared_ptr<c_perspective_camera> perspective_camera_ptr;
typedef shared_ptr<c_rng> rng_ptr;
typedef shared_ptr<c_sample> sample_ptr;

typedef shared_ptr<c_sampler> sampler_ptr;
typedef shared_ptr<c_stratified_sampler> stratified_sampler_ptr; 

typedef shared_ptr<c_renderer> renderer_ptr;
typedef shared_ptr<c_integrator> integrator_ptr; 
typedef shared_ptr<c_surface_integrator> surface_integrator_ptr; 
typedef shared_ptr<c_volume_integrator> volume_integrator_ptr;
typedef shared_ptr<c_display> display_ptr; 

typedef shared_ptr<c_bsdf> bsdf_ptr;
typedef shared_ptr<c_light> light_ptr;
typedef shared_ptr<c_material> material_ptr; 
typedef shared_ptr<c_aabb> aabb_ptr;
typedef shared_ptr<c_scene> scene_ptr; 
typedef shared_ptr<c_scene_primtive> scene_primitive_ptr;
typedef shared_ptr<c_accel_structure> accel_structure_ptr; 
typedef shared_ptr<c_shape> shape_ptr;
typedef shared_ptr<c_differential_geometry> diff_geom_ptr;
typedef shared_ptr<c_triangle_mesh> triangle_mesh_ptr; 
typedef shared_ptr<c_triangle_face> triangle_face_ptr; 
typedef shared_ptr<c_assimp_mesh_impl> assimp_mesh_impl_ptr;
typedef shared_ptr<c_assimp_face_impl> assimp_face_impl_ptr;
typedef shared_array<c_light_sample_record> light_sample_record_array_ptr;
typedef shared_array<c_bsdf_sample_record> bsdf_sample_record_array_ptr;
typedef shared_array<c_ray> ray_array_ptr;

// typedef shared_ptr<c_bxdf_base> bxdf_ptr; 
// typedef shared_ptr<c_bsdf> bsdf_ptr;

typedef shared_ptr<c_render_target> render_target_ptr;
typedef shared_ptr<c_camera> camera_ptr;
typedef shared_ptr<c_filter> filter_ptr; 

typedef boost::scoped_array<c_spectrum> spectrum_array_ptr; 
typedef shared_array<c_sample> samples_array_ptr; 
typedef shared_array<float> samples_buf_ptr; 
typedef scoped_array<float> samples_buf_temp_ptr;

typedef shared_array<float> filter_table_ptr; 

typedef boost::shared_ptr<boost::pool<> > pool_ptr; 

using std::min;
using std::max; 
using std::swap; 
using std::sort;

typedef unsigned int uint32_t;
typedef cml::vector3f point3f; 

//////////////////////////////////////////////////////////////////////////
// Platform 
//////////////////////////////////////////////////////////////////////////
#if defined(_WIN32) || defined(_WIN64)
#define YART_WIN
#endif 

#if defined(YART_WIN)
#include <float.h>
#define isnan _isnan; 

#pragma warning (disable : 4305) // double constant assigned to float
#pragma warning (disable : 4244) // int -> float conversion
#pragma warning (disable : 4267) // size_t -> unsigned int conversion
#endif 

#define PARAM_OUT
#define PARAM_INOUT