#include "pch.h"
#include "camera.h"
#include "rng.h"
#include "integrator.h"
#include "stratified_sampler.h"
#include "render_target.h"
#include "filter.h"
#include "geometry.h"
#include "reflection.h"

#include "cml/cml.h"
#include <iomanip>
#include <time.h>

using namespace std;

void profile_bsdf_memory()
{
	diff_geom_ptr dg = diff_geom_ptr(new c_differential_geometry());

	vector3f vec; 

	clock_t time_start = clock(); 
	float elapsed = 0.0f; 

	for (int i = 0; i < 5000000; ++i)
	{
		c_bsdf *bsdf = new c_bsdf(*dg, vec); 
		delete bsdf;
	}

	elapsed = clock() - time_start; 
	time_start = clock(); 
	cout << "Default memory allocation: " << (float)elapsed << endl; 
	elapsed = 0.0f; 

	pool_ptr pool = get_pool(sizeof(c_bsdf)); 
	for (int i = 0; i < 5000000; ++i)
	{
		c_bsdf *bsdf = NEW_BSDF(c_bsdf, pool)(*dg, vec); 
		DELETE_BSDF(bsdf, pool); 
	}
	elapsed = clock() - time_start; 
	time_start = clock(); 
	cout << "Boost pool memory allocation: " << (float)elapsed << endl; 
	
}

int main(int argc, char **argv)
{
	

	return 0;
}