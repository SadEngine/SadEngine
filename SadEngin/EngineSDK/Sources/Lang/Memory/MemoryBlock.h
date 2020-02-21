#pragma once

#include <set>

struct MemoryAllocatedRecord
{
    const void * ptr;
    unsigned int offset;
    unsigned int length;

    explicit MemoryAllocatedRecord(
        const void * const ptr,
        unsigned int offset = 0,
        unsigned int length = 0);

    inline bool operator< (const MemoryAllocatedRecord& other) const
    {
        return this->ptr < other.ptr;
    }
};

class MemoryBlock
{
public:
    explicit MemoryBlock(const unsigned int size);
    ~MemoryBlock();

public:
    void * malloc(unsigned int size);
    void free(const void * const ptr);

    inline double memoryUsageRatio() const
    {
        return allocatedBytes * 1.0 / chunkTotalBytes;
    }

    inline bool canFree(const void * const ptr) const
    {
        return  nullptr != this->memoryChunk &&
            ptr >= this->memoryChunk &&
            ptr < (this->memoryChunk + this->chunkTotalBytes);
    }

    inline bool hasAllocedPointer() const
    {
        return allocatedBytes != 0;
    }

private:
    void createMemoryChunk();

    bool updateCurrentOffsetToValidOffset(unsigned int size);

private:
    char* memoryChunk;
    unsigned int currentOffset;
    unsigned int chunkTotalBytes;
    unsigned int allocatedBytes;

    std::set<const MemoryAllocatedRecord *> allocatedRecord;
};
