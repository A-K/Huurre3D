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

#include "Scene/SpatialSceneItem.h"
#include <iostream>

namespace Huurre3D
{

void SpatialSceneItem::setPosition(const Vector3& position)
{
    localPosition = position;
    if(!transformDirty)
        setTransformDirty();
}

void SpatialSceneItem::setRotation(const Quaternion& rotation)
{
    localRotation = rotation;
    if(!transformDirty)
        setTransformDirty();
}

void SpatialSceneItem::setScale(const Vector3& scale)
{
    localScale = scale;
    if(!transformDirty)
        setTransformDirty();
}

void SpatialSceneItem::setScale(float scale)
{
    localScale = Vector3(scale, scale, scale);
    if(!transformDirty)
        setTransformDirty();
}

void SpatialSceneItem::setTransform(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
{
    setPosition(position);
    setRotation(rotation);
    setScale(scale);
}

const Vector3& SpatialSceneItem::getPosition(FrameOfReference frame)
{
    return frame == FrameOfReference::Local ? localPosition : worldPosition;
}

const Quaternion& SpatialSceneItem::getRotation(FrameOfReference frame)
{
    return frame == FrameOfReference::Local ? localRotation : worldRotation;
}

const Vector3& SpatialSceneItem::getScale(FrameOfReference frame)
{
    return frame == FrameOfReference::Local ? localScale : worldScale;
}

void SpatialSceneItem::translate(const Vector3& delta, FrameOfReference frame)
{
    frame == FrameOfReference::Local ? localPosition += localRotation.rotate(delta) : localPosition += delta;
		
    if(!transformDirty)
        setTransformDirty();
}

void SpatialSceneItem::rotate(const Quaternion& delta, FrameOfReference frame)
{
    frame == FrameOfReference::Local ? localRotation = localRotation * delta : localRotation = delta * localRotation;

    localRotation.normalize();
		
    if(!transformDirty)
        setTransformDirty();
}

void SpatialSceneItem::pitch(float angle, FrameOfReference frame)
{
    rotate(Quaternion(angle, Vector3::UNIT_X), frame);
}

void SpatialSceneItem::yaw(float angle, FrameOfReference frame)
{
    rotate(Quaternion(angle, Vector3::UNIT_Y), frame);
}

void SpatialSceneItem::roll(float angle, FrameOfReference frame)
{
    rotate(Quaternion(angle, Vector3::UNIT_Z), frame);
}

void SpatialSceneItem::scale(const Vector3& delta)
{
    localScale *= delta;
    if(!transformDirty)
        setTransformDirty();
}

void SpatialSceneItem::scale(float delta)
{
    localScale *= delta;
    if(!transformDirty)
        setTransformDirty();
}

void SpatialSceneItem::lookAt(const Vector3& target, const Vector3& upVector, FrameOfReference frame)
{
    Vector3 targetX;
    Vector3 targetY;
    Vector3 targetZ;
		
    frame == FrameOfReference::World ? targetZ = (target - worldPosition).normalized() : targetZ = (target - localPosition).normalized();
        
    targetX = upVector.cross(targetZ).normalized();
    targetY = targetZ.cross(targetX).normalized();	
    Quaternion newRotation = Quaternion(targetX, targetY, targetZ);
        
    (frame == FrameOfReference::Local || !parent) ? setRotation(newRotation) : setRotation(parent->worldRotation.inverse() * newRotation);
}

void SpatialSceneItem::updateWorldTransform()
{
    if(transformDirty)
    {
        if(!parent)
        {
            worldPosition = localPosition;
            worldRotation = localRotation;
            worldScale = localScale;
        }
        else
        {
            parent->updateWorldTransform();
            worldPosition = parent->worldPosition + (parent->worldRotation.rotate(parent->worldScale * localPosition));
            worldRotation = parent->worldRotation * localRotation;
            worldScale = parent->worldScale * localScale;
        }

        worldTransform.setTransform(worldPosition, worldRotation, worldScale);
        inverseWorldTransform.setInverseTransform(worldPosition, worldRotation, worldScale);
        transformDirty = false;
    }
}

void SpatialSceneItem::addChild(SpatialSceneItem* child)
{
    if(!child || child == this)
        return;

    if(child->parent)
    {
        std::cout << "Failed to add child because SpatialSceneItem " <<child->getId()<< " already have parent." <<std::endl;
        return;
    }

    //if the new child node has child nodes, check for possible cyclic assignments  
    if(child->hasChildren())
    {
        if(child->hasChild(this, true))
            this->removeParent();
    }

    child->parent = this;
    children.pushBack(child);
}

void SpatialSceneItem::removeChild(SpatialSceneItem* child)
{
    children.eraseUnordered(child);
}

void SpatialSceneItem::removeParent()
{
    if(parent)
    {
        parent->removeChild(this);
        parent = nullptr;
    }
}

bool SpatialSceneItem::hasChild(SpatialSceneItem* child, bool recursive) 
{	
    bool result = false;

    if(recursive)
    {
        for(unsigned int i = 0; i < children.size(); ++i)
        {
            if(children[i]->hasChildren())
                result = children[i]->hasChild(child, recursive);
        }
    }
    else
        result = children.containsItem(child);

    return result;
}

void SpatialSceneItem::setTransformDirty()
{
    transformDirty = true;

    for(unsigned int i = 0; i < children.size(); ++i)
        children[i]->setTransformDirty();

    if(!settedForUpdate)
        setForUpdate();
}

}