#pragma once

#include "boost/shared_ptr.hpp"
#include "boost/shared_array.hpp"

using namespace boost;

//////////////////////////////////////////////////////////////////////////
// Class Forward Declarations
//////////////////////////////////////////////////////////////////////////

// Geometry Classes
class c_ray;

//
class c_film;
class c_camera;


class c_camera_sample;

typedef shared_ptr<c_film> film_ptr;
typedef shared_ptr<c_camera> camera_ptr;

