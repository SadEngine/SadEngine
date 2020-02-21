#include "Window.h"

namespace Sparky {
    namespace Graphics {
        Window::Window(String title, int width, int height)
        {
            this->title = title;
            this->width = width;
            this->height = height;

            this->init();
        }
    }
}