#include "pch.h"
#include "camera.h"
#include "rng.h"
#include "integrator.h"
#include "stratified_sampler.h"
#include "render_target.h"
#include "filter.h"
#include "color.h"

#include "cml/cml.h"
#include <iomanip>
#include <iostream>
#include <fstream> 


using namespace std;


int res_x = 4; 
int res_y = 4; 
int spp_x = 2; 
int spp_y = 2; 

sampler_ptr g_main_sampler; 
sample_ptr g_origin_sample; 
render_target_ptr g_render_target; 
filter_ptr g_filter; 
camera_ptr g_camera;

typedef boost::scoped_array<c_spectrum> spectrum_array;

void print_pixels(ostream& os, pixels_buf_ptr& pixels, int num_pixels_x, int num_pixels_y, int prec = 2, int width = 4)
{
	std::ios::fmtflags old_flags = os.flags(); 
	os.setf(ios::left, ios::adjustfield); 

	for (int y = 0; y < num_pixels_y; ++y)
	{
		for (int x = 0; x < num_pixels_x; ++x)
		{
			float inv_weight = 1 / (pixels[y * num_pixels_x + x].weighted_sum);
			os << std::setprecision(prec) << std::setw(width) << std::setfill(' ') 
				<< '('
				<< pixels[y * num_pixels_x + x].l_rgb[0] * inv_weight << ',' 
				<< pixels[y * num_pixels_x + x].l_rgb[1] * inv_weight << ',' 
				<< pixels[y * num_pixels_x + x].l_rgb[2] * inv_weight << ')';

			os << "\t"; 

		}
		os << std::endl; 
	}
	os.setf(old_flags); 
}

int main(int argc, char **argv)
{
	//////////////////////////////////////////////////////////////////////////
	// Make the sampler
	//////////////////////////////////////////////////////////////////////////
	g_main_sampler.reset(new c_stratified_sampler(0, res_x, 0, res_y, spp_x, spp_y, true, 0.0f, 0.0f)); 
	g_origin_sample.reset(new c_sample(g_main_sampler, surface_integrator_ptr(), volume_integrator_ptr(), scene_ptr()));

	//////////////////////////////////////////////////////////////////////////
	// Make Render Target
	//////////////////////////////////////////////////////////////////////////
	g_filter = make_box_filter(0.5f, 0.5f); 
	if (g_render_target)
		g_render_target.reset();
	g_render_target = make_bitmap_render_target(res_x, res_y, g_filter); 

	//////////////////////////////////////////////////////////////////////////
	// Make Camera
	//////////////////////////////////////////////////////////////////////////
	matrix44f m;
	cml::matrix_translation(m, vector3f(0,0,0));
	c_transform world_to_cam(m); 
	c_transform cam_to_world(m);
	float wnd[4] = {-1.0f, 1.0f, -1.0f, 1.0f};
	g_camera.reset(new c_perspective_camera(cam_to_world, wnd, 0, 0, 90, g_render_target));

	
	//////////////////////////////////////////////////////////////////////////
	// Ray Tracing and Rendering
	//////////////////////////////////////////////////////////////////////////
	c_rng rng(2047); 
	c_rng rng2(1024);

	int num_pixel_samples = 0; 
	int max_samples = g_main_sampler->get_max_num_samples(); 
	samples_array_ptr samples_array = g_origin_sample->duplicate(max_samples); 
	spectrum_array ls(new c_spectrum[max_samples]); 
	while ((num_pixel_samples = g_main_sampler->get_current_pixel_samples(samples_array, rng)) > 0)
	{
		for (int j = 0; j < num_pixel_samples; ++j)
		{
			c_ray r; 
			g_camera->generate_ray(samples_array[j], &r); 
			float red = rng2.random_float(); 
			float green = rng2.random_float(); 
			float blue = rng2.random_float(); 
			ls[j] = c_spectrum(red, green, blue); 
			g_camera->get_render_target()->add_sample(samples_array[j], ls[j]);
		}
	} 

	ofstream ofs("output.txt"); 
	pixels_buf_ptr pixels = g_render_target->get_pixels(); 
	print_pixels(ofs, pixels, res_x, res_y, 2, 4); 
	
	return 0;
}