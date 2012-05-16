#include "assimp_api_wrap.h" 

vector3f aivec3_to_cmlvec3(const aiVector3D& v)
{
    return vector3f(v.x, v.y, v.z); 
}

aiVector3D cmlvec3_to_aivec3(const vector3f& v)
{
    return aiVector3D(v[0], v[1], v[2]); 
}

aiVector3D aivec3_cross(const aiVector3D& v1, const aiVector3D& v2)
{
    vector3f _v1 = aivec3_to_cmlvec3(v1); 
    vector3f _v2 = aivec3_to_cmlvec3(v2); 
    vector3f v = cross(_v1, _v2); 
    return aiVector3D(v[0], v[1], v[2]);
}