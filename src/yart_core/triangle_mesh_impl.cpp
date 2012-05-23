#include "triangle_mesh_impl.h"

c_assimp_mesh_impl::c_assimp_mesh_impl(aiMesh *mesh) 
	: m_ai_mesh(mesh)
{}

bool c_assimp_mesh_impl::has_tengent() const
{
	assert(m_ai_mesh); 
	return m_ai_mesh->HasTangentsAndBitangents();
}

bool c_assimp_mesh_impl::has_normal() const 
{
	assert(m_ai_mesh); 
	return m_ai_mesh->HasNormals(); 
}

bool c_assimp_mesh_impl::has_uv(uint32_t uv_set) const
{
	assert(m_ai_mesh); 
	return m_ai_mesh->HasTextureCoords(uv_set); 
}

uint32_t c_assimp_mesh_impl::get_num_faces() const 
{
	assert(m_ai_mesh); 
	return m_ai_mesh->mNumFaces; 
}

uint32_t c_assimp_mesh_impl::get_num_verts() const 
{
	assert(m_ai_mesh); 
	return m_ai_mesh->mNumVertices;
}

triangle_face c_assimp_mesh_impl::get_face(uint32_t idx) const 
{
	assert(m_ai_mesh); 
	assert(idx < m_ai_mesh->mNumFaces); 
	aiFace *face = &m_ai_mesh->mFaces[idx];
	assert(face->mNumIndices == 3);

	return triangle_face(face->mIndices[0], face->mIndices[1], face->mIndices[2]); 
}

point3f c_assimp_mesh_impl::get_vert(uint32_t idx) const 
{
	assert(m_ai_mesh);
	assert(idx < m_ai_mesh->mNumVertices);

	return aivec3_to_cmlvec3(m_ai_mesh->mVertices[idx]); 
}

uv c_assimp_mesh_impl::get_vert_uv(uint32_t vert_idx, uint32_t uv_set) const 
{
	assert(m_ai_mesh); 
	assert(has_uv(idx));
	assert(vert_idx < m_ai_mesh->mNumVertices);

	uv ret(m_ai_mesh->mTextureCoords[uv_set][vert_idx].x, m_ai_mesh->mTextureCoords[uv_set][vert_idx].y); 
	return ret;
}

vector3f c_assimp_mesh_impl::get_vert_tengent(uint32_t vert_idx) const 
{
	assert(m_ai_mesh); 
	assert(has_tengent());
	assert(vert_idx < m_ai_mesh->mNumVertices);
	
	return aivec3_to_cmlvec3(m_ai_mesh->mTangents[vert_idx]); 
}

vector3f c_assimp_mesh_impl::get_vert_normal(uint32_t vert_idx) const 
{
	assert(m_ai_mesh); 
	assert(has_normal());
	assert(vert_idx < m_ai_mesh->mNumVertices);
	
	return aivec3_to_cmlvec3(m_ai_mesh->mNormals[vert_idx]);
}

void c_assimp_mesh_impl::apply_transform(const c_transform& t)
{
	assert(m_ai_mesh); 
	
	aiMatrix4x4 aim = cmlmat4x4_to_aimat4x4(t.get_matrix()); 
	
	for (uint32_t i = 0; i < m_ai_mesh->mNumVertices; ++i)
	{
		m_ai_mesh->mVertices[i] = aim * m_ai_mesh->mVertices[i]; 
	}
	
}

//////////////////////////////////////////////////////////////////////////

c_assimp_face_impl::c_assimp_face_impl(assimp_mesh_impl_ptr& assimp_mesh, uint32_t face_idx)
	: m_mesh(assimp_mesh)
	, m_face_idx(face_idx)
{
}