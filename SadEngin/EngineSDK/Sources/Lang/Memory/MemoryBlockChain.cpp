#include <memory>
#include <assert.h>

#include "MemoryBlockChain.h"

MemoryBlockChain::MemoryBlockChain(unsigned int sizeOfBytesEachBlock)
    :sizeOfBytesEachBlock(sizeOfBytesEachBlock)
{
    pushNewBlock();
}

void * MemoryBlockChain::malloc(unsigned int size)
{
    assert(size > 0 && size <= sizeOfBytesEachBlock);
    void* ptr = nullptr;
    for (auto block = blockChain.cbegin(); block != blockChain.cend(); ++block)
    {
        ptr = (*block)->malloc(size);
        if (ptr != nullptr)
        {
            return ptr;
        }
    }

    pushNewBlock();
    ptr = (*(blockChain.back())).malloc(size);
    assert(ptr);
    return ptr;
}

void MemoryBlockChain::free(const void * const ptr)
{
    static unsigned int _freeCount = 0;
    for (auto block = blockChain.cbegin(); block != blockChain.cend(); ++block)
    {
        if ((*block)->canFree(ptr))
        {
            (*block)->free(ptr);
            ++_freeCount;
            break;
        }
    }
    if (_freeCount > 1000)
    {
        _freeCount = 0;
        gc();
    }
}

bool MemoryBlockChain::canFree(const void * const ptr) const
{
    for (auto block = blockChain.cbegin(); block != blockChain.cend(); ++block)
    {
        if ((*block)->canFree(ptr))
        {
            return true;
        }
    }
    return false;
}

bool MemoryBlockChain::gc()
{
    size_t oldSize = blockChain.size();
    struct RemovePred
    {
        inline bool operator() (const std::unique_ptr<MemoryBlock>& ptr) const
        {
            return !ptr->hasAllocedPointer();
        }
    };
    RemovePred rp;
    blockChain.remove_if(rp);

    return blockChain.size() < oldSize;
}

void MemoryBlockChain::pushNewBlock()
{
    blockChain.push_back(
        std::make_unique<MemoryBlock>(sizeOfBytesEachBlock)
    );
}
