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

#include "NinjaEntity.h"
#include "Animation/AnimationClip.h"
#include "Scene/Camera.h"

NinjaEntity::NinjaEntity(const Vector3 startPosition, const Vector3 scale, Mesh* mesh) :
mesh(mesh)
{
    mesh->setPosition(startPosition);
    mesh->setScale(scale);
    setAction(Action::Walk);
    mesh->getAnimationClip(0)->setSpeed(animationSpeed);
    mesh->getAnimationClip(0)->play();
}

void NinjaEntity::move(float delta, Direction direction)
{
    switch(direction)
    {
        case Direction::Forward:
            mesh->translate((Vector3(-delta, 0.0f, 0.0f)), FrameOfReference::Local);
            break;
        case Direction::Backward:
            mesh->translate((Vector3(delta, 0.0f, 0.0f)), FrameOfReference::Local);
            break;
    }
    
    mesh->setPosition(clamp(mesh->getPosition(FrameOfReference::Local), posFloor, posCeil));
}

void NinjaEntity::rotate(float delta, Direction direction)
{
    switch(direction)
    {
    case Direction::Left:
        mesh->rotate(Quaternion(delta, Vector3::UNIT_Y), FrameOfReference::World);
        break;
    case Direction::Right:
        mesh->rotate(Quaternion(-delta, Vector3::UNIT_Y), FrameOfReference::World);
        break;
    }
}

void NinjaEntity::setAction(Action action)
{
    if(action == Action::Walk && currentAction != Action::Walk)
        mesh->getAnimationClip(0)->setRange(0.0f, 0.56f);
    else if(action == Action::Idle && currentAction != Action::Idle)
        mesh->getAnimationClip(0)->setRange(8.24f, 12.0f);
    else if(action == Action::Kick && currentAction != Action::Kick)
        mesh->getAnimationClip(0)->setRange(2.92f, 3.32f);
    else if(action == Action::BackFlip && currentAction != Action::BackFlip)
        mesh->getAnimationClip(0)->setRange(5.84f, 6.32f);
    else if(action == Action::SwordSwipe && currentAction != Action::SwordSwipe)
        mesh->getAnimationClip(0)->setRange(1.8f, 2.36f);
    
    currentAction = action;
}

void NinjaEntity::attachCamera(Camera* camera)
{
    //Mesh is scaled down and has been exported as Z-axis up,
    //therefore the camera must be transformed when it's set as a child to the mesh.
    Vector3 meshScale = mesh->getScale(FrameOfReference::Local);
    Vector3 invScale(1.0f / meshScale.x, 1.0f / meshScale.y, 1.0f / meshScale.z);
    camera->setTransform(Vector3(20.0f, 0.0f, 10.0f), mesh->getRotation(FrameOfReference::Local), invScale);
    camera->yaw(90.0f, FrameOfReference::Local);
    camera->roll(180.0f, FrameOfReference::Local);
    mesh->addChild(static_cast<SpatialSceneItem*>(camera));
}