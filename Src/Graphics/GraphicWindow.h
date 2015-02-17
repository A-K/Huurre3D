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

#ifndef GraphicWindow_H
#define GraphicWindow_H

#ifdef USE_OGL
#include "Graphics/OGLGraphicsBackEnd/GLFWGraphicWindowImpl.h"
#define GraphicWindowImpl GLFWGraphicWindowImpl
#endif

#ifdef USE_NULL
#include "Graphics/GraphicWindowImpl.h"
#endif

namespace Huurre3D
{

class GraphicWindow
{
public:
    GraphicWindow();
    ~GraphicWindow();

    bool create(int width, int height, const std::string& windowTitle, bool fullscreen = false, bool vsync = false);
    void setWindowTitle(const std::string& windowTitle);
    void destroy() const {graphicWindowImpl->destroy();}
    void swapBuffers() const {graphicWindowImpl->swapBuffers();}
    const InputEventsContainer& getEvents() const {return graphicWindowImpl->getEvents();}
    int getHeight() const {return height;}
    int getWidth() const {return width;}

protected:
    GraphicWindowImpl* graphicWindowImpl = nullptr;
    int height = 0;
    int width = 0;
    bool fullScreen = false;
    bool vsync = false;
    std::string windowTitle;

#ifdef USE_OGL
    GraphicWindowImpl* CreateGraphicWindowImpl() const {return new GLFWGraphicWindowImpl();}
#endif

#ifdef USE_NULL
    GraphicWindowImpl* CreateGraphicWindowImpl() const { return new GraphicWindowImpl(); }
#endif

};

}

#endif