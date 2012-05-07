#pragma once

#include <ostream>
#include "cml/cml.h"

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

c_transform make_translate(const vector3f& trans)
{
	matrix44f mat(1, 0, 0, trans[0], 
				  0, 1, 0, trans[1],
				  0, 0, 1, trans[2], 
				  0, 0, 0,		 1);
	
	matrix44f inv_mat(1, 0, 0, -trans[0], 
					  0, 1, 0, -trans[1],
					  0, 0, 1, -trans[2], 
					  0, 0, 0,		 1);
	
	return c_transform(mat, inv_mat);
}

c_transform make_scale(float sx, float sy, float sz)
{
	matrix44f mat(sx, 0, 0, 0,
				  0, sy, 0, 0,
				  0, 0, sz, 0,
				  0, 0, 0, 1);
	matrix44f inv_mat(sx, 0, 0, 0,
					  0, sy, 0, 0,
					  0, 0, sz, 0,
					  0, 0, 0, 1);
	return c_transform(mat, inv_mat); 
}