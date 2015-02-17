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

#ifndef Input_H
#define Input_H

#include "Input/InputEvents.h"
#include "Graphics/GraphicWindow.h"
#include "Math/Vector2.h"
#include "Util/FixedArray.h"

namespace Huurre3D
{

class Input
{
public:
    Input(GraphicWindow* graphicWindow);
    ~Input() = default;

    void update();
    //Returns true if the given key is held down at this moment.
    bool isKeyDown(int key) const;
    //Returns true if the given key has been pressed on this frame.
    bool isKeyPressed(int key) const;
    bool windowResized(int& newWidth, int& newHeight) const;
    //Return is the given mouse button held down at this moment.
    bool isMouseButtonDown(int button) const {return (mouseButtonsDownFlag & (1 << button)) != 0;}
    //Returns true if the given mouse button has been pressed on this frame.
    bool isMouseButtonPressed(int button) const {return (mouseButtonsPressedFlag & (1 << button)) != 0;}
    const Vector2& getMouseCursorPos() const {return mouseCursorPos;}
    const Vector2& getMouseCursorDelta() const {return mouseCursorDelta;}
    const Vector2& getMouseScrollOffset() const {return mouseScrollOffset;}
    const Vector2& getMouseScrollDelta() const {return mouseScrollDelta;}
    bool windowClosed() const {return windowShouldClose;}

private:
    void processKeyEvent(const Vector<KeyEvent>& keyEvents);
    void processMouseButtonEvent(const Vector<MouseButtonEvent>& mouseButtonEvents);
    void processMouseCursorEvent(const Vector<MouseCursorEvent>& mouseCursorEvents);
    void processMouseScrollEvent(const Vector<MouseScrollEvent>& mouseScrollEvents);
    void processWindowEvents(const Vector<WindowEvent>& windowEvents);

    GraphicWindow* graphicWindow;
    Vector<int> keysDown;
    Vector<int> keysPressed;
    int mouseButtonsDownFlag;
    int mouseButtonsPressedFlag;
    Vector2 mouseCursorPos = Vector2::ZERO;
    Vector2 mouseScrollOffset = Vector2::ZERO;
    Vector2 mouseCursorDelta = Vector2::ZERO;
    Vector2 mouseScrollDelta = Vector2::ZERO;
    FixedArray<int, 2> windowDimensions;
    bool windowShouldClose = false;
    bool windowShouldResize = false;
};

}

#endif