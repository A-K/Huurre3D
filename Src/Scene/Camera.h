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

#ifndef Camera_H
#define Camera_H

#include "Scene/SpatialSceneItem.h"
#include "Math/Matrix4x4.h"
#include "Math/Frustum.h"

namespace Huurre3D
{

static const float DefaultNearClipDistance = 0.1f;
static const float DefaultFarClipDistance = 1000.0f;
static const float DefaultFov = 45.0f;

class ShaderParameterBlock;

enum class ProjectionType
{
    Perspective,
    Ortographic
};

class Camera : public SpatialSceneItem
{
    SCENEITEM_TYPE(Camera);

public:

    Camera();
    ~Camera() = default;

    void setNearClipDistance(float nearClipDistance);
    void setFarClipDistance(float farClipDistance);
    void setFov(float fov);
    void setAspectRatio(float aspectRatio);
    void setProjectionType(ProjectionType projectionType);
    void setShaderParameterBlock(ShaderParameterBlock* shaderParameterBlock);
    Matrix4x4 getViewProjectionMatrix();
    Frustum getViewFrustumInWorldSpace();
    void getCameraShaderParameterBlock(ShaderParameterBlock* cameraShaderParameterBlock) const;
    void updateItem() override;
    const Frustum& getViewFrustum() const {return viewFrustum;}
    const Matrix4x4& getViewMatrix() const {return getInverseWorldTransform4x4();}
    const Matrix4x4& getProjectionMatrix() const {return projectionMatrix;}
    float getNearClipDistance() const {return nearClipDistance;}
    float getFarClipDistance() const {return farClipDistance;}
    float getFov() const {return fov;}
    float getAspectRatio() const {return aspectRatio;}
    const FixedArray<Vector3, 8>& getViewFrustumCorners() const {return viewFrustumCorners;}

private:
    ProjectionType projectionType = ProjectionType::Perspective;
    Matrix4x4 projectionMatrix = Matrix4x4::IDENTITY;
    FixedArray<Vector3, 8> viewFrustumCorners;
    Frustum viewFrustum;
    float nearClipDistance = DefaultNearClipDistance;
    float farClipDistance = DefaultFarClipDistance;
    float fov = DefaultFov;
    float aspectRatio = 1.0f;
};

}

#endif