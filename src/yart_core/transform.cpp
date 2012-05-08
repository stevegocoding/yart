#include "transform.h"

c_transform c_transform::operator*(const c_transform& tr) const
{
	matrix44f m = get_matrix() * tr.get_matrix();
	matrix44f inv_m = tr.get_inv_matrix() * get_inv_matrix(); 

	return c_transform(m, inv_m);
}

c_transform make_translate(const vector3f& trans)
{
	matrix44f m, inv_m;
	matrix_translation(m, trans); 
	matrix_translation(inv_m, -trans);
	return c_transform(m, inv_m);
}

c_transform make_scale(float sx, float sy, float sz)
{
	matrix44f m, inv_m;
	matrix_scale(m, sx, sy, sz);
	matrix_scale(inv_m, 1.0f/sx, 1.0f/sy, 1.0f/sz); 
	return c_transform(m, inv_m); 
}

c_transform inverse_transform(const c_transform& t)
{
	return c_transform(t.get_inv_matrix(), t.get_matrix());
}

c_transform make_perspective_proj(float fov, float near, float far)
{
	
}