#include <Lang/Memory/MemoryPoll.h>

#include <gtest/gtest.h>
#include <memory>
#include <chrono>

class MemoryPollTest : public testing::Test
{
protected:
    virtual void SetUp() override
    {
        memoryPoll = std::make_shared<MemoryPoll>();
    }

    _int64 getPoolMallocdTime()
    {
        using namespace std::chrono;
        auto start_time = std::chrono::system_clock::now();
        memoryPoll->malloc(32);
        auto end_time = std::chrono::system_clock::now();

        auto duration = duration_cast<nanoseconds>(end_time - start_time);
        return duration.count();
    }

    _int64 getSystemMallocdTime()
    {
        using namespace std::chrono;
        auto start_time = std::chrono::system_clock::now();
        auto ptr = malloc(32);
        auto end_time = std::chrono::system_clock::now();
        //free(ptr);
        auto duration = duration_cast<nanoseconds>(end_time - start_time);
        return duration.count();
    }

protected:
    const unsigned int ONE_K = 1024;
    std::shared_ptr<MemoryPoll> memoryPoll;
};

TEST_F(MemoryPollTest, MallocReturnNotNull)
{
    EXPECT_NE(nullptr, memoryPoll->malloc(ONE_K * ONE_K * 6));
    EXPECT_NE(nullptr, memoryPoll->malloc(ONE_K * ONE_K * 6));
}

TEST_F(MemoryPollTest, MallocUseable)
{
    memoryPoll->malloc(32);
    auto start_time = getPoolMallocdTime();

    int count = 0;
    for (int i = 0; i < 200000; ++i)
    {
        auto malloc_time = getPoolMallocdTime();
        if (malloc_time > start_time * 2)
        {
            count++;
        }
    }
    EXPECT_LT(count, 250);
}

TEST_F(MemoryPollTest, PollFastThenDefaultMalloc)
{
    memoryPoll->malloc(32);
    auto _pollMallocTime = getPoolMallocdTime();
    auto _systemMallocTime = getSystemMallocdTime();
    EXPECT_LT(_pollMallocTime, _systemMallocTime);
}
