
#include <Windows.h>
#include <iostream>
#include "memory.h"
#include "reflection.h"
#include "geometry.h"

#include "memory_arena.h"

using namespace std;

c_fixed_size_mem_pool *bsdf_pool = NULL; 
boost::pool<> raw_pool(sizeof(c_bsdf)); 
MemoryArena mem_arena; 

int main(int argc, char **argv)
{
	int totoal_work = 800000;
	int work = 0; 
	bsdf_pool = new c_fixed_size_mem_pool(sizeof(c_bsdf));
	c_differential_geometry dg; 
	vector3f n; 

	int num_isects = 1000;

	LARGE_INTEGER freq; 
	LARGE_INTEGER start;
	LARGE_INTEGER end; 
	LARGE_INTEGER elapsed;
	QueryPerformanceFrequency(&freq);
	
	QueryPerformanceCounter(&start);
	cout << "Allocation with pool: " << endl;

	/*
	while (work++ < totoal_work)
	{
		char *buf = bsdf_pool->alloc();
		c_bsdf *bsdf = new (buf) c_bsdf(dg, n);
		
		// Do something with the bsdf... 

		bsdf->~c_bsdf(); 
		bsdf_pool->free_all();
		
	}
	*/ 

	/*
	while (work++ < totoal_work)
	{
		char *buf = (char*)raw_pool.malloc(); 
		c_bsdf *bsdf = new (buf) c_bsdf(dg, n);

		// Do something with the bsdf... 

		bsdf->~c_bsdf(); 
		raw_pool.free(buf);
	}
	*/

	/*
	while (work++ < totoal_work)
	{
		pool_ptr pool = get_pool(sizeof(c_bsdf)); 
		c_bsdf *bsdf = NEW_BSDF(c_bsdf, pool)(dg, n); 

		// Do something with the bsdf... 

		bsdf->~c_bsdf(); 
		// raw_pool.free(buf);
	}
	*/
	while (work++ < totoal_work)
	{
		char *buf = (char*)mem_arena.Alloc(sizeof(c_bsdf)); 
		c_bsdf *bsdf = new (buf) c_bsdf(dg, n);

		// Do something with the bsdf... 

		bsdf->~c_bsdf(); 
		mem_arena.FreeAll();
	}

	QueryPerformanceCounter(&end);
	elapsed.QuadPart = end.QuadPart - start.QuadPart; 
	double time = (double)((double)elapsed.QuadPart / (double)freq.QuadPart); 	

	cout << "Time: " << time << endl;
	
	work = 0; 


	QueryPerformanceCounter(&start);
	cout << "Allocation with new: " << endl;

	while (work++ < totoal_work)
	{
		c_bsdf *bsdf = new c_bsdf(dg, n);

		// Do something with the bsdf... 

		delete bsdf; 
	}

	QueryPerformanceCounter(&end);
	elapsed.QuadPart = end.QuadPart - start.QuadPart; 
	time = (double)((double)elapsed.QuadPart / (double)freq.QuadPart); 	

	cout << "Time: " << time << endl;

	raw_pool.release_memory(); 

	delete bsdf_pool; 
	bsdf_pool = NULL; 

	return 0; 
}