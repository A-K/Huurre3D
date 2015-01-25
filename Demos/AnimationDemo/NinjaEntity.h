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


#include "Scene/Mesh.h"

using namespace Huurre3D;

enum class Action
{
    Walk,
    Idle,
    Kick,
    SwordSwipe,
    BackFlip
};

enum class Direction
{
    Left,
    Right,
    Forward,
    Backward
};

class NinjaEntity
{
public:
    NinjaEntity() = default;
    NinjaEntity(const Vector3 startPosition, const Vector3 scale, Mesh* mesh);
    ~NinjaEntity() = default;
    void move(float delta, Direction direction);
    void rotate(float delta, Direction direction);
    void setAction(Action action);
    void attachCamera(Camera* camera);

private:
    Mesh* mesh;
    float animationSpeed = 0.75;
    Action currentAction = Action::Idle;
    Vector3 posCeil = Vector3(80.0f, 0.0f, 80.0f);
    Vector3 posFloor = Vector3(-185.0f, 0.0f, -185.0f);
};