#include "boost/make_shared.hpp"

#include "scene_obj.h"
#include "shape.h"

c_scene_object::c_scene_object(const c_transform& w2o, const c_transform& o2w, shape_ptr geometry)
	: m_world_to_obj(w2o)
	, m_obj_to_world(o2w) 
	, m_shape(geometry)
{
	if (geometry)
	{
		geometry->apply_transform(o2w);
	}
}

scene_object_ptr make_simple_scene_obj(const c_transform& w2o, const c_transform& o2w, shape_ptr geo)
{
	scene_object_ptr obj = boost::make_shared<c_scene_object>(w2o, o2w, geo); 
	return obj; 
}