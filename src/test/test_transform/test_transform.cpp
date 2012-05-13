#include "../../yart_core/pch.h"
#include "../../yart_core/transform.h"
#include "cml/cml.h"
#include <iomanip>

using namespace std;

void print_matrix(std::ostream& os, const cml::matrix44f& mat, int prec = 4, int width = 8)
{
	std::ios::fmtflags old_flags = os.flags(); 
	os.setf(ios::left, ios::adjustfield); 

	// row #1
	os << std::setprecision(prec) << std::setw(width) << std::setfill(' ') 
		<< mat(0, 0) << ' ' 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< mat(0, 1) << ' '
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< mat(0, 2) << ' ' 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< mat(0, 3) << endl; 

	// row #2
	os << std::setprecision(prec) << std::setw(width) << std::setfill(' ') 
		<< mat(1, 0) << ' ' 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< mat(1, 1) << ' '
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< mat(1, 2) << ' ' 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< mat(1, 3) << endl; 
	
	// row #3  
	os << std::setprecision(prec) << std::setw(width) << std::setfill(' ') 
		<< mat(2, 0) << ' ' 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< mat(2, 1) << ' '
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< mat(2, 2) << ' ' 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< mat(2, 3) << endl; 

	// row #4
	os << std::setprecision(prec) << std::setw(width) << std::setfill(' ') 
		<< mat(3, 0) << ' ' 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< mat(3, 1) << ' '
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< mat(3, 2) << ' ' 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< mat(3, 3) << endl; 

	os.setf(old_flags); 
}

void print_vector3f_col(std::ostream& os, const cml::vector3f& vec, int prec = 4, int width = 8)
{
	std::ios::fmtflags old_flags = os.flags(); 
	os.setf(ios::left, ios::adjustfield); 

	os << std::setprecision(prec) << std::setw(width) << std::setfill(' ') 
		<< vec[0] << ' ' << std::endl 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< vec[1] << ' ' << std::endl 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< vec[2] << ' ' << std::endl; 

	os.setf(old_flags); 
}

void print_vector4f_col(std::ostream& os, const cml::vector4f& vec, int prec = 4, int width = 8)
{
	std::ios::fmtflags old_flags = os.flags(); 
	os.setf(ios::left, ios::adjustfield); 

	os << std::setprecision(prec) << std::setw(width) << std::setfill(' ') 
		<< vec[0] << ' ' << std::endl 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< vec[1] << ' ' << std::endl 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< vec[2] << ' ' << std::endl 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< vec[3] << endl; 

	os.setf(old_flags); 
}
	
int main(int argc, char **argv)
{
	int res_x = 640; 
	int res_y = 480; 

	float screen_wnd[4] = {-1.3333f, 1.3333f, -1.0f, 1.0f}; 
	
	c_transform camera_to_screen = make_perspective_proj(90.0f, 1.0f, 2.0f); 
	c_transform screen_to_raster = 
		make_scale((float)(res_x), 
		(float)(res_y), 
		1.0f) * 
		make_scale(1.0f / (screen_wnd[1] - screen_wnd[0]), 1.0f / (screen_wnd[2] - screen_wnd[3]), 1.0f) * 
		make_translate(vector3f(-screen_wnd[0], -screen_wnd[3], 0)); 
	c_transform camera_to_raster = camera_to_screen * screen_to_raster;
	c_transform raster_to_camera = inverse_transform(camera_to_raster);

	print_matrix(cout, camera_to_screen.get_matrix());

	cout << endl; 
	
	print_matrix(cout, screen_to_raster.get_matrix());
	
	cout << endl; 

	print_matrix(cout, camera_to_raster.get_matrix());

	cout << endl; 
	
	vector3f p(0.0f, 0.0f, 1.0f); 
	vector3f proj_p = camera_to_raster.transform_pt(p); 
	print_vector3f_col(cout, proj_p); 

	cout << endl; 

	vector3f p2(320.0f, 240.0f, 0.0f); 
	vector3f proj_p2 = raster_to_camera.transform_pt(p2); 
	print_vector3f_col(cout, proj_p2); 
	
	return 0;
}