#include "assimp_api_wrap.h" 

vector3f aivec3_to_cmlvec3(const aiVector3D& v)
{
    return vector3f(v.x, v.y, v.z); 
}

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