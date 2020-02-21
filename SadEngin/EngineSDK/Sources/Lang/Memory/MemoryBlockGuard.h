#pragma once

#include <time.h>
#include <list>

class MemoryBlockGuard
{
public:
    float memoryBlockUsage();
    bool  memoryBlockCanMalloc();

    /**
     *调用malloc、free的总次数
    */
    int mallocCallNumber();
    int freeCallnumber();

    /**
     *调用malloc、free的总时间
    */
    time_t mallocAllSpendTimes();
    time_t freeAllSpendTimes();

    /**
     *上次调用malloc/free的时间花费
    */
    time_t lastSpendTimes();
    time_t lastFreeTimes();

    /**
     *所有的malloc/free的调用时间花费历史记录
    */
    std::list<time_t> allSpendTimes();
    std::list<time_t> allFreeTimes();
};