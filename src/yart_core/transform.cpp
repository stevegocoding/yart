#include "transform.h"

c_transform c_transform::operator*(const c_transform& tr) const
{
	matrix44f m = get_matrix() * tr.get_matrix();
	matrix44f inv_m = tr.get_inv_matrix() * get_inv_matrix(); 

	return c_transform(m, inv_m);
}

c_ray c_transform::operator*(const c_ray& r) const 
{
    return transform_ray(r); 
}

c_ray c_transform::transform_ray(const c_ray& r) const 
{
    c_ray ret = r; 
    ret.o = transform_pt(r.o);
    ret.d = transform_vec3(r.d); 
	
	ret.d.normalize();
	
    return ret; 
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

c_transform make_rotate_x(float deg)
{
	matrix44f m, inv_m; 
	matrix_rotate_about_local_x(m, cml::rad(deg)); 
	matrix_rotate_about_local_x(inv_m, cml::rad(-deg)); 
	return c_transform(m, inv_m); 
}

c_transform make_rotate_y(float deg)
{
	matrix44f m, inv_m; 
	matrix_rotate_about_world_y(m, cml::rad(deg)); 
	matrix_rotate_about_world_y(inv_m, cml::rad(-deg)); 
	return c_transform(m, inv_m); 
}

c_transform make_rotate_z(float deg)
{
	matrix44f m, inv_m; 
	matrix_rotate_about_local_z(m, cml::rad(deg)); 
	matrix_rotate_about_local_z(inv_m, cml::rad(-deg)); 
	return c_transform(m, inv_m); 
}


c_transform inverse_transform(const c_transform& t)
{
	return c_transform(t.get_inv_matrix(), t.get_matrix());
}

c_transform make_perspective_proj(float fov, float near, float far)
{ 
	matrix44f m 
		= matrix44f(
			1, 0, 0, 0,
			0, 1, 0, 0, 
			0, 0, far / (far - near), -far * near / (far - near),
			0, 0, 1, 0); 

	float inv_tan_angle = 1.0f / tanf(cml::rad(fov)/2);
	return  make_scale(inv_tan_angle, inv_tan_angle, 1) * c_transform(m);
}

c_transform make_look_at_lh(const point3f& pos, const point3f& look, const vector3f& up)
{
	float m[4][4];
	
	// Initialize fourth column of viewing matrix
	m[0][3] = pos[x];
	m[1][3] = pos[y];
	m[2][3] = pos[z];
	m[3][3] = 1;

	// Initialize first three columns of viewing matrix
	vector3f dir = normalize(look - pos); 
	vector3f left = normalize(cross(normalize(up),dir)); 
	vector3f new_up = cross(dir, left); 

	m[0][0] = left[x]; 
	m[1][0] = left[y]; 
	m[2][0] = left[z]; 
	m[3][0] = 0.0f; 
	
	m[0][1] = new_up[x];
	m[1][1] = new_up[y]; 
	m[2][1] = new_up[z]; 
	m[3][1] = 0.0f; 

	m[0][2] = dir[x]; 
	m[1][2] = dir[y];
	m[2][2] = dir[z]; 
	m[3][2] = 0.0f; 
	
	matrix44f cam_to_world(m);
	matrix44f world_to_cam = inverse(cam_to_world);
	
	return c_transform(world_to_cam, cam_to_world);
	
}

void build_coord_system(const vector3f& v1, PARAM_OUT vector3f* v2, PARAM_OUT vector3f *v3)
{
	if (fabsf(v1[x]) > fabsf(v1[y]))
	{
		float inv_len = 1.0f / sqrtf(v1[x] * v1[x] + v1[z] * v1[z]); 
		*v2 = vector3f(-v1[z] * inv_len, 0.0f, v1[x] * inv_len);		
	}
	else 
	{
		float inv_len = 1.0f / sqrtf(v1[y] * v1[y] + v1[z] * v1[z]);
		*v2 = vector3f(0.0f, v1[z] * inv_len, -v1[y] * inv_len); 
	}
	
	*v3 = cross(v1, *v2);
}