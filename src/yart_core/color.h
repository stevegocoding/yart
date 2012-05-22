#pragma once 

#include <assert.h>
#include "prerequisites.h"

inline void xyz_to_rgb(const float xyz[3], float rgb[3]) {
	rgb[0] =  3.240479f*xyz[0] - 1.537150f*xyz[1] - 0.498535f*xyz[2];
	rgb[1] = -0.969256f*xyz[0] + 1.875991f*xyz[1] + 0.041556f*xyz[2];
	rgb[2] =  0.055648f*xyz[0] - 0.204043f*xyz[1] + 1.057311f*xyz[2];
}

inline void rgb_to_xyz(const float rgb[3], float xyz[3])
{
	xyz[0] = 0.412453f*rgb[0] + 0.357580f*rgb[1] + 0.180423f*rgb[2];
	xyz[1] = 0.212671f*rgb[0] + 0.715160f*rgb[1] + 0.072169f*rgb[2];
	xyz[2] = 0.019334f*rgb[0] + 0.119193f*rgb[1] + 0.950227f*rgb[2];
}

namespace 
{
	enum e_rgb_spectrum_component
	{
		r = 0,
		g = 1, 
		b = 2
	}; 
}

template <int num_samples> 
class c_coeff_spectrum
{
public:
	explicit c_coeff_spectrum(float v = 0.0f)
	{
		for (int i = 0; i < num_samples; ++i)
		{
			c[i] = v; 
		}
	}

	c_coeff_spectrum(const c_coeff_spectrum& other)
	{
		assert(!other.has_nan()); 
		for (int i = 0; i < num_samples; ++i)
		{
			c[i] = other.c[i];
		}
	}

	bool has_nan() const 
	{
		for (int i = 0; i < num_samples; ++i)
		{
			if (_isnan(c[i]))
				return true; 
		}
		return false; 
	} 
	
	c_coeff_spectrum& operator += (const c_coeff_spectrum& other)
	{
		assert(!other.has_nan());
		for (int i = 0; i < num_samples; ++i)
		{
			c[i] += other.c[i]; 
		}
		return *this;
	}
	
	c_coeff_spectrum operator + (const c_coeff_spectrum& other) const 
	{
		assert(!other.has_nan());
		c_coeff_spectrum ret = *this;
		for (int i = 0; i < num_samples; ++i)
		{
			ret.c[i] += other.c[i]; 
		}
		return ret; 
	}

	c_coeff_spectrum operator - (const c_coeff_spectrum& other) const
	{
		assert(!other.has_nan());
		c_coeff_spectrum ret = *this;
		for (int i = 0; i < num_samples; ++i)
		{
			ret.c[i] -= other.c[i]; 
		}
		return ret; 
	}

	c_coeff_spectrum operator / (const c_coeff_spectrum& other) const
	{
		assert(!other.has_nan());
		c_coeff_spectrum ret = *this;
		for (int i = 0; i < num_samples; ++i)
		{
			ret.c[i] /= other.c[i]; 
		}
		return ret; 
	}

	c_coeff_spectrum operator * (const c_coeff_spectrum& other) const
	{
		assert(!other.has_nan());
		c_coeff_spectrum ret = *this;
		for (int i = 0; i < num_samples; ++i)
		{
			ret.c[i] *= other.c[i]; 
		}
		return ret; 
	}

	c_coeff_spectrum operator *= (const c_coeff_spectrum& other) 
	{
		assert(!other.has_nan());
		for (int i = 0; i < num_samples; ++i)
		{
			c[i] *= other.c[i]; 
		}
		return *this;
	}

	c_coeff_spectrum operator * (float scalar) const
	{
		assert(!has_nan() && !_isnan(scalar));
		c_coeff_spectrum ret = *this;
		for (int i = 0; i < num_samples; ++i)
		{
			ret.c[i] *= scalar; 
		}
		return ret; 
	}

	float operator [] (uint32_t idx) const 
	{
		assert(!has_nan() && (idx < num_samples));
		return c[idx]; 
	}
	
	friend inline c_coeff_spectrum operator * (float scalar, const c_coeff_spectrum& s) 
	{
		assert(!_isnan(scalar) && !s.has_nan());
		return s * a; 
	}

protected: 
	float c[num_samples];
};

class c_rgb_spectrum : public c_coeff_spectrum<3> 
{
	using c_coeff_spectrum<3>::c; 
	typedef c_coeff_spectrum<3> super;
	
public:
	explicit c_rgb_spectrum(float v = 0.0f) 
		: super(v)
	{
	}
	
	c_rgb_spectrum(const c_coeff_spectrum<3>& other)
		: super(other) 
	{}

	c_rgb_spectrum(float _r, float _g, float _b)
	{
		c[0] = _r; 
		c[1] = _g; 
		c[2] = _b;
	}
	
	bool has_nan() const 
	{
		return _isnan(c[0]) || _isnan(c[1]) || _isnan(c[2]); 
	}
	
	void to_xyz(float xyz[3]) const 
	{
		rgb_to_xyz(c, xyz); 
	}

private:

};