#include "transform.h"


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