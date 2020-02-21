#pragma once
#include <vector>

#include "MemoryBlockChain.h"

struct MemoryAllocator
{
    explicit MemoryAllocator(unsigned int chunkBytesOfMemoryBlock);
    MemoryAllocator(const MemoryAllocator& other);
    ~MemoryAllocator();

    MemoryAllocator& operator= (const MemoryAllocator& other);

    unsigned int chunkBytesOfMemoryBlock;
    MemoryBlockChain * memoryBlockChain;
};

class MemoryPoll
{
public:
    const unsigned int SIZE_MULT = 131072;
    const std::vector<unsigned int> sizePool = { 64, 128, 512, 2048, 8192 }; //8M, 16M, 64M, 256M, 1G

public:
    MemoryPoll();
    virtual ~MemoryPoll();

    void* malloc(unsigned int size);
    void free(const void * const ptr);

private:
    void initMemoryPoll();
    const MemoryAllocator* selectAllocator(unsigned int size) const;

private:
    std::vector<MemoryAllocator*> allocators;
};
