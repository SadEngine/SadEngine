#pragma once

#include "../Window.h"

#include <gl/glew.h>
#include <GLFW/glfw3.h>

namespace Sparky {
    namespace Graphics {
        class GLWindow : public Window
        {
        private:
            GLFWwindow * window;

        public:
            GLWindow(String title, int width, int heigh);
            virtual ~GLWindow();

            virtual bool isClosed() const override;
            virtual void exec() override;
        protected:
            virtual void init() override;
            virtual void update() override;

        private:
            void initGlfwWindow();
        };
    }
}