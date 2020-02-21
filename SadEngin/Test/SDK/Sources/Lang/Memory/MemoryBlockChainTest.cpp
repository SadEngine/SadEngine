#include <Lang/Memory/MemoryBlockChain.h>

#include <gtest/gtest.h>
#include <memory>

class MemoryBlockChainTest : public testing::Test
{
protected:
    virtual void SetUp() override
    {
        EXPECT_GT(BLOCK_SIZE, 1);

        memoryBlockChain = std::make_shared<MemoryBlockChain>(BLOCK_SIZE);

        ptr1InBlock1 = reinterpret_cast<char *>(memoryBlockChain->malloc(1));
        ptr2InBlock1 = reinterpret_cast<char *>(memoryBlockChain->malloc(BLOCK_SIZE - 1));
        ptr3InBlock2 = reinterpret_cast<char *>(memoryBlockChain->malloc(BLOCK_SIZE));
    }

protected:
    const unsigned int BLOCK_SIZE = 8;
    std::shared_ptr<MemoryBlockChain> memoryBlockChain;
    char * ptr1InBlock1, *ptr2InBlock1, *ptr3InBlock2;
};

TEST_F(MemoryBlockChainTest, MallocReturnNotNull)
{
    EXPECT_NE(nullptr, ptr1InBlock1);
    EXPECT_NE(nullptr, ptr2InBlock1);
    EXPECT_NE(nullptr, ptr3InBlock2);
}

TEST_F(MemoryBlockChainTest, MallocResultIsValid)
{
    EXPECT_NE(ptr1InBlock1, ptr2InBlock1);
    EXPECT_NE(ptr1InBlock1, ptr3InBlock2);
    EXPECT_NE(ptr2InBlock1, ptr3InBlock2);
}

TEST_F(MemoryBlockChainTest, ReMalloc)
{
    memoryBlockChain->free(ptr3InBlock2);
    auto ptr = memoryBlockChain->malloc(BLOCK_SIZE);
    EXPECT_EQ(ptr, ptr3InBlock2);
}

TEST_F(MemoryBlockChainTest, MallocUseable)
{
    const int length = 100;
    for (int i = 0; i < length; ++i)
    {
        auto ptr = memoryBlockChain->malloc(BLOCK_SIZE);
        EXPECT_NE(nullptr, ptr);
    }
}

TEST_F(MemoryBlockChainTest, CanFreeReturnTrue)
{
    EXPECT_TRUE(memoryBlockChain->canFree(ptr1InBlock1));
}

TEST_F(MemoryBlockChainTest, CanFreeReturnFalse)
{
    EXPECT_FALSE(memoryBlockChain->canFree(nullptr));
    EXPECT_FALSE(memoryBlockChain->canFree(ptr1InBlock1 - 1));
}

TEST_F(MemoryBlockChainTest, GcReturnTrue)
{
    memoryBlockChain->free(ptr3InBlock2);
    EXPECT_TRUE(memoryBlockChain->gc());
}

TEST_F(MemoryBlockChainTest, GcReturnFalse)
{
    EXPECT_FALSE(memoryBlockChain->gc());
    memoryBlockChain->free(ptr2InBlock1);
    EXPECT_FALSE(memoryBlockChain->gc());
}
