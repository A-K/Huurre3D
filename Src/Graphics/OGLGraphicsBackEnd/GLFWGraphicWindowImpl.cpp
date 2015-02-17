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

#include "Graphics/OGLGraphicsBackEnd/GLFWGraphicWindowImpl.h"
#include <iostream>

namespace Huurre3D
{

GLFWGraphicWindowImpl::~GLFWGraphicWindowImpl()
{
    destroy();
    glfwTerminate();
}

bool GLFWGraphicWindowImpl::create(int width, int height, const std::string& windowTitle, bool fullScreen, bool vsync)
{
    if(window)
        return true;

    if(!glfwInit())
    {
        std::cout << "Failed to initialize GLFW\n" << std::endl;
    }

    if(fullScreen)
        window = glfwCreateWindow(width, height, windowTitle.c_str(), glfwGetPrimaryMonitor(), 0);
    else
        window = glfwCreateWindow(width, height, windowTitle.c_str(), 0, 0);

    if(!window)
    {
        std::cout << "Failed to create a window\n" << std::endl;
        return false;
    }

    glfwMakeContextCurrent(window);

    GLenum error = glewInit();
    if(GLEW_OK != error)
    {
        std::cout << "Failed to initialize GLEW: " << glewGetErrorString(error) <<std::endl;
        return false;
    }

    if (!GLEW_VERSION_3_3)
    {
        std::cout<< "Open GL 3.3 required"<<std::endl;
        return false;
    }

    glfwSwapInterval(vsync ? 1 : 0);
    glfwSetWindowUserPointer(window, &inputEvents);
    glfwSetKeyCallback(window, gatherKeyEvents);
    glfwSetMouseButtonCallback(window, gatherMouseButtonEvents);  
    glfwSetCursorPosCallback(window, gatherMouseCursorEvents);
    glfwSetScrollCallback(window, gatherMouseScrollEvents);
    glfwSetWindowSizeCallback(window, windowResizeCallback);

    return true;
}

void GLFWGraphicWindowImpl::setWindowTitle(const std::string& windowTitle)
{
    glfwSetWindowTitle(window, windowTitle.c_str());
}

void GLFWGraphicWindowImpl::destroy() const
{
    glfwDestroyWindow(window);
}

void GLFWGraphicWindowImpl::swapBuffers() const
{
    glfwSwapBuffers(window);
}

const InputEventsContainer& GLFWGraphicWindowImpl::getEvents() const
{
    inputEvents.clearEvents();

    if(window)
    {
        if(glfwWindowShouldClose(window))
            inputEvents.windowEvents.pushBack(WindowEvent(EventAction::Close, 0, 0));

        glfwPollEvents();
    }

    return inputEvents;
}

void GLFWGraphicWindowImpl::gatherKeyEvents(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    InputEventsContainer* inputEvents;
    inputEvents = static_cast<InputEventsContainer*>(glfwGetWindowUserPointer(window));
    inputEvents->keyEvents.pushBack(KeyEvent(static_cast<EventAction>(action), key));
}

void GLFWGraphicWindowImpl::gatherMouseButtonEvents(GLFWwindow* window, int button, int action, int mods)
{
    InputEventsContainer* inputEvents;
    inputEvents = static_cast<InputEventsContainer*>(glfwGetWindowUserPointer(window));
    inputEvents->mouseButtonEvents.pushBack(MouseButtonEvent(static_cast<EventAction>(action), button));
}

void GLFWGraphicWindowImpl::gatherMouseCursorEvents(GLFWwindow* window, double posX, double posY)
{
    InputEventsContainer* inputEvents;
    inputEvents = static_cast<InputEventsContainer*>(glfwGetWindowUserPointer(window));
    inputEvents->mouseCursorEvents.pushBack(MouseCursorEvent((float)posX, (float)posY));
}

void GLFWGraphicWindowImpl::gatherMouseScrollEvents(GLFWwindow* window, double xOffset, double yOffset)
{
    InputEventsContainer* inputEvents;
    inputEvents = static_cast<InputEventsContainer*>(glfwGetWindowUserPointer(window));
    inputEvents->mouseScrollEvents.pushBack(MouseScrollEvent((float)xOffset, (float)yOffset));
}


void GLFWGraphicWindowImpl::windowResizeCallback(GLFWwindow* window, int width, int height)
{
    InputEventsContainer* inputEvents;
    inputEvents = static_cast<InputEventsContainer*>(glfwGetWindowUserPointer(window));
    inputEvents->windowEvents.pushBack(WindowEvent(EventAction::Resize, width, height));
}

const int MOUSE_BUTTON_LEFT = GLFW_MOUSE_BUTTON_LEFT;
const int MOUSE_BUTTON_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE;
const int MOUSE_BUTTON_RIGHT = GLFW_MOUSE_BUTTON_RIGHT;

const int KEY_0 = GLFW_KEY_0;
const int KEY_1 = GLFW_KEY_1;
const int KEY_2 = GLFW_KEY_2;
const int KEY_3 = GLFW_KEY_3;
const int KEY_4 = GLFW_KEY_4;
const int KEY_5 = GLFW_KEY_5;
const int KEY_6 = GLFW_KEY_6;
const int KEY_7 = GLFW_KEY_7;
const int KEY_8 = GLFW_KEY_8;
const int KEY_9 = GLFW_KEY_9;
const int KEY_LEFT = GLFW_KEY_LEFT;
const int KEY_RIGHT = GLFW_KEY_RIGHT;
const int KEY_UP = GLFW_KEY_UP;
const int KEY_DOWN = GLFW_KEY_DOWN;
const int KEY_CTRL = GLFW_KEY_LEFT_CONTROL;
const int KEY_SHIFT = GLFW_KEY_LEFT_SHIFT;
const int KEY_ENTER = GLFW_KEY_ENTER;
const int KEY_SPACE = GLFW_KEY_SPACE;
const int KEY_TAB = GLFW_KEY_TAB;
const int KEY_ESCAPE = GLFW_KEY_ESCAPE;
const int KEY_BACKSPACE = GLFW_KEY_BACKSPACE;
const int KEY_HOME = GLFW_KEY_HOME;
const int KEY_END = GLFW_KEY_END;
const int KEY_INSERT = GLFW_KEY_INSERT;
const int KEY_DELETE = GLFW_KEY_DELETE;
const int KEY_F1 = GLFW_KEY_F1;
const int KEY_F2 = GLFW_KEY_F2;
const int KEY_F3 = GLFW_KEY_F3;
const int KEY_F4 = GLFW_KEY_F4;
const int KEY_F5 = GLFW_KEY_F5;
const int KEY_F6 = GLFW_KEY_F6;
const int KEY_F7 = GLFW_KEY_F7;
const int KEY_F8 = GLFW_KEY_F8;
const int KEY_F9 = GLFW_KEY_F9;
const int KEY_F10 = GLFW_KEY_F10;
const int KEY_F11 = GLFW_KEY_F11;
const int KEY_F12 = GLFW_KEY_F12;
const int KEY_NUMPAD0 = GLFW_KEY_KP_0;
const int KEY_NUMPAD1 = GLFW_KEY_KP_1;
const int KEY_NUMPAD2 = GLFW_KEY_KP_2;
const int KEY_NUMPAD3 = GLFW_KEY_KP_3;
const int KEY_NUMPAD4 = GLFW_KEY_KP_4;
const int KEY_NUMPAD5 = GLFW_KEY_KP_5;
const int KEY_NUMPAD6 = GLFW_KEY_KP_6;
const int KEY_NUMPAD7 = GLFW_KEY_KP_7;
const int KEY_NUMPAD8 = GLFW_KEY_KP_8;
const int KEY_NUMPAD9 = GLFW_KEY_KP_9;
const int KEY_ADD = GLFW_KEY_KP_ADD;
const int KEY_SUBTRACT = GLFW_KEY_KP_SUBTRACT;
const int KEY_MULTIPLY = GLFW_KEY_KP_MULTIPLY;
const int KEY_DIVIDE = GLFW_KEY_KP_DIVIDE;
const int KEY_DECIMAL = GLFW_KEY_KP_DECIMAL;
const int KEY_PAUSE = GLFW_KEY_PAUSE;
const int KEY_A = GLFW_KEY_A;
const int KEY_B = GLFW_KEY_B;
const int KEY_C = GLFW_KEY_C;
const int KEY_D = GLFW_KEY_D;
const int KEY_E = GLFW_KEY_E;
const int KEY_F = GLFW_KEY_F;
const int KEY_G = GLFW_KEY_G;
const int KEY_H = GLFW_KEY_H;
const int KEY_I = GLFW_KEY_I;
const int KEY_J = GLFW_KEY_J;
const int KEY_K = GLFW_KEY_K;
const int KEY_L = GLFW_KEY_L;
const int KEY_M = GLFW_KEY_M;
const int KEY_N = GLFW_KEY_N;
const int KEY_O = GLFW_KEY_O;
const int KEY_P = GLFW_KEY_P;
const int KEY_Q = GLFW_KEY_Q;
const int KEY_R = GLFW_KEY_R;
const int KEY_S = GLFW_KEY_S;
const int KEY_T = GLFW_KEY_T;
const int KEY_U = GLFW_KEY_U;
const int KEY_V = GLFW_KEY_V;
const int KEY_W = GLFW_KEY_W;
const int KEY_X = GLFW_KEY_X;
const int KEY_Y = GLFW_KEY_Y;
const int KEY_Z = GLFW_KEY_Z;

}