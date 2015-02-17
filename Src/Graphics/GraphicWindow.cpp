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

#include "Graphics/GraphicWindow.h"

namespace Huurre3D
{

GraphicWindow::GraphicWindow()
{
    graphicWindowImpl = CreateGraphicWindowImpl();
}

GraphicWindow::~GraphicWindow() 
{
    delete graphicWindowImpl;
}

bool GraphicWindow::create(int width, int height, const std::string& windowTitle, bool fullscreen, bool vsync)
{
    this->height = height;
    this->width = width;
    this->windowTitle = windowTitle;
    this->vsync = vsync;
    this->fullScreen = fullScreen;
    return graphicWindowImpl->create(width, height, windowTitle, fullscreen, vsync);
}

void GraphicWindow::setWindowTitle(const std::string& windowTitle) 
{
    graphicWindowImpl->setWindowTitle(windowTitle);
    this->windowTitle = windowTitle;
}

}