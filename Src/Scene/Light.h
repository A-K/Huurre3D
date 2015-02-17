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

#ifndef Light_H
#define Light_H

#include "Scene/SpatialSceneItem.h"
#include "Math/Vector3.h"
#include "Math/Vector2.h"
#include "Math/Sphere.h"
#include "Util/FixedArray.h"

namespace Huurre3D
{

enum class LightType
{
    Directional = 0,
    Spot,
    Point
};

class Camera;
class Sphere;

class Light : public SpatialSceneItem
{
    SCENEITEM_TYPE(Light);

public:
    Light() = default;
    ~Light() = default;
	
    void setLightType(LightType lightType);
    void setActive(bool active);
    void setRadius(float radius);
    void setFallOffExponent(float exponent);
    void setDirection(const Vector3& direction);
    void updateItem() override;
    void setColor(const Vector3& color);
    void setCastShadow(bool castShadow);
    void setShadowMinDistanceOffset(float offset);
    void setShadowBias(float bias);
    void setShadowOcclusionMask(int mask);
    void setOuterConeAngle(float angle);
    void setInnerConeAngle(float angle);
    void setCascadeSplits(const FixedArray<float, 4>& splits);
    LightType getLightType() const {return lightType;}
    float getRadius() {return radius;}
    float getFallOffExponent() {return fallOffExponent;}
    const Vector3& getDirection() const {return direction;}
    const Sphere& getBoundingSphere() const {return boundingSphere;}
    const Vector3& getColor() const {return color;}
    const FixedArray<float, 4>& getCascadeSplits() const {return splits;}
    int getCascadeSplitsNum() const {return numSplits;}
    bool useAutomatedCascadeSplits() const {return useAutomatedSplits;}
    bool getCastShadow() const {return castShadow;}
    float getShadowMinDistanceOffset() const {return shadowMinDistanceOffset;}
    int getShadowOcclusionMask() const {return shadowOcclusionMask;}
    float getShadowBias() const {return shadowBias;}
    float getOuterConeAngle() const {return outerConeAngle;}
    float getInnerConeAngle() const {return innerConeAngle;}
    Vector2 getConeAngles() const {return Vector2(innerConeAngle, outerConeAngle);}
    Vector2 getConeAngleCosines() const {return Vector2(innerConeAngleCos, outerConeAngleCos);}

private:
    LightType lightType = LightType::Point;
    bool active = true;
    bool castShadow = false;
    Vector3 color = Vector3::ONE;
    Vector3 direction = Vector3::UNIT_Z;
    float fallOffExponent = 1.0f;
    float radius = 50.0f;
    float outerConeAngle;
    float innerConeAngle;
    float outerConeAngleCos;
    float innerConeAngleCos;
    FixedArray<float, 4> splits; //Far clip values of the splits.
    int numSplits;
    int shadowOcclusionMask;
    float shadowBias = 0.0006f;
    //This value determines how far behind the camera the near clip should be moved, 
    //so that all shadow casters which are located behind the camera will be included.
	float shadowMinDistanceOffset = 0.0;
    bool useAutomatedSplits = true;
    Sphere boundingSphere;
};

}

#endif