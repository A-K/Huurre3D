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

#include "Renderer/RenderStage.h"
#include "Renderer/Renderer.h"
#include "Graphics/GraphicSystem.h"

namespace Huurre3D
{

RenderStage::RenderStage(Renderer* renderer):
renderer(renderer)
{
}

void RenderStage::init(const JSONValue& renderStageJSON)
{
    auto renderStageRenderPassesJSON = renderStageJSON.getJSONValue("renderPasses");
    if(!renderStageRenderPassesJSON.isNull())
    {
        for(unsigned int i = 0; i < renderStageRenderPassesJSON.getSize(); ++i)
            if(!renderStageRenderPassesJSON.getJSONArrayItem(i).isNull())
                renderPasses.pushBack(createRenderPassFromJson(renderStageRenderPassesJSON.getJSONArrayItem(i)));
    }
}

void RenderStage::resizeResources()
{
    ViewPort screenViewPort = renderer->getScreenViewPort();
    GraphicSystem* graphicSystem = renderer->getGraphicSystem();

    for(unsigned int i = 0; i < renderPasses.size(); ++i)
    {
        renderPasses[i].viewPort = screenViewPort;
        if(renderPasses[i].renderTarget)
            renderPasses[i].renderTarget->setSize(screenViewPort.width, screenViewPort.height);
    }
}

void RenderStage::drawRenderPasses(const Vector<RenderPass>& renderPasses) const
{
    GraphicSystem* graphicSystem = renderer->getGraphicSystem();

    for(unsigned int i = 0; i < renderPasses.size(); ++i)
    {
        RenderPass pass = renderPasses[i];

        if(pass.renderTarget)
        {
            pass.renderTarget->setRenderLayer(pass.renderTargetLayer); 
            graphicSystem->setOffLineRenderTarget(pass.renderTarget);
        }
        else
        {
            graphicSystem->setMainRenderTarget();
        }

        graphicSystem->setViewPort(pass.viewPort);
        graphicSystem->setDepthWrite(pass.depthWrite);
        graphicSystem->setColorWrite(pass.colorWrite);

        if(pass.flags != 0)
            graphicSystem->clear(pass.flags, pass.clearColor);

        for(unsigned int j = 0; j < renderPasses[i].shaderPasses.size(); ++j)
        {
            ShaderPass shaderPass = renderPasses[i].shaderPasses[j];
            graphicSystem->setVertexData(shaderPass.vertexData);
            graphicSystem->setRasterState(shaderPass.rasterState);
            graphicSystem->setShaderProgram(shaderPass.program);

            for(unsigned int k = 0; k < shaderPass.textures.size(); ++k)
                graphicSystem->setTexture(shaderPass.textures[k]);

            for(unsigned int n = 0; n < shaderPass.shaderParameterBlocks.size(); ++n)
                graphicSystem->setShaderParameterBlock(shaderPass.shaderParameterBlocks[n]);

            for(unsigned int m = 0; m < shaderPass.shaderParameters.size(); ++m)
                graphicSystem->setShaderParameter(shaderPass.shaderParameters[m]);

            shaderPass.vertexData->isIndexed() ? graphicSystem->drawIndexed(shaderPass.vertexData->getIndexBuffer()->getNumIndices(), 0) :
                graphicSystem->draw(shaderPass.vertexData->getNumVertices(), 0);
        }
    }
}

RenderPass RenderStage::createRenderPassFromJson(const JSONValue& renderPassJSON)
{
    GraphicSystem* graphicSystem = renderer->getGraphicSystem();

    RenderPass renderPass;
    auto clearColorJSON = renderPassJSON.getJSONValue("clearColor");
    auto colorWriteJSON = renderPassJSON.getJSONValue("colorWrite");
    auto depthWriteJSON = renderPassJSON.getJSONValue("depthWrite");
    auto renderTargetLayerJSON = renderPassJSON.getJSONValue("renderTargetLayer");
    auto flagsJSON = renderPassJSON.getJSONValue("flags");

    if(!clearColorJSON.isNull())
        renderPass.clearColor = clearColorJSON.getVector4();
    if(!colorWriteJSON.isNull())
        renderPass.colorWrite = colorWriteJSON.getBool();
    if(!depthWriteJSON.isNull())
        renderPass.depthWrite = depthWriteJSON.getBool();
    if(!renderTargetLayerJSON.isNull())
        renderPass.renderTargetLayer = renderTargetLayerJSON.getInt();
    if(!flagsJSON.isNull())
    {
        unsigned int flags = 0;
        for(unsigned int i = 0; i < flagsJSON.getSize(); ++i)
        {
            if(flagsJSON.getJSONArrayItem(i).getString().compare("CLEAR_COLOR") == 0)
                flags |= CLEAR_COLOR;
            if(flagsJSON.getJSONArrayItem(i).getString().compare("CLEAR_DEPTH") == 0)
                flags |= CLEAR_DEPTH;
        }
        renderPass.flags = flags;
    }

    auto viewPortJSON = renderPassJSON.getJSONValue("viewPort");
    if(!viewPortJSON.isNull())
    {
        FixedArray<int, 4> viewPort = viewPortJSON.getInt4();
        renderPass.viewPort.set(viewPort[0], viewPort[1], viewPort[2], viewPort[3]);
    }
    else
        renderPass.viewPort = renderer->getScreenViewPort();

    auto renderTargetJSON = renderPassJSON.getJSONValue("renderTarget");
    renderPass.renderTarget = graphicSystem->createRenderTarget(renderTargetJSON);

    auto shaderPasses = renderPassJSON.getJSONValue("shaderPasses");

    if(!shaderPasses.isNull())
    {
        for(unsigned int i = 0; i < shaderPasses.getSize(); ++i)
        {
            auto shaderPassJSON = shaderPasses.getJSONArrayItem(i);
            auto programJSON = shaderPassJSON.getJSONValue("shaderProgram");
            auto vertexDataJSON = shaderPassJSON.getJSONValue("vertexData");
            auto rasterStateJSON = shaderPassJSON.getJSONValue("rasterState");
            auto shaderParameterBlocksJSON = shaderPassJSON.getJSONValue("shaderParameterBlocks");
            auto texturesJSON = shaderPassJSON.getJSONValue("textures");
            ShaderPass shaderPass;

            if(!vertexDataJSON.isNull())
            {
                if(vertexDataJSON.getString().compare("fullScreenQuad") == 0)
                    shaderPass.vertexData = renderer->getFullScreenQuad();
            }
            if(!rasterStateJSON.isNull())
            {
                auto blendFunctionJSON = rasterStateJSON.getJSONValue("blendFunction");
                auto compareFunctionJSON = rasterStateJSON.getJSONValue("compareFunction");
                auto cullFaceJSON = rasterStateJSON.getJSONValue("cullFace");
                auto blendState = !blendFunctionJSON.isNull() ? BlendState(true, enumFromString<BlendFunction>(blendFunctionJSON.getString())) : BlendState(false, BlendFunction::Replace);
                auto compareState = !compareFunctionJSON.isNull() ? CompareState(true, enumFromString<CompareFunction>(compareFunctionJSON.getString())) : CompareState(false, CompareFunction::Never);
                auto cullState = !cullFaceJSON.isNull() ? CullState(true, enumFromString<CullFace>(cullFaceJSON.getString())) : CullState(false, CullFace::Back);
                shaderPass.rasterState = RasterState(blendState, compareState, cullState);
            }

            if(!shaderParameterBlocksJSON.isNull())
            {
                for(unsigned int j = 0; j < shaderParameterBlocksJSON.getSize(); ++j)
                {
                    ShaderParameterBlock* block = graphicSystem->createShaderParameterBlock(shaderParameterBlocksJSON.getJSONArrayItem(j));
                    if(block)
                        shaderPass.shaderParameterBlocks.pushBack(block);
                }
            }
            if(!texturesJSON.isNull())
            {
                for(unsigned int j = 0; j < texturesJSON.getSize(); ++j)
                {
                    Texture* texture = graphicSystem->createTexture(texturesJSON.getJSONArrayItem(j));
                    if(texture)
                        shaderPass.textures.pushBack(texture);
                }
            }

            if(!programJSON.isNull())
                shaderPass.program = graphicSystem->createShaderProgram(programJSON);

            renderPass.shaderPasses.pushBack(shaderPass);
        }
    }
    return renderPass;
}

}