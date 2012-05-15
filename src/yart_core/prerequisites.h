#pragma once

#include "boost/shared_ptr.hpp"
#include "boost/shared_array.hpp"

using namespace boost;

//////////////////////////////////////////////////////////////////////////
// Class Forward Declarations
//////////////////////////////////////////////////////////////////////////

// Geometry Classes
class c_ray;

class c_film;
class c_camera;
class c_perspective_camera;

class c_rng;
struct c_camera_sample;
struct c_sample;

class c_sampler; 
class c_stratified_sampler; 

class c_rbg_spectrum; 
typedef c_rbg_spectrum c_spectrum; 

class c_integrator;
class c_surface_integrator; 
class c_volume_integrator;

class c_scene;

typedef shared_ptr<c_perspective_camera> perspective_camera_ptr;
typedef shared_ptr<c_rng> rng_ptr;
typedef shared_ptr<c_sample> sample_ptr;

typedef shared_ptr<c_sampler> sampler_ptr;
typedef shared_ptr<c_stratified_sampler> stratified_sampler_ptr; 

typedef shared_ptr<c_integrator> integrator_ptr; 
typedef shared_ptr<c_surface_integrator> surface_integrator_ptr; 
typedef shared_ptr<c_volume_integrator> volume_integrator_ptr;

typedef shared_ptr<c_scene> scene_ptr; 

typedef shared_ptr<c_film> film_ptr;
typedef shared_ptr<c_camera> camera_ptr;

typedef shared_array<c_sample> samples_array_ptr; 
typedef shared_array<float> samples_buf_ptr; 

using std::min;
using std::max; 
using std::swap; 
using std::sort;

typedef unsigned int uint32_t;

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