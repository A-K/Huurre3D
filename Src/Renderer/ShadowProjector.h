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

#ifndef ShadowProjector_H
#define ShadowProjector_H

#include "Scene/Camera.h"
#include "Graphics/Texture.h"

namespace Huurre3D
{

class Light;

static const unsigned int MaxSplits = 4;

struct ShadowDepthData
{
    FixedArray<Matrix4x4, NumCubeMapFaces> shadowViewProjectionMatrices;
    int numSplits;
};

struct ShadowOcclusionData
{
    FixedArray<Matrix4x4, NumCubeMapFaces> viewToLightViewProjMatrices;
    FixedArray<float, MaxSplits> farDistances;
    int lightType;
    int shadowOcclusionMask;
    float shadowBias;
    float padding;
};

struct ShadowInputParameters
{
    float camFarClip;
    float camNearClip;
    float distFromCentroid;
    Vector3 frustumCentroid;
    FixedArray<Vector3, 8> camViewSpaceCorners;
    Matrix4x4 inverseViewMatrix;

    ShadowInputParameters(const Camera* camera)
    {
        camFarClip = camera->getFarClipDistance();
        camNearClip = camera->getNearClipDistance();
        camViewSpaceCorners = camera->getViewFrustumCorners();
        inverseViewMatrix = camera->getWorldTransform4x4();

        //Calculate the frustum centroid in world space
        frustumCentroid = Vector3::ZERO;
        for(unsigned int i = 0; i < 8; ++i)
            frustumCentroid += (inverseViewMatrix * Vector4(camViewSpaceCorners[i], 1.0)).xyz();
		 
        frustumCentroid /= 8;
        distFromCentroid = camFarClip - camNearClip;
    }
};

class ShadowProjector
{
public:
    ShadowProjector();
    ~ShadowProjector() = default;
    void calculateDirectionalLightShadowViewProjections(const Vector<Light*>& lights, const ShadowInputParameters& parameters,
        Vector<ShadowDepthData>& shadowDepthDataArray, Vector<ShadowOcclusionData>& shadowOcclusionDataArray) const;
	void calculateSpotLightShadowViewProjections(const Vector<Light*>& lights, const ShadowInputParameters& parameters,
        Vector<ShadowDepthData>& shadowDepthDataArray, Vector<ShadowOcclusionData>& shadowOcclusionDataArray) const;
    void calculatePointLightShadowViewProjections(const Vector<Light*>& lights, const ShadowInputParameters& parameters,
        Vector<ShadowDepthData>& shadowDepthDataArray, Vector<ShadowOcclusionData>& shadowOcclusionDataArray) const;
    void setShadowMapSize(float size) {shadowMapSize = size;}

private:
    //Calculate the cascade splits so that each successive split is larger than the previous.
    FixedArray<float, MaxSplits> calculateCascedeSplits(float near, float far, int numSplits) const;
    FixedArray<Vector3, NumCubeMapFaces> pointLightDirections;
    FixedArray<Vector3, NumCubeMapFaces> pointLightupVectors;
    float shadowMapSize;
};

}

#endif