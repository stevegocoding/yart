#include "assimp_api_wrap.h" 

aiVector3D cmlvec3_to_aivec3(const vector3f& v)
{
    return aiVector3D(v[0], v[1], v[2]); 
}

aiMatrix4x4 cmlmat4x4_to_aimat4x4(const matrix44f& m)
{
	aiMatrix4x4 _m(
		m(0,0), m(0,1), m(0,2), m(0,3),
		m(1,0), m(1,1), m(1,2), m(1,3),
		m(2,0), m(2,1), m(2,2), m(2,3),
		m(3,0), m(3,1), m(3,2), m(3,3)); 
	
	return _m; 
}

aiVector3D aivec3_cross(const aiVector3D& v1, const aiVector3D& v2)
{
    vector3f _v1 = aivec3_to_cmlvec3(v1); 
    vector3f _v2 = aivec3_to_cmlvec3(v2); 
    vector3f v = cross(_v1, _v2); 
    return aiVector3D(v[0], v[1], v[2]);
}

void ai_build_coord_system(const aiVector3D& v1, PARAM_OUT aiVector3D *v2, PARAM_OUT aiVector3D *v3)
{
	if (fabsf(v1.x) > fabsf(v1.y))
	{
		float inv_len = 1.0f / sqrtf(v1.x * v1.x + v1.z * v1.z); 
		*v2 = aiVector3D(-v1.z * inv_len, 0.0f, v1.x * inv_len);		
	}
	else 
	{
		float inv_len = 1.0f / sqrtf(v1.y * v1.y + v1.z * v1.z);
		*v2 = aiVector3D(0.0f, v1.z * inv_len, -v1.y * inv_len); 
	}

	*v3 = v1 ^ (*v2);

}
