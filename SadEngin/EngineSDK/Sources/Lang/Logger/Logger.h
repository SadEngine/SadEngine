#pragma once

#include<iostream>

#include "../String/String.h"

void LOG(String logData)
{
    std::cout << logData.c_str() << std::endl;
}
