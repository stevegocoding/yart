#include "render_target.h"
#include "color.h"
#include "sampler.h"
#include "filter.h"

static const int FILTER_KERNEL_SIZE = 16; 
c_bitmap_render_target::c_bitmap_render_target(int res_x, int res_y, filter_ptr filter, const float window[4])
	: super(res_x, res_y)
	, m_filter(filter)
{	
	memcpy_s(m_window, 4*sizeof(float), window, 4*sizeof(float));
	
	// Compute image extent with the window 
	m_x_pixel_start = (int)std::ceilf(m_resolution_x * m_window[0]); 
	m_x_pixel_count = max(1, (int)ceilf(m_resolution_x * m_window[1] - m_x_pixel_start));
	m_y_pixel_start = (int)std::ceilf(m_resolution_y * m_window[2]);
	m_y_pixel_count = max(1, (int)ceilf(m_resolution_y * m_window[3] - m_y_pixel_start));
	
	// Allocate the memory for pixels 
	m_pixels_buf = pixels_buf_ptr(new c_render_pixel[m_x_pixel_count * m_y_pixel_count]);

	// Pre-compute the filter weight table
	m_filter_table = filter_table_ptr(new float[FILTER_KERNEL_SIZE * FILTER_KERNEL_SIZE]);
	float *fp = &m_filter_table[0]; 
	for (int y = 0; y < FILTER_KERNEL_SIZE; ++y)
	{
		float fy = ((float)y + 0.5f) * m_filter->y_width / FILTER_KERNEL_SIZE; 
		for (int x = 0; x < FILTER_KERNEL_SIZE; ++x)
		{
			float fx = ((float)x + 0.5f) * m_filter->x_width / FILTER_KERNEL_SIZE;
			*fp++ = m_filter->evaluate(fx, fy);
		}
	} 

	
}

void c_bitmap_render_target::add_sample(const c_camera_sample& sample, const c_spectrum& radiance)
{
	// Convert the continuous image (x, y) to discrete image (x, y)
	float d_img_x = sample.image_x - 0.5f; 
	float d_img_y = sample.image_y - 0.5f; 
  
	int x0 = (int)std::ceilf(d_img_x - m_filter->x_width); 
	int x1 = (int)std::floorf(d_img_x + m_filter->x_width); 
	int y0 = (int)std::ceilf(d_img_y - m_filter->y_width); 
	int y1 = (int)std::floorf(d_img_y + m_filter->y_width); 

	x0 = max(x0, m_x_pixel_start); 
	x1 = min(x1, m_x_pixel_start + m_x_pixel_count - 1);
	y0 = max(y0, m_y_pixel_start); 
	y1 = min(y1, m_y_pixel_start + m_y_pixel_count - 1); 

	if ( (x1 - x0) < 0 ||  (y1 - y0) < 0 )
	{
		assert(false && "Sample is out of image extent"); 
		return; 
	}	
	
	// Precompute filter table offsets 

	//int *ifx = (int*)_alloca(sizeof(int) * (x1-x0+1)); 
	//int *ify = (int*)_alloca(sizeof(int) * (y1-y0+1));  

	int *ifx = new int[x1-x0+1];
	int *ify = new int[y1-y0+1];

	for (int x = x0; x <= x1; ++x)
	{
		float fx = fabsf((x - d_img_x) * m_filter->inv_x_width * FILTER_KERNEL_SIZE);
		ifx[x-x0] = min((int)std::floorf(fx), FILTER_KERNEL_SIZE-1);
	}

	for (int y = y0; y <= y1; ++y)
	{
		float fy = fabsf((y - d_img_y) * m_filter->inv_y_width * FILTER_KERNEL_SIZE);
		ify[y-y0] = min((int)std::floorf(fy), FILTER_KERNEL_SIZE-1);
	}
 
	for (int y = y0; y <= y1; ++y)
	{
		for (int x = x0; x <= x1; ++x)
		{
			// Evaluate filter value at $(x,y)$ pixel
			int offset = ify[y-y0] * FILTER_KERNEL_SIZE + ifx[x-x0];
			float filter_weight = m_filter_table[offset];
			
			// Update pixel values with filtered sample contribution
			int pixel_idx = m_x_pixel_count * (y - m_y_pixel_start) + (x - m_x_pixel_start); 
			c_render_pixel& pixel = m_pixels_buf[pixel_idx];
			pixel.l_rgb[0] += filter_weight * radiance[r];
			pixel.l_rgb[1] += filter_weight * radiance[g];
			pixel.l_rgb[2] += filter_weight * radiance[b];
			pixel.weighted_sum += filter_weight;
		}
	}

 	delete[] ifx; 
	delete[] ify; 
}

void c_bitmap_render_target::get_sample_extent(int *x_start, int *x_end, int *y_start, int *y_end) const 
{
	*x_start = (int)std::floorf(m_x_pixel_start + 0.5f - m_filter->x_width);
	*x_end = (int)std::floorf(m_x_pixel_start + 0.5f + m_x_pixel_count + m_filter->x_width);

	*y_start = (int)std::floorf(m_y_pixel_start + 0.5f - m_filter->y_width);
	*y_end = (int)std::floorf(m_y_pixel_start + 0.5f + m_y_pixel_count + m_filter->y_width);	
}

void c_bitmap_render_target::get_pixel_extent(int *x_start, int *x_end, int *y_start, int *y_end) const 
{
	*x_start = m_x_pixel_start; 
	*x_end = m_x_pixel_start + m_x_pixel_count; 
	*y_start = m_y_pixel_start; 
	*y_end = m_y_pixel_start + m_y_pixel_count; 
}

//////////////////////////////////////////////////////////////////////////

render_target_ptr make_bitmap_render_target(int res_x, int res_y, filter_ptr filter)
{
	float wnd[4] = {0.0, 1.0f, 0.0f, 1.0f}; 
	render_target_ptr ret = render_target_ptr(new c_bitmap_render_target(res_x, res_y, filter, wnd)); 
	return ret; 
}
