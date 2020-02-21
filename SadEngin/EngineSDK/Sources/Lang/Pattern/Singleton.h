#pragma once

#include <assert.h>

#include "../Micro.h"
#include "../Memory/Memory.h"

template<class T>
class Singleton
{
private:
    static T* _instance;
public:

    inline static T* create()
    {
        assert(nullptr == _instance);
        _instance = new T();
        return instance();
    }

    inline static T* instance()
    {
        assert(_instance);
        return _instance;
    }

    inline static void destroy()
    {
        SAFE_DELETE(_instance);
    }
};

template<class T>
T* Singleton<T>::_instance = nullptr;