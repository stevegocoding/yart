#pragma once

#include <ostream>
#include <assert.h>
#include "cml/cml.h"

#include "geometry.h"

using namespace cml;

class c_transform
{
public:
	c_transform() {}

	c_transform(const matrix44f& mat, const matrix44f& inv_mat)
		: m_mat(mat)
		, m_inv_mat(inv_mat)
	{}

	explicit c_transform(matrix44f& mat)
		: m_mat(mat)
		, m_inv_mat(mat.inverse())
	{}

	explicit c_transform(const float mat[4][4])
	{
		matrix44f temp = matrix44f(
			mat[0][0], mat[0][1], mat[0][2], mat[0][3],
			mat[1][0], mat[1][1], mat[1][2], mat[1][3],
			mat[2][0], mat[2][1], mat[2][2], mat[2][3],
			mat[3][0], mat[3][1], mat[3][2], mat[3][3]); 
		
		m_mat = temp; 
		m_inv_mat = temp.inverse();
	}
	
	// ---------------------------------------------------------------------
	/*
		Operators
	*/ 
	// ---------------------------------------------------------------------
	c_transform operator*(const c_transform& tr) const;
	inline vector3f transform_pt(const vector3f& pt) const;
	inline vector3f transform_vec3(const vector3f& vec) const; 

    c_ray operator*(const c_ray& r) const;

    c_ray transform_ray(const c_ray& r) const; 
	
	const matrix44f& get_matrix() const { return m_mat; }
	const matrix44f& get_inv_matrix() const { return m_inv_mat; }

	void print(std::ostream& os)
	{
		os << "Transform:" << std::endl;
	}
	
private:
	matrix44f m_mat; 
	matrix44f m_inv_mat; 
};

inline vector3f c_transform::transform_pt(const vector3f& pt) const 
{
	vector4f _p = vector4f(pt, 1.0f); 
	vector4f new_p = transform_vector_4D(m_mat, _p);
	float w = new_p[3]; 
	assert(w != 0); 
	if (w == 1.)
		return vector3f(new_p[0], new_p[1], new_p[2]); 
	else 
		return vector3f(new_p[0]/w, new_p[1]/w, new_p[2]/w);
}

inline vector3f c_transform::transform_vec3(const vector3f& vec) const 
{
	vector3f v = transform_vector(m_mat, vec); 
	return v; 
}

c_transform make_translate(const vector3f& trans);
c_transform make_scale(float sx, float sy, float sz);
c_transform inverse_transform(const c_transform& t); 
c_transform make_perspective_proj(float fov, float near, float far); 
