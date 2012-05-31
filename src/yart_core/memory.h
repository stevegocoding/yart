#pragma once

#include <map>
#include <boost/make_shared.hpp> 
#include "prerequisites.h" 

typedef std::map<size_t, pool_ptr> size_pool_map; 
typedef size_pool_map::iterator size_pool_map_it; 

extern size_pool_map g_pools_map; 

char *yart_pool_alloc(size_t size, pool_ptr pool);
pool_ptr get_pool(size_t size);
void free_all_pool(); 

#define NEW_BSDF(type, pool) new (yart_pool_alloc(sizeof(type),pool)) type
#define DELETE_BSDF(type, pool) delete_pooled(type, pool)

template <typename T>
void delete_pooled(T *p, pool_ptr pool)
{
	assert(pool); 
	p->~T(); 
	pool->free(p); 
} 


class c_mem_pool_group
{
public:
	c_mem_pool_group() {}
	
	
	
private:
	size_pool_map m_mem_pools_map; 
	
	
};