#pragma once

#include <list>
#include <memory>

#include "MemoryBlock.h"

class MemoryBlockChain
{
public:
    explicit MemoryBlockChain(unsigned int sizeOfBytesEachBlock);

    void* malloc(unsigned int size);
    void  free(const void * const ptr);

    bool canFree(const void* const ptr) const;

    bool gc();
private:
    void pushNewBlock();

private:
    unsigned int sizeOfBytesEachBlock;
    std::list<std::unique_ptr<MemoryBlock>> blockChain;
};
