#pragma once

#include "../Micro.h"

ABSTRACT class Newable
{
public:
    virtual void create() = 0;
    virtual void destroy() = 0;
};