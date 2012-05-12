#pragma once

#include "prerequisites.h" 

class c_rng
{
public:
	c_rng(uint32_t _seed = 5489UL) 
	{
		mti = n+1;
		seed(_seed);
	}

	void seed(uint32_t seed) const; 
	float random_float() const; 
	unsigned long random_uint() const; 
	
private:
	static const int n = 624; 
	mutable unsigned long mt[n];	// The array for the state vector 
	mutable int mti; 
};