#pragma once

#include <assert.h>
#include "Memory/Memory.h"

#ifdef _MSC_VER
#define PLATFORM_WINDOWS

#endif // _MSC_VER

#ifdef PLATFORM_WINDOWS
#include<Windows.h>
#endif // PLATFORM_WINDOWS

#define SAFE_DELETE(p) { assert(p); delete (p);  (p)=nullptr; }

#define ABSTRACT