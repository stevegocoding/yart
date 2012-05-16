#include <assert.h>
#include "triangle_mesh.h" 
#include "assimp_api_wrap.h"

class c_assimp_mesh_impl : public c_triangle_mesh_impl
{
public: 
    c_assimp_mesh_impl(aiMesh *mesh) 
        : m_ai_mesh(mesh)
    {}

    bool has_normal() const 
    {
        assert(m_ai_mesh); 
        return m_ai_mesh->HasNormals(); 
    }

    uint32_t get_num_faces() const 
    {
        assert(m_ai_mesh); 
        m_ai_mesh->mNumFaces; 
    }
    
    uint32_t get_num_verts() const 
    {
        assert(m_ai_mesh); 
        m_ai_mesh->mNumVertices;
    }
    
    triangle_face get_face(uint32_t idx) const 
    {
        assert(m_ai_mesh); 
        assert(idx < m_ai_mesh->mNumFaces); 
        aiFace *face = &m_ai_mesh->mFaces[idx];
        assert(face->mNumIndices == 3);
        
        return triangle_face(face->mIndices[0], face->mIndices[1], face->mIndices[2]); 
    }

    point3f get_vert(uint32_t idx) const 
    {
        assert(m_ai_mesh);
        assert(idx < m_ai_mesh->mNumVertices);
        
        return aivec3_to_cmlvec3(m_ai_mesh->mVertices[idx]); 
    }

    aiMesh *m_ai_mesh; 
};


//////////////////////////////////////////////////////////////////////////

class c_assimp_face_impl : public c_triangle_face_impl
{
public:
    c_assimp_face_impl(assimp_mesh_impl_ptr& assimp_mesh, uint32_t face_idx)
        : m_mesh(assimp_mesh)
        , m_face_idx(face_idx)
    {
    }
    

    
private:
    assimp_mesh_impl_ptr m_mesh;
    uint32_t m_face_idx; 
}; 

//////////////////////////////////////////////////////////////////////////


bool c_triangle_face::intersects(const c_ray& ray, 
    PARAM_OUT float *t_hit, PARAM_OUT float *ray_epsilon, PARAM_OUT diff_geom_ptr& diff_geom) const 
{
    triangle_face face = m_mesh->get_face(m_face_idx);

    point3f p1 = m_mesh->get_vert(face[0]);
    point3f p2 = m_mesh->get_vert(face[1]);
    point3f p3 = m_mesh->get_vert(face[2]);

    vector3f e1 = p2 - p1;
    vector3f e2 = p3 - p2; 
    
    vector3f s1 = cross(ray.d, e2); 
    float divisor = dot(s1, e1); 
    
    if (divisor == 0)
        return false;
    
    float inv_div = 1.0f / divisor; 

    // Compute the first barycentric coordinates
    vector3f d = ray.o - p1; 
    float b1 = dot(d, s1) * inv_div;
    if (b1 < 0.0f || b1 > 1.0f)
        return false;

    // Compute the second barycentric coordinates 
    vector3f s2 = cross(d, e1); 
    float b2 = dot(ray.d, s2) * inv_div; 
    if (b2 < 0.0f || b1 + b2 > 1.0f)
        return false;

    // Compute t to intersection point 
    float t = dot(e2, s2) * inv_div; 
    if (t < ray.t_min || t > ray.t_max)
        return false; 
    
    // Compute the triangle partial derivatives 
    vector3f dpdu, dpdv; 
    float uvs[3][2];
    get_uv(uvs);
    
    // Compute deltas for triangle partial derivatives 
    float du1 = uvs[0][0] - uvs[2][0]; 
    float du2 = uvs[1][0] - uvs[2][0]; 
    float dv1 = uvs[0][1] - uvs[2][1]; 
    float dv2 = uvs[1][1] - uvs[2][1]; 
    vector3f dp1 = p1 - p3; 
    vector3f dp2 = p2 - p3; 
    float determinant = du1 * dv2 - dv1 * du2; 
    if (determinant == 0.0f)
    {
        // Handle zero determinant for triangle partial derivative matrix
        
    }





    return true; 
}