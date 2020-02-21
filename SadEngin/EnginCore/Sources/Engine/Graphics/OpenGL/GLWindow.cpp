#include "GLWindow.h"

#include <assert.h>
#include <string>

namespace Sparky {
    namespace Graphics {
        GLWindow::GLWindow(String title, int width, int heigh)
            :Window::Window(title, width, heigh)
        {
            init();
        }

        GLWindow::~GLWindow()
        {
            glfwTerminate();
        }

        void GLWindow::init()
        {
            assert(glfwInit());
            this->initGlfwWindow();
            assert(glewInit() == GLEW_OK);
        }

        void GLWindow::initGlfwWindow()
        {
            assert(nullptr == this->window);

            const std::string title_askii(this->title.begin(), this->title.end());
            this->window = glfwCreateWindow(this->width, this->height, title_askii.c_str(), nullptr, nullptr);
            assert(this->window);

            glfwSetWindowSizeCallback(
                this->window,
                [](GLFWwindow *, int width, int height) -> void {
                glViewport(0, 0, width, height);
            }
            );
            glfwMakeContextCurrent(window);
        }

        void GLWindow::exec()
        {
            while (!this->isClosed())
            {
                this->update();
            }
        }

        bool GLWindow::isClosed() const
        {
            return glfwWindowShouldClose(window);
        }

        void GLWindow::update()
        {
            glfwGetFramebufferSize(window, &width, &height);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
}