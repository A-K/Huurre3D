//
// Copyright (c) 2013-2014 Antti Karhu.
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

#include "Scene/Camera.h"
#include "Graphics/ShaderParameterBlock.h"

namespace Huurre3D
{

SCENEITEM_TYPE_IMPL(Camera);

Camera::Camera()
{
    projectionMatrix.setPerspective(fov, aspectRatio, nearClipDistance, farClipDistance);
}

void Camera::setNearClipDistance(float nearClipDistance)
{
    this->nearClipDistance = nearClipDistance;   
    if(!dirty)
        setDirty();
}

void Camera::setFarClipDistance(float farClipDistance)
{
    this->farClipDistance = farClipDistance;
    if(!dirty)
        setDirty();
}

void Camera::setFov(float fov)
{
    this->fov = fov;
    if(!dirty)
        setDirty();
}

void Camera::setAspectRatio(float aspectRatio)
{
    this->aspectRatio = aspectRatio;
    if(!dirty)
        setDirty();
}

void Camera::setProjectionType(ProjectionType projectionType)
{
    this->projectionType = projectionType;
    if(!dirty)
        setDirty();
}

Matrix4x4 Camera::getViewProjectionMatrix()
{ 
    return projectionMatrix * getViewMatrix(); 
}

Frustum Camera::getViewFrustumInWorldSpace()
{
    //ViewProjection -> frustum planes in world sapce.
    return Frustum(getViewProjectionMatrix().transpose());
}

void Camera::getCameraShaderParameterBlock(ShaderParameterBlock* cameraShaderParameterBlock)
{
    if(cameraShaderParameterBlock)
    {
        //Frustum far plane corners have to be in the same order as the triangle fan vertices which is used in fullscreen quad.
        Matrix4x4 frustumFarPlaneCorners = Matrix4x4(Vector4(viewFrustumCorners[4], 1.0f),  //Upper Left
                                                     Vector4(viewFrustumCorners[6], 1.0f), //Bottom Left
                                                     Vector4(viewFrustumCorners[5], 1.0f),   //Upper Right
                                                     Vector4(viewFrustumCorners[7], 1.0f)); //Bottom Right

        cameraShaderParameterBlock->clearBuffer();
        cameraShaderParameterBlock->addParameter(getViewMatrix());
        cameraShaderParameterBlock->addParameter(projectionMatrix);
        cameraShaderParameterBlock->addParameter(frustumFarPlaneCorners);
        cameraShaderParameterBlock->addParameter(nearClipDistance);
        cameraShaderParameterBlock->addParameter(farClipDistance);
    }
}

void Camera::updateItem()
{
    if(transformDirty)
        updateWorldTransform();

    if(dirty)
    {
        switch(projectionType)
        {
            case ProjectionType::Perspective:
                projectionMatrix.setPerspective(fov, aspectRatio, nearClipDistance, farClipDistance);
                break;
            case ProjectionType::Ortographic:
                float top = nearClipDistance * tan(fov * DEGTORAD * 0.5f);
                float bottom = -top;
                float left = bottom * aspectRatio;
                float right = top * aspectRatio;
                projectionMatrix.setOrtho(left, right, bottom, top, nearClipDistance, farClipDistance);
                break;
        }

        viewFrustum.set(projectionMatrix.transpose());
        viewFrustumCorners = viewFrustum.getCorners();
        dirty = false;
    }

    settedForUpdate = false;
}

}