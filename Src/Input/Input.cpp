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

#include "input/Input.h"

namespace Huurre3D
{

Input::Input(GraphicWindow* graphicWindow):
graphicWindow(graphicWindow)
{
}

void Input::update()
{
    windowShouldResize = false;
    keysPressed.clear();
    mouseButtonsPressedFlag = 0;
    mouseCursorDelta = Vector2::ZERO;
    mouseScrollDelta = Vector2::ZERO;

    InputEventsContainer inputEvents = graphicWindow->getEvents();
    processKeyEvent(inputEvents.keyEvents);
    processMouseButtonEvent(inputEvents.mouseButtonEvents);
    processMouseCursorEvent(inputEvents.mouseCursorEvents);
    processMouseScrollEvent(inputEvents.mouseScrollEvents);
    processWindowEvents(inputEvents.windowEvents);
}

bool Input::isKeyDown(int key) const
{
    return (!keysDown.empty() && keysDown.containsItem(key));
}

bool Input::isKeyPressed(int key) const
{
    return (!keysPressed.empty() && keysPressed.containsItem(key));
}

bool Input::windowResized(int& newWidth, int& newHeight) const
{
    newWidth =  windowDimensions[0];
    newHeight = windowDimensions[1];
    return windowShouldResize;
}

void Input::processKeyEvent(const Vector<KeyEvent>& keyEvents)
{
    for(unsigned int i = 0; i < keyEvents.size(); ++i)
    {
        if(keyEvents[i].action == EventAction::Press)
        {
            keysDown.pushBack(keyEvents[i].key);
            keysPressed.pushBack(keyEvents[i].key);
        }

        else if(keyEvents[i].action == EventAction::Release)
            keysDown.eraseUnordered(keyEvents[i].key);
    }
}

void Input::processMouseButtonEvent(const Vector<MouseButtonEvent>& mouseButtonEvents)
{
    for(unsigned int i = 0; i < mouseButtonEvents.size(); ++i)
    {
        if(mouseButtonEvents[i].action == EventAction::Press)
        {
            mouseButtonsDownFlag |= 1 << mouseButtonEvents[i].button;
            mouseButtonsPressedFlag |= 1 << mouseButtonEvents[i].button;
        }

        else if(mouseButtonEvents[i].action == EventAction::Release)
            mouseButtonsDownFlag &= ~(1 << mouseButtonEvents[i].button);
    }
}

void Input::processMouseCursorEvent(const Vector<MouseCursorEvent>& mouseCursorEvents)
{
    for(unsigned int i = 0; i < mouseCursorEvents.size(); ++i)
    {
        Vector2 newPos = Vector2(mouseCursorEvents[i].xPos, mouseCursorEvents[i].yPos);
        mouseCursorDelta = mouseCursorPos - newPos;
        mouseCursorPos = newPos;
    }
} 

void Input::processMouseScrollEvent(const Vector<MouseScrollEvent>& mouseScrollEvents)
{
    for(unsigned int i = 0; i < mouseScrollEvents.size(); ++i)
    {
        Vector2 newOffset = Vector2(mouseScrollEvents[i].xOffset, mouseScrollEvents[i].xOffset);
        mouseScrollDelta = mouseScrollOffset - newOffset;
        mouseScrollOffset = newOffset;
    }
}

void Input::processWindowEvents(const Vector<WindowEvent>& windowEvents)
{
    for(unsigned int i = 0; i < windowEvents.size(); ++i)
    {
        if(windowEvents[i].action == EventAction::Close)
            windowShouldClose = true;

        else if(windowEvents[i].action == EventAction::Resize)
        {
            windowShouldResize = true;
            windowDimensions[0] = windowEvents[i].width;
            windowDimensions[1] = windowEvents[i].height;
        }
    }
}

}