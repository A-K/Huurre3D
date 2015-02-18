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

#include "Engine/App.h"
#include "Engine/Engine.h"

#include "Input/Input.h"
#include "Scene/SceneImporter.h"
#include "NinjaEntity.h"

using namespace Huurre3D;

class AnimationDemo : public App
{
public:
    AnimationDemo() = default;
    ~AnimationDemo() = default;

    void init() override;
    void deinit() override {}
    void update(float timeSinceLastUpdate) override;

private:
    void createScene();
    void createNinjas();
    Camera* camera;
    Scene* scene;
    NinjaEntity mainNinja;
    Vector<NinjaEntity> ninjas;
    const unsigned int numNinjas = 100;
    Vector3 ninjaScale = Vector3(0.25f, 0.25f, 0.25f);
    AnimationClip* ninjaMovements;
    float ninjaMovementStartToEndPosTime = 40.0f;
};