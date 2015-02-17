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

#include "Renderer/LightTileGrid.h"
#include "Scene/Camera.h"
#include "Scene/Light.h"

namespace Huurre3D
{

LightTileGrid::~LightTileGrid()
{
    delete[] tiles;
}

void LightTileGrid::setGridDimensions(int tileWidth, int tileHeight, int screenWidth, int screenHeight)
{
    numTiles = 0;
    gridDimensions.tileHeight = tileWidth;
    gridDimensions.tileWidth = tileHeight;
    gridDimensions.widthResolution = (screenWidth + tileWidth - 1) / tileWidth;
    gridDimensions.heightResolution = (screenHeight + tileHeight - 1) / tileHeight;
    float widthOffset = float(tileWidth) / float(screenWidth);
    float heightOffset = float(tileHeight) / float(screenHeight);
    float left = 0.0f;
    float bottom = 0.0f;

    if(tiles)
        delete[] tiles;

    tiles = new Tile[gridDimensions.widthResolution * gridDimensions.heightResolution];
    tileLightInfo = Vector<int>(MaXNumLights * gridDimensions.widthResolution * gridDimensions.heightResolution);

    for(int i = 0 ; i < gridDimensions.heightResolution; ++i)
    {
        bottom = 2.0f * i * heightOffset -1.0f;
        for(int j = 0; j < gridDimensions.widthResolution; ++j)
        {	
            left = 2.0f * j * widthOffset -1.0f;
            tiles[i * gridDimensions.widthResolution + j].rect.set(Vector2(left, bottom), Vector2(left + widthOffset, bottom + heightOffset));
            tiles[i * gridDimensions.widthResolution + j].numLights = 0;
            numTiles++;
        }
    }
}

void LightTileGrid::binLightsToTiles(const Vector<Light*>& lights, Camera* camera)
{
    lightParameterBlockValues.clear();
    tileLightInfo.fill(-1);

    for(int i = 0; i < numTiles; ++i)
        tiles[i].numLights = 0;

    int numLights = lights.size();
    Matrix4x4 viewMat = camera->getViewMatrix();
    Matrix4x4 projMat = camera->getProjectionMatrix();
    Rect lightScreenSpaceAABB;
    Quaternion viewRot;
    Vector3 lightViewPosition;
    float lightRadius;
    float castShadow;
    float shadowOcclusionMask = 0.0f;
    Sphere viewSpaceBoundingSphere;

    lightParameterBlockValues.pushBack(Vector4(static_cast<float>(numLights), 0.0f, 0.0f, 0.0f));
    FixedArray<Vector4, 4> lightParameterValue;

    for(int j = 0; j < numLights; ++j)
    {
        castShadow = lights[j]->getCastShadow() ? 1.0f : 0.0f;
        shadowOcclusionMask = static_cast<float>(lights[j]->getShadowOcclusionMask());

        switch(lights[j]->getLightType())
        {
            case LightType::Directional:
                viewRot = camera->getRotation(FrameOfReference::World).inverse();
                lightParameterValue[0] = Vector4::ZERO;
                lightParameterValue[1] = Vector4(lights[j]->getColor(), 0.0f);
                lightParameterValue[2] = Vector4(viewRot.rotate(lights[j]->getDirection()), float(LightType::Directional));
                lightParameterValue[3] = Vector4(0.0f, 0.0f, castShadow, shadowOcclusionMask);
                lightScreenSpaceAABB = Rect(-1.0, -1.0, 1.0, 1.0);
                break;

            case LightType::Spot:
                viewRot = camera->getRotation(FrameOfReference::World).inverse();
                lightViewPosition = (viewMat * Vector4(lights[j]->getPosition(FrameOfReference::World), 1.0f)).xyz();
                lightRadius = lights[j]->getRadius();
                lightParameterValue[0] = Vector4(lightViewPosition, lightRadius);
                lightParameterValue[1] = Vector4(lights[j]->getColor(), lights[j]->getFallOffExponent());
                lightParameterValue[2] = Vector4(viewRot.rotate(lights[j]->getDirection()), float(LightType::Spot));
                lightParameterValue[3] = Vector4(lights[j]->getConeAngleCosines(), Vector2(castShadow, shadowOcclusionMask));
                viewSpaceBoundingSphere = Sphere(lightViewPosition, lightRadius);
                lightScreenSpaceAABB = BoundingBox(viewSpaceBoundingSphere).project2D(projMat);
                break;

            case LightType::Point:
                lightViewPosition = (viewMat * Vector4(lights[j]->getPosition(FrameOfReference::World), 1.0f)).xyz();
                lightRadius = lights[j]->getRadius();
                lightParameterValue[0] = Vector4(lightViewPosition, lightRadius);
                lightParameterValue[1] = Vector4(lights[j]->getColor(), lights[j]->getFallOffExponent());
                lightParameterValue[2] = Vector4(0.0f, 0.0f, 0.0f, float(LightType::Point));
                lightParameterValue[3] = Vector4(0.0f, 0.0f, castShadow, shadowOcclusionMask);
                viewSpaceBoundingSphere = Sphere(lightViewPosition, lightRadius);
                lightScreenSpaceAABB = BoundingBox(viewSpaceBoundingSphere).project2D(projMat);
                break;
        }

        lightParameterBlockValues.pushBack(lightParameterValue.data(), lightParameterValue.size());

        //Check which tiles this light affects.
        for(int i = 0; i < numTiles; ++i)
        {
            if(tiles[i].rect.overlap(lightScreenSpaceAABB))
            {
                tileLightInfo[i * numLights + tiles[i].numLights] = j;
                tiles[i].numLights++;
            }
        }
    }
}

}