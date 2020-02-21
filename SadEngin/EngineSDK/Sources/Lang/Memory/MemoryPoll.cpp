#include <assert.h>
#include <algorithm>

#include "MemoryPoll.h"

MemoryAllocator::MemoryAllocator(unsigned int chunkBytesOfMemoryBlock)
    :chunkBytesOfMemoryBlock(chunkBytesOfMemoryBlock)
{
    memoryBlockChain = new MemoryBlockChain(chunkBytesOfMemoryBlock);
}

MemoryAllocator::MemoryAllocator(const MemoryAllocator & other)
{
    this->chunkBytesOfMemoryBlock = other.chunkBytesOfMemoryBlock;
    this->memoryBlockChain = other.memoryBlockChain;
}

MemoryAllocator::~MemoryAllocator()
{
    assert(memoryBlockChain);
    delete memoryBlockChain;
}

MemoryAllocator & MemoryAllocator::operator=(const MemoryAllocator & other)
{
    this->chunkBytesOfMemoryBlock = other.chunkBytesOfMemoryBlock;
    this->memoryBlockChain = other.memoryBlockChain;
    return *this;
}

MemoryPoll::MemoryPoll()
{
    initMemoryPoll();
}

MemoryPoll::~MemoryPoll()
{
    auto call_back = [](auto * ptr)
    {
        delete ptr;
    };
    std::for_each(allocators.cbegin(), allocators.cend(), call_back);
}

void * MemoryPoll::malloc(unsigned int size)
{
    auto allocator = selectAllocator(size);
    return allocator->memoryBlockChain->malloc(size);
}

void MemoryPoll::free(const void * const ptr)
{
    for (auto allocator = this->allocators.cbegin();
        allocator != this->allocators.cend();
        ++allocator)
    {
        if ((*allocator)->memoryBlockChain->canFree(ptr))
        {
            (*allocator)->memoryBlockChain->free(ptr);
        }
    }
}

void MemoryPoll::initMemoryPoll()
{
    for (auto ptr = sizePool.cbegin(); ptr != sizePool.cend(); ++ptr)
    {
        allocators.push_back(new MemoryAllocator(*ptr * SIZE_MULT));
    }
}

const MemoryAllocator* MemoryPoll::selectAllocator(
    unsigned int size
) const
{
    for (int i = 0; i < sizePool.size(); ++i)
    {
        if (sizePool.at(i) > size)
        {
            return allocators.at(i);
        }
    }
    assert(sizePool.at(sizePool.size() - 1) * SIZE_MULT >= size);
    return allocators.back();
}
