#pragma once

#include <time.h>
#include <list>

class MemoryBlockGuard
{
public:
    float memoryBlockUsage();
    bool  memoryBlockCanMalloc();

    /**
     *����malloc��free���ܴ���
    */
    int mallocCallNumber();
    int freeCallnumber();

    /**
     *����malloc��free����ʱ��
    */
    time_t mallocAllSpendTimes();
    time_t freeAllSpendTimes();

    /**
     *�ϴε���malloc/free��ʱ�仨��
    */
    time_t lastSpendTimes();
    time_t lastFreeTimes();

    /**
     *���е�malloc/free�ĵ���ʱ�仨����ʷ��¼
    */
    std::list<time_t> allSpendTimes();
    std::list<time_t> allFreeTimes();
};