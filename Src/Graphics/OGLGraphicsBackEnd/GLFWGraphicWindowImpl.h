//
// Copyright (c) 2013-2015 Antti Karhu.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef GLFWGraphicWindowImpl_H
#define GLFWGraphicWindowImpl_H

#include "Graphics/GraphicWindowImpl.h"
#include "Graphics/OGLGraphicsBackEnd/OGLHeaders.h"

namespace Huurre3D
{

class GLFWGraphicWindowImpl : public GraphicWindowImpl
{
public:
    GLFWGraphicWindowImpl() = default;
    ~GLFWGraphicWindowImpl();

    bool create(int width, int height, const std::string& windowTitle, bool fullScreen, bool vsync);
    void setWindowTitle(const std::string& windowTitle);
    void destroy() const;
    void swapBuffers() const;
    const InputEventsContainer& getEvents() const;

private:
    static void gatherKeyEvents(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void gatherMouseButtonEvents(GLFWwindow* window, int button, int action, int mods);
    static void gatherMouseCursorEvents(GLFWwindow* window, double posX, double posY);
    static void gatherMouseScrollEvents(GLFWwindow* window, double xOffset, double yOffset);
    static void windowResizeCallback(GLFWwindow* window, int width, int height);
    GLFWwindow* window = nullptr;
};

}
#endif