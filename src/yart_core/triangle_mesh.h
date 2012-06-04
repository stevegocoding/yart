#pragma once 

#include "prerequisites.h"
#include "transform.h"
#include "shape.h"

typedef vector2f uv;
typedef vector3i tri_indices;
typedef std::vector<point3f> vertices_array; 
typedef std::vector<vector3f> normals_array; 
typedef std::vector<vector3f> tangents_array;
typedef std::vector<uv> uvs_array; 
typedef std::vector<tri_indices> face_indices_array;

class c_triangle_mesh
{
public: 
	c_triangle_mesh(const vertices_array& verts, 
		const normals_array& normals, 
		const tangents_array& tangents,
		const uvs_array& uvs,
		const face_indices_array& indices); 

	uint32_t get_num_verts() const 
	{ 
		return m_num_verts;
	}

	uint32_t get_num_faces() const
	{
		return m_num_faces; 
	}

	bool has_normal() const 
	{
		return m_has_normal; 
	}

	bool has_tangent() const
	{
		return m_has_tangent; 
	}

	bool has_uvs() const 
	{
		return m_has_uv;
	}

	const c_triangle_face* get_triangle_face(uint32_t face_idx) const 
	{
		assert(face_idx < m_num_faces);
		return &m_triangles[face_idx];
	}

private: 
	vertices_array m_verts;
	normals_array m_normals;
	tangents_array m_tangents; 
	uvs_array m_uvs; 
	face_indices_array m_faces_indices; 

	std::vector<c_triangle_face> m_triangles;

	uint32_t m_num_verts;
	uint32_t m_num_faces; 
	
	bool m_has_normal;
	bool m_has_tangent; 
	bool m_has_uv;

	friend class c_triangle_face; 
};

//////////////////////////////////////////////////////////////////////////

class c_triangle_face : public c_shape
{
public:
	c_triangle_face(c_triangle_mesh *mesh, int face_idx)
		: m_mesh(mesh)
	{
		m_tri_face = (int*)&m_mesh->m_faces_indices[face_idx];
	}

	virtual void get_shading_geometry(const c_transform& o2w, 
		const c_differential_geometry& geom_dg, 
		PARAM_OUT c_differential_geometry *shading_dg) const; 

	virtual bool intersects(const c_ray& ray, 
		PARAM_OUT float *t_hit, 
		PARAM_OUT float *ray_epsilon, 
		PARAM_OUT c_differential_geometry *geom_dg) const; 
	
private:
	void get_uv(float uv[3][2]) const; 

	c_triangle_mesh *m_mesh;
	int *m_tri_face;
};

