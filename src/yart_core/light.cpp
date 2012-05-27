#include "light.h"
#include "scene.h"

bool c_occlusion_tester::is_occluded(scene_ptr scene) const
{
	return scene->query_intersection(ray);
} 