#pragma once

class c_film
{
public:
	c_film(int res_x, int res_y)
		: m_resolution_x(res_x)
		, m_resolution_y(res_y)
	{

	}
	
	int res_x() const { return m_resolution_x; }
	int res_y() const { return m_resolution_y; }
	
private:
	int m_resolution_x, m_resolution_y;
};

