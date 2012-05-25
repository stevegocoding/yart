#include "memory.h" 

size_pool_map g_pools_map; 

char *yart_pool_alloc(size_t size, pool_ptr pool)
{	
	char *buf = (char*)pool->malloc(); 
	return buf; 
}

pool_ptr get_pool(size_t size)
{
	size_pool_map_it it = g_pools_map.find(size); 
	if (it == g_pools_map.end())
	{
		pool_ptr new_pool(new boost::pool<>(size)); 
		g_pools_map.insert(std::pair<size_t, pool_ptr>(size, new_pool));
		return new_pool; 
	}
	else 
		return it->second; 
}