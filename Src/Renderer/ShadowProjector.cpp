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

#include "Renderer/ShadowProjector.h"
#include "Scene/Light.h"

namespace Huurre3D
{

ShadowProjector::ShadowProjector()
{
    pointLightDirections[0] = Vector3::UNIT_X; //Right
    pointLightDirections[1] = -Vector3::UNIT_X; //Left
    pointLightDirections[2] = Vector3::UNIT_Y; //Up
    pointLightDirections[3] = -Vector3::UNIT_Y; //Down
    pointLightDirections[4] = Vector3::UNIT_Z; // Back
    pointLightDirections[5] = -Vector3::UNIT_Z; // Forward

    pointLightupVectors[0] = Vector3::UNIT_Y;
    pointLightupVectors[1] = Vector3::UNIT_Y;
    pointLightupVectors[2] = Vector3::UNIT_Z;
    pointLightupVectors[3] = Vector3::UNIT_Z;
    pointLightupVectors[4] = Vector3::UNIT_Y;
    pointLightupVectors[5] = Vector3::UNIT_Y;
}

//Cascaded implementation is based on http://mynameismjp.wordpress.com/2009/02/17/deferred-cascaded-shadow-maps/
void ShadowProjector::calculateDirectionalLightShadowViewProjections(const Vector<Light*>& lights, const ShadowInputParameters& parameters,
    Vector<ShadowDepthData>& shadowDepthDataArray, Vector<ShadowOcclusionData>& shadowOcclusionDataArray) const
{
    for(unsigned int k = 0; k < lights.size(); ++k)
    {
        ShadowDepthData shadowDepthData;
        ShadowOcclusionData shadowOcclusionData;

        int numSplits = clampInt(lights[k]->getCascadeSplitsNum(), 0, MaxSplits);
        FixedArray<float, MaxSplits> splits;

        lights[k]->useAutomatedCascadeSplits() ? splits = calculateCascedeSplits(parameters.camNearClip, parameters.camFarClip, numSplits) : 
            splits = lights[k]->getCascadeSplits();

        //Find light view matrix so that it's looking at the frustum centroid.
        Matrix4x4 lightViewMatrix;
        Vector3 lightDir = lights[k]->getDirection();

        //World up vector should not be parallel with the direction of the light.
        Vector3 up = (lightDir.absDot(Vector3::UNIT_Y) > 0.95) ? Vector3::UNIT_Z : Vector3::UNIT_Y;
        lightViewMatrix.setLookAt(parameters.frustumCentroid - (lightDir * parameters.distFromCentroid), parameters.frustumCentroid, up);
        float nearClipOffset = lights[k]->getShadowMinDistanceOffset();

        for(int j = 0; j < numSplits; ++j)
        {
            Vector3 splitFrustumViewSpaceCornerNear;
            Vector3 splitFrustumViewSpaceCornersFar;
            FixedArray<Vector3, 8> splitFrustumWorldSpaceCorners;
            FixedArray<Vector3, 8> splitFrustumLightSpaceCorners;
            Matrix4x4 lightProjectionMatrix;
            BoundingBox lightSpaceBounds;

            //the first value in splits is the far plane of the first cascade split, 
            //but the camera's near plane is needed in the first iteration.
            float minZ = (j == 0) ? parameters.camNearClip : splits[j - 1];
            float maxZ = splits[j];

            for(int i = 0; i < 4; i++)
            {
                //Define the corner points of the current view space frustum split by scaling the view frustum.
                splitFrustumViewSpaceCornerNear = parameters.camViewSpaceCorners[i + 4] * (minZ / parameters.camFarClip);
                splitFrustumViewSpaceCornersFar = parameters.camViewSpaceCorners[i + 4] * (maxZ / parameters.camFarClip);

                //Transform the frustum slice corners into world space
                splitFrustumWorldSpaceCorners[i] = (parameters.inverseViewMatrix * Vector4(splitFrustumViewSpaceCornerNear, 1.0f)).xyz();
                splitFrustumWorldSpaceCorners[i + 4] = (parameters.inverseViewMatrix * Vector4(splitFrustumViewSpaceCornersFar, 1.0f)).xyz();

                //Transform the frustum slice corners into light space.
                splitFrustumLightSpaceCorners[i] = (lightViewMatrix * Vector4(splitFrustumWorldSpaceCorners[i], 1.0f)).xyz();
                splitFrustumLightSpaceCorners[i +4] = (lightViewMatrix * Vector4(splitFrustumWorldSpaceCorners[i + 4], 1.0f)).xyz();
            }

            //Find the enclosing bounds.
            lightSpaceBounds.mergePoints(splitFrustumLightSpaceCorners.data(), splitFrustumLightSpaceCorners.size());
            Vector3 minCorner = lightSpaceBounds.getMin();
            Vector3 maxCorner = lightSpaceBounds.getMax();
			
            //Find the world space size of a texel, and use it for texel snapping to prevent shadow jitter and crawling.
            float diagonal = (splitFrustumWorldSpaceCorners[0] - splitFrustumWorldSpaceCorners[7]).length() + 2;
            float worldsUnitsPerTexel = diagonal / shadowMapSize;

            Vector3 offset = (Vector3(diagonal, diagonal, diagonal) - (maxCorner - minCorner)) * 0.5f;
            maxCorner += offset;
            minCorner -= offset;

            minCorner /= worldsUnitsPerTexel;
            minCorner = floor(minCorner);
            minCorner *= worldsUnitsPerTexel;

            maxCorner /= worldsUnitsPerTexel;
            maxCorner = floor(maxCorner);
            maxCorner *= worldsUnitsPerTexel;

            lightProjectionMatrix.setOrtho(minCorner.x, maxCorner.x, minCorner.y, maxCorner.y, -maxCorner.z - nearClipOffset, -minCorner.z);

            shadowDepthData.shadowViewProjectionMatrices[j] = lightProjectionMatrix * lightViewMatrix;
            shadowOcclusionData.viewToLightViewProjMatrices[j] = shadowDepthData.shadowViewProjectionMatrices[j] * parameters.inverseViewMatrix;
        }

        shadowDepthData.numSplits = numSplits;
        shadowDepthDataArray.pushBack(shadowDepthData);

        shadowOcclusionData.farDistances = splits;
        shadowOcclusionData.lightType = static_cast<int>(LightType::Directional);
        shadowOcclusionData.shadowOcclusionMask = lights[k]->getShadowOcclusionMask();
        shadowOcclusionData.shadowBias = lights[k]->getShadowBias();
        shadowOcclusionDataArray.pushBack(shadowOcclusionData);
    }
}

void ShadowProjector::calculateSpotLightShadowViewProjections(const Vector<Light*>& lights, const ShadowInputParameters& parameters,
    Vector<ShadowDepthData>& shadowDepthDataArray, Vector<ShadowOcclusionData>& shadowOcclusionDataArray) const
{
    Matrix4x4 lightViewMatrix;
    Matrix4x4 lightProjectionMatrix;

    //TODO: because the shadow map array has six textures, six spot lights can be processed in one pass. 
    //Needs minor modifications to the shadow depth and occlusion shaders
    for(unsigned int i = 0; i < lights.size(); ++i)
    {
        ShadowDepthData shadowDepthData;
        ShadowOcclusionData shadowOcclusionData;

        float radius = lights[i]->getRadius();
        Vector3 worldPos = lights[i]->getPosition(FrameOfReference::World);
        Quaternion lightRot = lights[i]->getRotation(FrameOfReference::World);
        Vector3 lightDir = lights[i]->getDirection();
        Vector3 target = worldPos + lightRot.rotate(lightDir) * radius;
				
        //World up vector should not be parallel with the direction of the light.
        Vector3 up = (lightDir.absDot(Vector3::UNIT_Y) > 0.95) ? Vector3::UNIT_Z : Vector3::UNIT_Y;

        lightViewMatrix.setLookAt(worldPos, target, up);
        lightProjectionMatrix.setPerspective(lights[i]->getOuterConeAngle(), 1.0f, radius * 0.005f, radius);

        shadowDepthData.shadowViewProjectionMatrices[0] = lightProjectionMatrix * lightViewMatrix;
        shadowDepthData.numSplits = 1;
        shadowDepthDataArray.pushBack(shadowDepthData);

        shadowOcclusionData.viewToLightViewProjMatrices[0] = shadowDepthData.shadowViewProjectionMatrices[0] * parameters.inverseViewMatrix;
        shadowOcclusionData.lightType = static_cast<int>(LightType::Spot);
        shadowOcclusionData.shadowOcclusionMask = lights[i]->getShadowOcclusionMask(); 
        shadowOcclusionData.shadowBias = lights[i]->getShadowBias();
        shadowOcclusionDataArray.pushBack(shadowOcclusionData);
    }
}

void ShadowProjector::calculatePointLightShadowViewProjections(const Vector<Light*>& lights, const ShadowInputParameters& parameters,
    Vector<ShadowDepthData>& shadowDepthDataArray, Vector<ShadowOcclusionData>& shadowOcclusionDataArray) const
{
    Matrix4x4 lightViewMatrix;
    Matrix4x4 lightProjectionMatrix;
    for(unsigned int i = 0; i < lights.size(); ++i)
    {
        ShadowDepthData shadowDepthData;
        ShadowOcclusionData shadowOcclusionData;

        Vector3 lightWorldPos = lights[i]->getPosition(FrameOfReference::World);
        float radius = lights[i]->getRadius();

        for(int j = 0; j < NumCubeMapFaces; ++j)
        {
            Vector3 target = lightWorldPos + pointLightDirections[j] * radius;
            lightViewMatrix.setLookAt(lightWorldPos, target, pointLightupVectors[j]);
            lightProjectionMatrix.setPerspective(90.0f, 1.0f, radius * 0.005f, radius);

            shadowDepthData.shadowViewProjectionMatrices[j] = lightProjectionMatrix * lightViewMatrix;
            shadowOcclusionData.viewToLightViewProjMatrices[j] = shadowDepthData.shadowViewProjectionMatrices[j] * parameters.inverseViewMatrix;
        }

        shadowDepthData.numSplits = NumCubeMapFaces;
        shadowDepthDataArray.pushBack(shadowDepthData);

        shadowOcclusionData.farDistances[0] = lightWorldPos.x;
        shadowOcclusionData.farDistances[1] = lightWorldPos.y;
        shadowOcclusionData.farDistances[2] = lightWorldPos.z;
        shadowOcclusionData.lightType = static_cast<int>(LightType::Point);
        shadowOcclusionData.shadowOcclusionMask = lights[i]->getShadowOcclusionMask();
        shadowOcclusionData.shadowBias = lights[i]->getShadowBias();
        shadowOcclusionDataArray.pushBack(shadowOcclusionData);
    }
}

FixedArray<float, MaxSplits> ShadowProjector::calculateCascedeSplits(float near, float far, int numSplits) const
{ 
    FixedArray<float, MaxSplits> cascadeSplits;
    cascadeSplits[numSplits - 1] = far;
    float splitWeight = 0.65f;

    for(int i = 0; i < numSplits - 1; ++i)
    {
        float si = i / static_cast<float>(numSplits);
        cascadeSplits[i] = splitWeight * near * pow(far / near, si) + (1.0f - splitWeight) * ((near + si) * (far - near));
    }

    return cascadeSplits;
}

}