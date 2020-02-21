#include <vld.h>
#include <iostream>
#include <memory>

#include "../Graphics/OpenGL/GLWindow.h"

//static Sparky::Graphics::GLWindow window(L"Sparky Game Engine, SparkyÓÎÏ·ÒıÇæ", 800, 600);

int main()
{
    Sparky::Graphics::WindowManager::create();
    auto ints = Sparky::Graphics::WindowManager::instance();
    Sparky::Graphics::WindowManager::destroy();
    //window.exec();
    return 0;
}