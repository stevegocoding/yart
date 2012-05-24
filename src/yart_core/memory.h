#pragma once

#include <map>
#include <boost/pool/object_pool.hpp> 
#include <boost/make_shared.hpp> 
#include "prerequisites.h" 

typedef boost::shared_ptr<boost::pool<> > pool_ptr; 
typedef std::map<size_t, pool_ptr> size_pool_map; 
typedef size_pool_map::iterator size_pool_map_it; 

extern size_pool_map g_pools_map; 

template <typename T>
T *yart_alloc_pool()
{
    size_pool_map_it it = g_pools_map.find(sizeof(T)); 
    if (it != g_pools_map.end())
    {
        pool_ptr pool = it->second;
        char *buf = (char*)pool->malloc(); 
        T *obj = new (buf) T(); 
        return obj; 
    }

    // register a pool with a new size
    size_t new_size = sizeof(T);
    pool_ptr new_pool = make_shared<boost::pool<> >(new_size);
    g_pools_map.insert(std::pair<size_t, pool_ptr>(new_size, new_pool)); 

    char *buf = (char*)new_pool->malloc(); 
    T *obj = new (buf) T(); 
    return obj; 
}
