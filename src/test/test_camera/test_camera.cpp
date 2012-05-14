#include "../../yart_core/pch.h"
#include "../../yart_core/camera.h"
#include "../../yart_core/rng.h"
#include "../../yart_core/integrator.h"
#include "../../yart_core/stratified_sampler.h"
#include "../../yart_core/film.h"

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

void print_vector3f_row(std::ostream& os, const cml::vector3f& vec, int prec = 4, int width = 8)
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

void print_ray(std::ostream& os, c_ray& ray, int prec = 4, int width = 8)
{
	std::ios::fmtflags old_flags = os.flags(); 
	os.setf(ios::left, ios::adjustfield); 

	os << "Ray: " << '[' << std::setprecision(prec) << std::setw(width) << std::setfill(' ') << ray.o[0] << ' ' 
						 << std::setprecision(prec) << std::setw(width) << std::setfill(' ') << ray.o[1] << ' '
						 << std::setprecision(prec) << std::setw(width) << std::setfill(' ') << ray.o[2] << ']' << "   "
				  << '[' << std::setprecision(prec) << std::setw(width) << std::setfill(' ') << ray.d[0] << ' '
						 << std::setprecision(prec) << std::setw(width) << std::setfill(' ') << ray.d[1] << ' '
						 << std::setprecision(prec) << std::setw(width) << std::setfill(' ') << ray.d[2] << ']' << endl;

	os.setf(old_flags); 
}

void print_img_sample(std::ostream& os, c_sample& s, int prec = 4, int width = 8)
{
	std::ios::fmtflags old_flags = os.flags(); 
	os.setf(ios::left, ios::adjustfield); 

	os << "Img Sample: " << '[' << std::setprecision(prec) << std::setw(width) << std::setfill(' ') << s.image_x << ' ' 
								<< std::setprecision(prec) << std::setw(width) << std::setfill(' ') << s.image_y << ']'
								<< endl;

	os.setf(old_flags); 
	
}
	
int main(int argc, char **argv)
{
	uint32_t res_x = 2; 
	uint32_t res_y = 2; 
	uint32_t sppx = 3;
	uint32_t sppy = 2; 
	c_rng rng(2047); 
	std::vector<c_sample> samples_vec;
	std::vector<c_ray> rays_vec;
	samples_vec.reserve(res_x * res_y * sppx * sppy);
	rays_vec.reserve(res_x * res_y * sppx * sppy);

	// Create the sampler
	stratified_sampler_ptr sampler = boost::make_shared<c_stratified_sampler>(0, res_x, 0, res_y, sppx, sppy, true, 0.0f, 0.0f);
	sample_ptr orig_sample = sample_ptr(new c_sample(sampler, surface_integrator_ptr(), volume_integrator_ptr(), scene_ptr()));
		
	// Create the camera 
	film_ptr film = film_ptr(new c_film(res_x, res_y));
	matrix44f m = cml::identity_4x4();
	c_transform cam_to_world(m); 
	float wnd[4] = {-1.0f, 1.0f, -1.0f, 1.0f};
	perspective_camera_ptr cam = perspective_camera_ptr(new c_perspective_camera(cam_to_world, wnd, 0, 0, 90, film));
	
	int sc = 0; 
	samples_array_ptr samples_array = orig_sample->duplicate(sppx*sppy); 
	while ((sc = sampler->get_current_pixel_samples(samples_array, rng)) > 0)
	{
		for (uint32_t j = 0; j < sppx * sppy; ++j)
		{
			c_ray r; 
			cam->generate_ray(samples_array[j], &r); 
			samples_vec.push_back(samples_array[j]);
			rays_vec.push_back(r); 
			//print_img_sample(cout, samples_array[j]); 
			print_ray(cout, r); 
		}
	}

	

	return 0;
}