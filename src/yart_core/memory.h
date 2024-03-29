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


//////////////////////////////////////////////////////////////////////////

class c_fixed_size_mem_pool
{
public:
	c_fixed_size_mem_pool(size_t size)
		: m_chunk_size(size)
	{
		m_pool.reset(new boost::pool<>(size));
	}

	char* alloc() 
	{
		char *chunk = (char*)m_pool->malloc();
		m_chunks.push_back(chunk); 
		return chunk; 
	}

	void free_all()
	{
		std::vector<char*>::iterator it; 
		for (it = m_chunks.begin(); it != m_chunks.end(); ++it)
		{
			 m_pool->free(*it);
		}
	}

private:
	pool_ptr m_pool;
	size_t m_chunk_size;
	
	std::vector<char*> m_chunks;
};

//////////////////////////////////////////////////////////////////////////

#define L1_CACHE_LINE_SIZE 64

void *AllocAligned(size_t size);
template <typename T> T *AllocAligned(uint32_t count) {
	return (T *)AllocAligned(count * sizeof(T));
}

void FreeAligned(void *);
class MemoryArena {
public:
	// MemoryArena Public Methods
	MemoryArena(uint32_t bs = 32768) {
		blockSize = bs;
		curBlockPos = 0;
		currentBlock = AllocAligned<char>(blockSize);
	}
	~MemoryArena() {
		FreeAligned(currentBlock);
		for (uint32_t i = 0; i < usedBlocks.size(); ++i)
			FreeAligned(usedBlocks[i]);
		for (uint32_t i = 0; i < availableBlocks.size(); ++i)
			FreeAligned(availableBlocks[i]);
	}
	void *Alloc(uint32_t sz) {
		// Round up _sz_ to minimum machine alignment
		sz = ((sz + 15) & (~15));
		if (curBlockPos + sz > blockSize) {
			// Get new block of memory for _MemoryArena_
			usedBlocks.push_back(currentBlock);
			if (availableBlocks.size() && sz <= blockSize) {
				currentBlock = availableBlocks.back();
				availableBlocks.pop_back();
			}
			else
				currentBlock = AllocAligned<char>(max(sz, blockSize));
			curBlockPos = 0;
		}
		void *ret = currentBlock + curBlockPos;
		curBlockPos += sz;
		return ret;
	}
	template<typename T> T *Alloc(uint32_t count = 1) {
		T *ret = (T *)Alloc(count * sizeof(T));
		for (uint32_t i = 0; i < count; ++i)
			new (&ret[i]) T();
		return ret;
	}
	void FreeAll() {
		curBlockPos = 0;
		while (usedBlocks.size()) {
#ifndef NDEBUG
			memset(usedBlocks.back(), 0xfa, blockSize);
#endif
			availableBlocks.push_back(usedBlocks.back());
			usedBlocks.pop_back();
		}
	}
private:
	// MemoryArena Private Data
	uint32_t curBlockPos, blockSize;
	char *currentBlock;
	std::vector<char *> usedBlocks, availableBlocks;
};