#pragma once 

#include "assimp/assimp.h"
#include "assimp/aiMesh.h"

#include "cml/cml.h"

using namespace cml;

inline vector3f aivec3_to_cmlvec3(const aiVector3D& v)
{
	return vector3f(v.x, v.y, v.z); 
}

void ai_build_coord_system(const aiVector3D& v1, PARAM_OUT aiVector3D *v2, PARAM_OUT aiVector3D *v3);

aiVector3D cmlvec3_to_aivec3(const vector3f& v); 

aiMatrix4x4 cmlmat4x4_to_aimat4x4(const matrix44f& m);

aiVector3D aivec3_cross(const aiVector3D& v1, const aiVector3D& v2); 
