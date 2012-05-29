#include "light.h"
#include "scene.h"
#include "intersection.h"

bool c_occlusion_tester::is_occluded(scene_ptr scene) const
{
	c_intersection isect;
	return scene->query_intersection(ray, &isect);
} 