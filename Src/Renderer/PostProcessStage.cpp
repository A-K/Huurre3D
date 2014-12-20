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

#include "Engine/Engine.h"
#include "Renderer/PostProcessStage.h"
#include "Renderer/Renderer.h"

namespace Huurre3D
{

PostProcessStage::PostProcessStage(Renderer* renderer):
RenderStage(renderer)
{
}

void PostProcessStage::init(const PostProcessStageDescription& postProcessStageDescription)
{
    ViewPort screenViewPort = renderer->getScreenViewPort();
    GraphicSystem* graphicSystem = renderer->getGraphicSystem();

    if(postProcessStageDescription.hasEnvironmentPass)
    {
        //Create environment pass.
        RenderPass environmentPass;
        environmentPass.colorWrite = true;
        environmentPass.depthWrite = false;
        environmentPass.flags = 0;
        environmentPass.renderTargetLayer = 0;
        environmentPass.renderTarget = graphicSystem->getRenderTargetByName("lighting");
        environmentPass.viewPort = screenViewPort;

        ShaderPass environmentShaderPass;
        environmentShaderPass.rasterState = RasterState(BlendState(true, BlendFunction::Add), CompareState(false, CompareFunction::Never), CullState(false, CullFace::Back));
        Texture* skyBoxTexture = graphicSystem->createTexture(TextureTargetMode::TextureCubeMap, TextureWrapMode::ClampEdge, TextureFilterMode::Bilinear, TexturePixelFormat::Rgb8, 0, 0);
        skyBoxTexture->setSlotIndex(TextureSlotIndex::SkyBox);
        environmentShaderPass.textures.pushBack(skyBoxTexture);

        Shader* environmentVert = graphicSystem->createShader(ShaderType::Vertex, postProcessStageDescription.environmentPass.vertexShader);
        Shader* environmentFrag = graphicSystem->createShader(ShaderType::Fragment, postProcessStageDescription.environmentPass.fragmentShader);
        environmentVert->setDefine(ShaderDefineType::UseWorldSpaceParameters);
        environmentShaderPass.program = graphicSystem->createShaderProgram(environmentVert, environmentFrag);
        environmentShaderPass.shaderParameterBlocks.pushBack(graphicSystem->getShaderParameterBlockByName(sp_cameraParameters));
        environmentShaderPass.vertexData = renderer->getFullScreenQuad();

        environmentPass.shaderPasses.pushBack(environmentShaderPass);
        renderPasses.pushBack(environmentPass);
    }

    if(postProcessStageDescription.hasAntiAliasingPass)
    {
        //Create anti-aliasing pass.
        RenderPass antiAliasingRenderPass;
        antiAliasingRenderPass.colorWrite = true;
        antiAliasingRenderPass.depthWrite = false;
        antiAliasingRenderPass.flags = CLEAR_COLOR;
        antiAliasingRenderPass.renderTargetLayer = 0;
        antiAliasingRenderPass.renderTarget = nullptr;
        antiAliasingRenderPass.viewPort = screenViewPort;

        ShaderPass antiAliasingShaderPass;
        Shader* antiAliasingVert = graphicSystem->createShader(ShaderType::Vertex, postProcessStageDescription.antiAliasingPass.vertexShader);
        Shader* antiAliasingFrag = graphicSystem->createShader(ShaderType::Fragment, postProcessStageDescription.antiAliasingPass.fragmentShader);
        antiAliasingFrag->setDefine(ShaderDefineType::FxaaQuality, postProcessStageDescription.antiAliasingPass.fxaaQuality);
        antiAliasingShaderPass.program = graphicSystem->createShaderProgram(antiAliasingVert, antiAliasingFrag);
        antiAliasingShaderPass.vertexData = renderer->getFullScreenQuad();

        antiAliasingShaderPass.shaderParameterBlocks.pushBack(graphicSystem->getShaderParameterBlockByName(sp_cameraParameters));
        antiAliasingShaderPass.shaderParameterBlocks.pushBack(graphicSystem->getShaderParameterBlockByName(sp_renderTargetSize));

        antiAliasingRenderPass.shaderPasses.pushBack(antiAliasingShaderPass);
        renderPasses.pushBack(antiAliasingRenderPass);
    }
}

void PostProcessStage::resizeResources()
{
    ViewPort screenViewPort = renderer->getScreenViewPort();
    GraphicSystem* graphicSystem = renderer->getGraphicSystem();

    for(unsigned int i = 0; i < renderPasses.size(); ++i)
        renderPasses[i].viewPort = screenViewPort;
}

void PostProcessStage::update(const Scene* scene)
{
    Texture* skyBoxTexture = renderer->getGraphicSystem()->getTextureBySlotIndex(TextureSlotIndex::SkyBox);

    if(skyBoxTexture)
    {
        SkyBox* skyBox = (SkyBox*)scene->getSceneItem("SkyBox");
        if(currentSkyBox != skyBox)
        {
            FixedArray<bool, 6> flipImages;
            flipImages.fill(false);
            TextureLoader loader = renderer->getTextureLoader();
            TextureLoadResult result = loader.loadCubeMapFromFile(skyBox->getTextureFiles(), flipImages);

            if(result.pixelData[0] && result.pixelData[1] && result.pixelData[2] && result.pixelData[3] && result.pixelData[4] && result.pixelData[5])
            {
                skyBoxTexture->setPixelFormat(result.format);
                skyBoxTexture->setWidth(result.width);
                skyBoxTexture->setHeight(result.height);
                skyBoxTexture->setData(result);
            }

            loader.releasedata(result);
            currentSkyBox = skyBox;
        }
    }
}

}
