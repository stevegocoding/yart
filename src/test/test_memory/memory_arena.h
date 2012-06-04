#pragma once
#include "prerequisites.h"

using namespace std;

#define L1_CACHE_LINE_SIZE 64; 

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