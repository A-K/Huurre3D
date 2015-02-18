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

#include "Renderer/LightingStage.h"
#include "Renderer/Renderer.h"
#include "Scene/Light.h"
#include "Scene/Camera.h"
#include "Graphics/ShaderParameterBlock.h"

namespace Huurre3D
{

LightingStage::LightingStage(Renderer& renderer):
RenderStage(renderer)
{
}

void LightingStage::init(const JSONValue& lightingStageJSON)
{
    auto lightTileWidthJSON = lightingStageJSON.getJSONValue("lightTileWidth");
    auto lightTileHeightJSON = lightingStageJSON.getJSONValue("lightTileHeight");
    auto lightingStageRenderPassesJSON = lightingStageJSON.getJSONValue("renderPasses");
    if(!lightingStageRenderPassesJSON.isNull())
    {
        for(unsigned int i = 0; i < lightingStageRenderPassesJSON.getSize(); ++i)
            if(!lightingStageRenderPassesJSON.getJSONArrayItem(i).isNull())
                renderPasses.pushBack(createRenderPassFromJson(lightingStageRenderPassesJSON.getJSONArrayItem(i)));
    }
    
    ViewPort screenViewPort = renderer.getScreenViewPort();
    GraphicSystem& graphicSystem = renderer.getGraphicSystem();

    tileGrid.setGridDimensions(lightTileWidthJSON.getInt(), lightTileHeightJSON.getInt(), screenViewPort.width, screenViewPort.height);
    auto gridDimensions = tileGrid.getGridDimensions();
    MemoryBuffer gridDimensionsData;
    gridDimensionsData.bufferData(&gridDimensions, sizeof(gridDimensions));
    ShaderParameterBlock* lightGridParametersBlock = graphicSystem.getShaderParameterBlockByName(sp_lightGridParameters);
    lightGridParametersBlock->setParameterData(std::move(gridDimensionsData));

}

void LightingStage::resizeResources()
{
    ViewPort screenViewPort = renderer.getScreenViewPort();

    for(unsigned int i = 0; i < renderPasses.size(); ++i)
    {
        renderPasses[i].viewPort = screenViewPort;
        renderPasses[i].renderTarget->setSize(screenViewPort.width, screenViewPort.height);
    }

    //Resize the light grid.
    auto gridDimensions = tileGrid.getGridDimensions();
    tileGrid.setGridDimensions(gridDimensions.tileWidth, gridDimensions.tileHeight, screenViewPort.width, screenViewPort.height);

    //Resize the light grid dimensions shader parameter block.
    auto newGridDimensions = tileGrid.getGridDimensions();
    MemoryBuffer newGridDimensionsData;
    newGridDimensionsData.bufferData(&newGridDimensions, sizeof(newGridDimensions));
    ShaderParameterBlock* lightGridParametersBlock = renderer.getGraphicSystem().getShaderParameterBlockByName(sp_lightGridParameters);
    lightGridParametersBlock->setParameterData(std::move(newGridDimensionsData));
}

void LightingStage::update(const Scene& scene)
{
    Camera* camera = scene.getMainCamera();
    Frustum worldSpaceCameraViewFrustum = camera->getViewFrustumInWorldSpace();
    cullLights(scene, frustumLights, worldSpaceCameraViewFrustum);
    Vector3 globalAmbientLight = scene.getGlobalAmbientLight();

    //Bin lights to tiles.
    tileGrid.binLightsToTiles(frustumLights, camera);
    
    auto gridDimensions = tileGrid.getGridDimensions();
    auto lightParameterBlock = tileGrid.getLightParameterBlockValues();

    //Set the global ambient light into the parameter
    lightParameterBlock[0].y = globalAmbientLight.x;
    lightParameterBlock[0].z = globalAmbientLight.y;
    lightParameterBlock[0].w = globalAmbientLight.z;

    GraphicSystem& graphicSystem = renderer.getGraphicSystem();

    //Update the lights parameter block in Tiled deferred shader pass.
    ShaderParameterBlock* lightParameters = graphicSystem.getShaderParameterBlockByName(sp_lightParameters);
    lightParameters->clearParameters();
    lightParameters->setParameterData(lightParameterBlock.getMemoryBuffer());

    //Update the light info texture in Tiled deferred shader pass.
    int width = frustumLights.size();;
    int height = gridDimensions.widthResolution * gridDimensions.heightResolution;
    int size = sizeof(int) * width * height;
    Texture* lightInfoTexture = graphicSystem.getTextureBySlotIndex(TextureSlotIndex::TileLightInfo);
    lightInfoTexture->setSize(width, height);
    lightInfoTexture->setPixelData(tileGrid.getTileLightInfo());
}

void LightingStage::clearStage()
{
    frustumLights.clear();
}

}