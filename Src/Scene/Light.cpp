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

#include "Scene/Light.h"

namespace Huurre3D
{

SCENEITEM_TYPE_IMPL(Light);

void Light::setLightType(LightType lightType)
{
    this->lightType = lightType;

    //Default cascade splits, this means 4, 1, 1 shadow maps for directional, point and spot light respectivelly.
    switch(lightType)
    {
        case LightType::Directional:
            numSplits = 4;
            radius = INF;
            shadowMinDistanceOffset = 100.0f;
            break;
        case LightType::Spot:
            numSplits = 1;
            shadowMinDistanceOffset = 0.0f;
            break;
        case LightType::Point:
            numSplits = 1;
            shadowMinDistanceOffset = 0.0f;
            break;
    }
}

void Light::setActive(bool active)
{
    this->active = active;
}

void Light::setRadius(float radius)
{
    this->radius = radius;
    if(!dirty)
        setDirty();
}

void Light::setFallOffExponent(float exponent)
{
    fallOffExponent = exponent;
}

void Light::setColor(const Vector3& color)
{
    this->color = color;
}

void Light::setCastShadow(bool castShadow)
{
    this->castShadow = castShadow;
}

void Light::setShadowMinDistanceOffset(float offset)
{
    shadowMinDistanceOffset = offset;
}

void Light::setShadowBias(float bias)
{
    shadowBias = bias;
}

void Light::setShadowOcclusionMask(int mask)
{
    shadowOcclusionMask = mask;
}

void Light::setOuterConeAngle(float angle)
{
    outerConeAngle = angle;
    outerConeAngleCos = cos(angle * DEGTORAD);
}

void Light::setInnerConeAngle(float angle)
{
    innerConeAngle = angle;
    innerConeAngleCos = cos(angle * DEGTORAD);
}

void Light::setDirection(const Vector3& direction)
{
    this->direction = direction;
}

void Light::setCascadeSplits(const FixedArray<float, 4>& splits)
{
    this->splits = splits;
    useAutomatedSplits = false;
}

void Light::updateItem()
{
    if(transformDirty)
        updateWorldTransform();

    if(dirty)
    {
        boundingSphere.set(getPosition(FrameOfReference::Local), radius);
        dirty = false;
    }

    settedForUpdate = false;
}

}