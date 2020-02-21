#include <Lang/Memory/MemoryBlock.h>

#include <gtest/gtest.h>
#include <array>

class MemoryBlockTest : public testing::Test
{
protected:
    virtual void SetUp() override
    {
        EXPECT_EQ(QUARTER_OF_BLOCK_SIZE % 2, 0);
        EXPECT_GT(QUARTER_OF_BLOCK_SIZE - 1, 0);

        memoryBlock = std::make_shared<MemoryBlock>(BLOCK_SIZE + 4 * sizeof(MemoryAllocatedRecord));

        void* _ptr = nullptr;
        for (auto & pointer : pointers)
        {
            _ptr = memoryBlock->malloc(QUARTER_OF_BLOCK_SIZE);
            pointer = reinterpret_cast<char *>(_ptr);
        }
    }

protected:
    const unsigned int BLOCK_SIZE = 24;
    const unsigned int HALF_OF_BLOCK_SIZE = BLOCK_SIZE / 2;
    const unsigned int QUARTER_OF_BLOCK_SIZE = BLOCK_SIZE / 4;

    std::shared_ptr<MemoryBlock> memoryBlock;
    std::array<char *, 4> pointers;
};

TEST_F(MemoryBlockTest, MallocReturnNotNull)
{
    for (const auto & pointer : pointers)
    {
        EXPECT_NE(nullptr, pointer);
    }
}

TEST_F(MemoryBlockTest, ReMalloc)
{
    memoryBlock->free(pointers[2]);
    auto newPtr = memoryBlock->malloc(QUARTER_OF_BLOCK_SIZE);
    EXPECT_EQ(pointers[2], newPtr);
}

TEST_F(MemoryBlockTest, MallocReturnNull)
{
    auto ptr = memoryBlock->malloc(1);
    EXPECT_EQ(ptr, nullptr);
}

TEST_F(MemoryBlockTest, MallocByOrder)
{
    EXPECT_EQ(pointers[1] + QUARTER_OF_BLOCK_SIZE + sizeof(MemoryAllocatedRecord), pointers[2]);
    EXPECT_EQ(pointers[0] + HALF_OF_BLOCK_SIZE + 2 * sizeof(MemoryAllocatedRecord), pointers[2]);
}

TEST_F(MemoryBlockTest, SplitMallocedBeginning)
{
    memoryBlock->free(pointers[0]);
    memoryBlock->free(pointers[3]);
    memoryBlock->malloc(QUARTER_OF_BLOCK_SIZE - 1);
    auto newPtr2 = memoryBlock->malloc(1);

    EXPECT_EQ(newPtr2, pointers[0] + QUARTER_OF_BLOCK_SIZE - 1);
    EXPECT_EQ(newPtr2, pointers[1] - 1 - sizeof(MemoryAllocatedRecord));
}

TEST_F(MemoryBlockTest, SplitMallocedMidder)
{
    memoryBlock->free(pointers[1]);
    memoryBlock->malloc(QUARTER_OF_BLOCK_SIZE - 1);
    auto newPtr2 = reinterpret_cast<char *>(memoryBlock->malloc(1));

    EXPECT_EQ(newPtr2, pointers[1] + QUARTER_OF_BLOCK_SIZE - 1);
    EXPECT_EQ(newPtr2, pointers[2] - 1);
}

TEST_F(MemoryBlockTest, SplitMallocedEnd)
{
    memoryBlock->free(pointers[3]);
    memoryBlock->malloc(QUARTER_OF_BLOCK_SIZE - 1);
    auto newPtr2 = reinterpret_cast<char *>(memoryBlock->malloc(1));

    EXPECT_EQ(newPtr2, pointers[3] + QUARTER_OF_BLOCK_SIZE - 1);
}

TEST_F(MemoryBlockTest, CanFreeReturnTrue)
{
    EXPECT_TRUE(memoryBlock->canFree(pointers[0]));
    EXPECT_TRUE(memoryBlock->canFree(pointers[2]));
}

TEST_F(MemoryBlockTest, CanFreeReturnFalse)
{
    EXPECT_FALSE(memoryBlock->canFree(nullptr));
    EXPECT_FALSE(memoryBlock->canFree(pointers[0] - sizeof(MemoryAllocatedRecord) - 1));
    EXPECT_FALSE(memoryBlock->canFree(pointers[0] + BLOCK_SIZE  + 1 + 3 * sizeof(MemoryAllocatedRecord)));
}

TEST_F(MemoryBlockTest, HasAllocedPointerReturnTrue)
{
    EXPECT_TRUE(memoryBlock->hasAllocedPointer());
}

TEST_F(MemoryBlockTest, HasAllocedPointerReturnFalse)
{
    for (const auto & pointer : pointers)
    {
        memoryBlock->free(pointer);
    }

    EXPECT_FALSE(memoryBlock->hasAllocedPointer());
}

TEST_F(MemoryBlockTest, MemoryUsageRatioIsHigh)
{
    EXPECT_GT(memoryBlock->memoryUsageRatio(), 0.99);
}

TEST_F(MemoryBlockTest, MemoryUsageRatioIsLow)
{
    memoryBlock->free(pointers[0]);
    memoryBlock->free(pointers[1]);
    memoryBlock->free(pointers[2]);
    EXPECT_LT(memoryBlock->memoryUsageRatio(), 0.26);
}
