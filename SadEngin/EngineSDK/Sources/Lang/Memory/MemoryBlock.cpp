#include <assert.h>
#include <algorithm>

#include "MemoryBlock.h"

MemoryAllocatedRecord::MemoryAllocatedRecord(
    const void* const ptr,
    unsigned int offset,
    unsigned int length
)
    : ptr(ptr), offset(offset), length(length)
{
    assert(ptr);
}

MemoryBlock::MemoryBlock(const unsigned int size)
    : currentOffset(0), chunkTotalBytes(size), allocatedBytes(0)
{
    memoryChunk = nullptr;
}

void MemoryBlock::createMemoryChunk()
{
    assert(chunkTotalBytes > 0);
    assert(nullptr == memoryChunk);
    memoryChunk = reinterpret_cast<char*>(::malloc(chunkTotalBytes));
    assert(memoryChunk);
}

MemoryBlock::~MemoryBlock()
{
    if (memoryChunk)
    {
        ::free(memoryChunk);
    }
}

void* MemoryBlock::malloc(unsigned int size)
{
    size += sizeof(MemoryAllocatedRecord);

    assert(size > 0 && size <= chunkTotalBytes);
    if (nullptr == memoryChunk)
    {
        createMemoryChunk();
    }
    if (!updateCurrentOffsetToValidOffset(size))
    {
        return nullptr;
    }
    char * result = &memoryChunk[currentOffset];

    MemoryAllocatedRecord * record = reinterpret_cast<MemoryAllocatedRecord *>(result);
    record->ptr = result + sizeof(MemoryAllocatedRecord);
    record->offset = currentOffset;
    record->length = size;

    allocatedRecord.insert(record);

    allocatedBytes += size;
    return result + sizeof(MemoryAllocatedRecord);
}

bool MemoryBlock::updateCurrentOffsetToValidOffset(unsigned int size)
{
    unsigned int _endPos = this->chunkTotalBytes;

    for (auto ptr = allocatedRecord.crbegin();
        ptr != allocatedRecord.crend();
        ++ptr)
    {
        if (_endPos >= size + (*ptr)->offset + (*ptr)->length)
        {
            currentOffset = (*ptr)->offset + (*ptr)->length;
            return true;
        }
        _endPos = (*ptr)->offset;
    }

    currentOffset = 0;
    return _endPos >= size;
}

void MemoryBlock::free(const void * ptr)
{
    assert(canFree(ptr));

    auto _ptr = reinterpret_cast<const char*>(ptr);
    _ptr -= sizeof(MemoryAllocatedRecord);

    auto _recordPtr = reinterpret_cast<const MemoryAllocatedRecord*>(_ptr);
    auto _recordIt = allocatedRecord.find(_recordPtr);

    allocatedBytes -= (*_recordIt)->length;
    allocatedRecord.erase(_recordIt);
}
