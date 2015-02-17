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

#ifndef InputEvents_H
#define InputEvents_H

#include "Util/Vector.h"

namespace Huurre3D
{

enum class EventAction
{
    Release,
    Press, 
    Move,
    Close,
    Resize
};

struct InputEvent
{
    EventAction action;

    InputEvent(EventAction action):
    action(action)
    {
    }
};

struct KeyEvent : public InputEvent
{
    int key;

    KeyEvent(EventAction action, int key):
    InputEvent(action),
    key(key)
    {
    }
};

struct MouseButtonEvent : public InputEvent
{
    int button;

    MouseButtonEvent(EventAction action, int button):
    InputEvent(action),
    button(button)
    {
    }
};

struct MouseCursorEvent : public InputEvent
{
    float xPos;
    float yPos;

    MouseCursorEvent(float xPos, float yPos):
    InputEvent(EventAction::Move),
    xPos(xPos),
    yPos(yPos)
    {
    }
};

struct MouseScrollEvent : public InputEvent
{
    float xOffset;
    float yOffset;

    MouseScrollEvent(float xOffset, float yOffset):
    InputEvent(EventAction::Move),
    xOffset(xOffset),
    yOffset(yOffset)
    {
    }
};

struct WindowEvent : public InputEvent
{
    int width;
    int height;
    WindowEvent(EventAction action, int width, int height):
    width(width),
    height(height),
    InputEvent(action)
    {
    }
};

struct InputEventsContainer
{
    Vector<KeyEvent> keyEvents;
    Vector<MouseButtonEvent> mouseButtonEvents;
    Vector<MouseCursorEvent> mouseCursorEvents;
    Vector<MouseScrollEvent> mouseScrollEvents;
    Vector<WindowEvent> windowEvents;

    void clearEvents()
    {
        keyEvents.clear();
        mouseButtonEvents.clear();
        mouseCursorEvents.clear();
        mouseScrollEvents.clear();
        windowEvents.clear();
    }
};

extern const int MOUSE_BUTTON_LEFT;
extern const int MOUSE_BUTTON_MIDDLE;
extern const int MOUSE_BUTTON_RIGHT;

extern const int KEY_0;
extern const int KEY_1;
extern const int KEY_2;
extern const int KEY_3;
extern const int KEY_4;
extern const int KEY_5;
extern const int KEY_6;
extern const int KEY_7;
extern const int KEY_8;
extern const int KEY_9;
extern const int KEY_LEFT;
extern const int KEY_RIGHT;
extern const int KEY_UP;
extern const int KEY_DOWN;
extern const int KEY_CTRL;
extern const int KEY_SHIFT;
extern const int KEY_ENTER;
extern const int KEY_SPACE;
extern const int KEY_TAB;
extern const int KEY_ESCAPE;
extern const int KEY_BACKSPACE;
extern const int KEY_HOME;
extern const int KEY_END;
extern const int KEY_INSERT;
extern const int KEY_DELETE;
extern const int KEY_F1;
extern const int KEY_F2;
extern const int KEY_F3;
extern const int KEY_F4;
extern const int KEY_F5;
extern const int KEY_F6;
extern const int KEY_F7;
extern const int KEY_F8;
extern const int KEY_F9;
extern const int KEY_F10;
extern const int KEY_F11;
extern const int KEY_F12;
extern const int KEY_NUMPAD0;
extern const int KEY_NUMPAD1;
extern const int KEY_NUMPAD2;
extern const int KEY_NUMPAD3;
extern const int KEY_NUMPAD4;
extern const int KEY_NUMPAD5;
extern const int KEY_NUMPAD6;
extern const int KEY_NUMPAD7;
extern const int KEY_NUMPAD8;
extern const int KEY_NUMPAD9;
extern const int KEY_ADD;
extern const int KEY_SUBTRACT;
extern const int KEY_MULTIPLY;
extern const int KEY_DIVIDE;
extern const int KEY_DECIMAL;
extern const int KEY_PAUSE;
extern const int KEY_A;
extern const int KEY_B;
extern const int KEY_C;
extern const int KEY_D;
extern const int KEY_E;
extern const int KEY_F;
extern const int KEY_G;
extern const int KEY_H;
extern const int KEY_I;
extern const int KEY_J;
extern const int KEY_K;
extern const int KEY_L;
extern const int KEY_M;
extern const int KEY_N;
extern const int KEY_O;
extern const int KEY_P;
extern const int KEY_Q;
extern const int KEY_R;
extern const int KEY_S;
extern const int KEY_T;
extern const int KEY_U;
extern const int KEY_V;
extern const int KEY_W;
extern const int KEY_X;
extern const int KEY_Y;
extern const int KEY_Z;

}

#endif