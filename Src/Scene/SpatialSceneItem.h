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

#ifndef SpatialSceneItem_H
#define SpatialSceneItem_H

#include "Scene/SceneItem.h"
#include "Util/Vector.h"
#include "Math/Quaternion.h"
#include "Math/Matrix4x4.h"

namespace Huurre3D
{

class Scene;

enum class FrameOfReference
{
    Local,
    World
};

class SpatialSceneItem : public SceneItem
{
public:
    SpatialSceneItem() = default;
    virtual ~SpatialSceneItem() = default;
	
    void setPosition(const Vector3& position);
    void setRotation(const Quaternion& rotation);
    void setScale(const Vector3& scale);
    void setScale(float scale);
    void setTransform(const Vector3& position, const Quaternion& rotation, const Vector3& scale);
    const Vector3& getPosition(FrameOfReference frame);
    const Quaternion& getRotation(FrameOfReference frame);
    const Vector3& getScale(FrameOfReference frame);
    void translate(const Vector3& delta, FrameOfReference frame);
    void rotate(const Quaternion& delta, FrameOfReference frame);
    void pitch(float angle, FrameOfReference frame);
    void yaw(float angle, FrameOfReference frame);
    void roll(float angle, FrameOfReference frame);
    void scale(const Vector3& delta);
    void scale(float delta);
    void lookAt(const Vector3& target, const Vector3& upVector, FrameOfReference frame);
    void updateWorldTransform();
    void addChild(SpatialSceneItem* child);
    void removeChild(SpatialSceneItem* child);
    void removeParent();
    bool hasChild(SpatialSceneItem* child, bool recursive = false);
    SpatialSceneItem* getParent() const {return parent;}
    const Matrix4x4& getWorldTransform4x4() const {return worldTransform;}
    const Matrix4x4& getInverseWorldTransform4x4() const {return inverseWorldTransform;}
    const Vector<SpatialSceneItem*>& getChildren() const {return children;}
    bool hasChildren() const {return !children.empty();}

protected:
    void setTransformDirty();
    SpatialSceneItem* parent = nullptr;
    Vector<SpatialSceneItem*> children;
    Vector3 localPosition = Vector3::ZERO;
    Quaternion localRotation = Quaternion::IDENTITY;
    Vector3 localScale = Vector3::ONE;
    Vector3 worldPosition = Vector3::ZERO;
    Quaternion worldRotation = Quaternion::IDENTITY;
    Vector3 worldScale = Vector3::ONE;
    Matrix4x4 worldTransform = Matrix4x4::IDENTITY;
    Matrix4x4 inverseWorldTransform = Matrix4x4::IDENTITY;
    bool transformDirty = false;
};

}

#endif