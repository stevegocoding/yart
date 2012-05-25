#pragma once

#include <map>
#include <boost/pool/object_pool.hpp> 
#include <boost/make_shared.hpp> 
#include "prerequisites.h" 

typedef boost::shared_ptr<boost::pool<> > pool_ptr; 
typedef std::map<size_t, pool_ptr> size_pool_map; 
typedef size_pool_map::iterator size_pool_map_it; 

extern size_pool_map g_pools_map; 

char *yart_pool_alloc(size_t size, pool_ptr pool);
pool_ptr get_pool(size_t size);

#define NEW_BSDF(type, pool) new (yart_pool_alloc(sizeof(type),pool)) type
#define DELETE_BSDF(type, pool) delete_pooled(type, pool)

template <typename T>
void delete_pooled(T *p, pool_ptr pool)
{
	assert(pool); 
	p->~T(); 
	pool->free(p); 
}


template <typename T>
class c_pool_alloc
{
public: 
	void *operator new (std::size_t size)
	{
		if (!size)
			return NULL;
		
		char *buf = yart_pool_alloc(size); 
		assert(buf); 
		if (!buf)
			return NULL; 

		return buf; 
	}
	
	void operator delete (void *p)
	{
		pool_ptr pool = get_pool(sizeof(T));
		assert(pool); 
		pool->free(p); 
	}
};