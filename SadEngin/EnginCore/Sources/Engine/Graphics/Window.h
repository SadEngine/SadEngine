#pragma once

#include <vector>

#include <Lang/String/String.h>
#include <Lang/Pattern/Singleton.h>

namespace Sparky {
    namespace Graphics {
        class Window
        {
        protected:
            String title;
            int width, height;

        public:
            Window(String title, int width, int height);
            virtual ~Window() {};

            virtual void update() {};
            virtual bool isClosed() const { return true; }

            virtual void exec() {}

        protected:
            virtual void init() {};
        };

        class WindowManager : public Singleton<WindowManager>
        {
        private:
            std::vector<Window> createdwindows;

        public:
        };
    }
}