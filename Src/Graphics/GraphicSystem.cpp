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

#include "Graphics/GraphicSystem.h"
#include <iostream>

namespace Huurre3D
{

GraphicSystem::GraphicSystem()
{
    graphicSystemBackEnd = CreateGraphicSystemBackEnd();
}

GraphicSystem::~GraphicSystem()
{
    clearVertexDatas();
    clearAttributeBuffers();
    clearVertexStreams();
    clearIndexBuffers();
    clearShaderPrograms();
    clearShaders();
    clearTextures();
    clearRenderTargets();
    clearShaderParameterBlocks();
    delete  graphicSystemBackEnd; 
}

VertexData* GraphicSystem::createVertexData(PrimitiveType primitiveType, int numVertices)
{
    VertexData* vertexData = new VertexData(this, primitiveType, numVertices);
    unsigned int id = graphicSystemBackEnd->createVertexData();
    vertexDataComponents.pushBack(vertexData);
    vertexData->setId(id);
    return vertexData;
}

AttributeBuffer* GraphicSystem::createAttributeBuffer(AttributeType type, AttributeSemantic semantic, int numComponentsPerVertex, bool normalized, bool dynamic)
{
    AttributeBuffer* attributeBuffer = new AttributeBuffer(this, type, semantic, numComponentsPerVertex, normalized, dynamic);
    unsigned int id;
    dynamic ? id = graphicSystemBackEnd->createBuffer() : id = -1;
    attributeBuffers.pushBack(attributeBuffer);
    attributeBuffer->setId(id);
    return attributeBuffer;
}

VertexStream* GraphicSystem::createVertexStream(int numVertices, bool interleaved)
{
    VertexStream* vertexStream = new VertexStream(this, numVertices);
    unsigned int id;
    interleaved ? id = graphicSystemBackEnd->createBuffer() : id = -1;
    vertexStreams.pushBack(vertexStream);
    vertexStream->setId(id);
    return vertexStream;
}

IndexBuffer* GraphicSystem::createIndexBuffer(IndexType indexType, int numIndices, bool dynamic)
{
    IndexBuffer* indexBuffer = new IndexBuffer(this, indexType, numIndices, dynamic);
    unsigned int id = graphicSystemBackEnd->createBuffer();
    indexBuffers.pushBack(indexBuffer);
    indexBuffer->setId(id);
    return indexBuffer;
}

Shader* GraphicSystem::createShader(ShaderType shaderType, const std::string& sourceFileName)
{
    Shader* shader = new Shader(this, shaderType, sourceFileName.c_str());
    shaders.pushBack(shader);
    return shader;
}

ShaderProgram* GraphicSystem::createShaderProgram(Shader* vertexShader, Shader* fragmentShader)
{
    ShaderProgram* shaderProgram = new ShaderProgram(this, vertexShader, fragmentShader);
    unsigned int id = graphicSystemBackEnd->createShaderProgram();
    shaderPrograms.pushBack(shaderProgram);
    shaderProgram->setId(id);

    Vector<Shader*> shaderComp;
    shaderComp.pushBack(vertexShader);
    shaderComp.pushBack(fragmentShader);
    unsigned int combinationTag = generateShaderCombinationTag(shaderComp);
    shaderProgram->setShaderCombinationTag(combinationTag);
	
    return shaderProgram;
}

Texture* GraphicSystem::createTexture(TextureTargetMode targetMode, TextureWrapMode wrapMode, TextureFilterMode filterMode, TexturePixelFormat pixelFormat, int width, int height)
{
    Texture* texture = new Texture(this, targetMode, wrapMode, filterMode, pixelFormat, width, height);
    unsigned int id = graphicSystemBackEnd->createTexture();
    textures.pushBack(texture);
    texture->setId(id);
    return texture;
}

RenderTarget* GraphicSystem::createRenderTarget(int width, int height, bool depthBuffer, int numBuffers, int numLayers)
{
    RenderTarget* renderTarget = new RenderTarget(this, width, height, depthBuffer, numBuffers, numLayers);
    unsigned int id = graphicSystemBackEnd->createRenderTarget(width, height, depthBuffer, numBuffers, numLayers);
    renderTargets.pushBack(renderTarget);
    renderTarget->setId(id);
    return renderTarget;
}

ShaderParameterBlock* GraphicSystem::createShaderParameterBlock(const std::string& name)
{
    ShaderParameterBlock* shaderParameterBlock = new ShaderParameterBlock(this, name);
    unsigned int id = graphicSystemBackEnd->createBuffer();
    shaderParameterBlocks.pushBack(shaderParameterBlock);
    shaderParameterBlock->setId(id);
    return shaderParameterBlock;
}

void GraphicSystem::setVertexData(VertexData* vertexData)
{
    if(vertexData && currentVertexData != vertexData)
    {
        graphicSystemBackEnd->setVertexData(vertexData);
        currentVertexData = vertexData;
    }
}

void GraphicSystem::setShaderProgram(ShaderProgram* program)
{
    if(program && currentShaderProgram != program)
    {
        graphicSystemBackEnd->setShaderProgram(program);
        currentShaderProgram = program;
    }
}

void GraphicSystem::setShaderParameter(const ShaderParameter& shaderParameter)
{
    if(currentShaderProgram)
    {
        ShaderParameterDescription* parameterDesc = currentShaderProgram->getShaderParameterDescription(shaderParameter.nameHash);

        if(parameterDesc)
            graphicSystemBackEnd->setShaderParameter(parameterDesc, shaderParameter);
        else
            std::cout << "Currently active shader program does not have parameter name " << shaderParameter.name << std::endl;
    }
}

void GraphicSystem::setShaderParameterBlock(ShaderParameterBlock* block)
{
    if(currentShaderProgram /*&& currentShaderParameterBlock != block*/)
    {
        ShaderParameterBlockDescription* paramBlockDesc = currentShaderProgram->getShaderParameterBlockDescription(block->getNameHash());

        if(paramBlockDesc)
            graphicSystemBackEnd->setShaderParameterBlock(paramBlockDesc, block);
        else
             std::cout << "Currently active shader program does not have parameter block name " << block->getName() << std::endl;
    }
}

void GraphicSystem::setTexture(Texture* texture)
{
    if(texture)
        graphicSystemBackEnd->setTexture(texture);   
}

void GraphicSystem::setDepthWrite(bool enable)
{
    if(depthWriteEnabled != enable)
    {
        graphicSystemBackEnd->setDepthWrite(enable);
        depthWriteEnabled = enable;
    }
}

void GraphicSystem::setColorWrite(bool enable)
{
    if(colorWriteEnabled != enable)
    {
        graphicSystemBackEnd->setColorWrite(enable);
        colorWriteEnabled = enable;
    }
}

void GraphicSystem::setRasterState(const RasterState& state)
{
    if(currentRasterState != state)
    {
        graphicSystemBackEnd->setRasterState(state);
        currentRasterState = state;
    }
}

void GraphicSystem::setOffLineRenderTarget(RenderTarget* renderTarget)
{
    if(renderTarget)
        graphicSystemBackEnd->setOffLineRenderTarget(renderTarget);
}

void GraphicSystem::setMainRenderTarget()
{
    graphicSystemBackEnd->setMainRenderTarget();
}

void GraphicSystem::setViewPort(const ViewPort& viewPort)
{
    if(currentViewPort != viewPort)
    {
        graphicSystemBackEnd->setViewPort(viewPort);
        currentViewPort = viewPort;
    }
}

void GraphicSystem::clear(unsigned int flags, const Vector4& color)
{
    graphicSystemBackEnd->clear(flags, color);
}

void GraphicSystem::draw(int numVertices, int vertexOffset)
{
    if(currentVertexData)
        graphicSystemBackEnd->draw(numVertices, vertexOffset);
    else
        std::cout << "Failed to render: Active vertex data is not set" << std::endl;
}

void GraphicSystem::drawIndexed(int numIndices, int indexOffset)
{
    if(currentVertexData)
    {
        if(currentVertexData->isIndexed() && currentVertexData->getIndexBuffer())
            graphicSystemBackEnd->drawIndexed(numIndices, indexOffset);
        else
            std::cout << "Failed to render: Active vertex data is not indexed or index buffer is not set." << std::endl;
    }
    else
    {
        std::cout << "Failed to render: Active vertex data is not set." << std::endl;
    }
}

void GraphicSystem::drawInstanced(int numIndices, int indexOffset, int instancesCount)
{
    graphicSystemBackEnd->drawInstanced(numIndices, indexOffset, instancesCount);
}

void GraphicSystem::removeVertexData(VertexData* vertexData)
{
    if(vertexData)
    {
        vertexDataComponents.eraseUnordered(vertexData);
        graphicSystemBackEnd->removeVertexData(vertexData->getId());
        delete vertexData;
        vertexData = nullptr;
    }
}

void GraphicSystem::removeAttributeBuffer(AttributeBuffer* attributeBuffer)
{
    if(attributeBuffer)
    {
        unsigned int id = attributeBuffer->getId();
        attributeBuffers.eraseUnordered(attributeBuffer);
        if(id != -1)
            graphicSystemBackEnd->removeBuffer(id);

        delete attributeBuffer;
        attributeBuffer = nullptr;
    }
}

void GraphicSystem::removeVertexStream(VertexStream* vertexStream)
{
    if(vertexStream)
    {
        unsigned int id = vertexStream->getId();
        vertexStreams.eraseUnordered(vertexStream);
        if(id != -1)
            graphicSystemBackEnd->removeBuffer(id);

        delete vertexStream;
        vertexStream = nullptr;
    }
}

void GraphicSystem::removeIndexBuffer(IndexBuffer* indexBuffer)
{
    if(indexBuffer)
    {
        indexBuffers.eraseUnordered(indexBuffer);
        graphicSystemBackEnd->removeBuffer(indexBuffer->getId());
        delete indexBuffer;
        indexBuffer = nullptr;
    }
}

void GraphicSystem::removeShaderProgram(ShaderProgram* program)
{
    if(program)
    {
        shaderPrograms.eraseUnordered(program);
        graphicSystemBackEnd->removeShaderProgram(program->getId());
        delete program;
        program = nullptr;
    }
}

void GraphicSystem::removeShader(Shader* shader)
{
    if(shader)
    {
        shaders.eraseUnordered(shader);
        delete shader;
        shader = nullptr;
    }
}

void GraphicSystem::removeTexture(Texture* texture)
{
    if(texture)
    {
        textures.eraseUnordered(texture);
        graphicSystemBackEnd->removeTexture(texture->getId());
        delete texture;
        texture = nullptr;
    }
}

void GraphicSystem::removeRenderTarget(RenderTarget* renderTarget)
{
    if(renderTarget)
    {
        renderTargets.eraseUnordered(renderTarget);
        graphicSystemBackEnd->removeRenderTarget(renderTarget->getId());
        delete renderTarget;
        renderTarget = nullptr;
    }
}

void GraphicSystem::removeShaderParameterBlock(ShaderParameterBlock* shaderParameterBlock)
{
    if(shaderParameterBlock)
    {
        shaderParameterBlocks.eraseUnordered(shaderParameterBlock);
        graphicSystemBackEnd->removeBuffer(shaderParameterBlock->getId());
        delete shaderParameterBlock;
        shaderParameterBlock = nullptr;
    }
}

void GraphicSystem::clearVertexDatas()
{
    for(unsigned int i = 0; i < vertexDataComponents.size(); ++i)
    {
        graphicSystemBackEnd->removeVertexData(vertexDataComponents[i]->getId());
        delete vertexDataComponents[i];
    }

    vertexDataComponents.reset();
}

void GraphicSystem::clearAttributeBuffers()
{
    for(unsigned int i = 0; i < attributeBuffers.size(); ++i)
    {
        unsigned int id = attributeBuffers[i]->getId();
        if(id != -1)
            graphicSystemBackEnd->removeBuffer(id);

        delete attributeBuffers[i];
    }

    attributeBuffers.reset();
}

void GraphicSystem::clearVertexStreams()
{
    for(unsigned int i = 0; i < vertexStreams.size(); ++i)
    {
        unsigned int id = vertexStreams[i]->getId();
        if(id != -1)
            graphicSystemBackEnd->removeBuffer(id);

        delete vertexStreams[i];
    }

    vertexStreams.reset();
}

void GraphicSystem::clearIndexBuffers()
{
    for(unsigned int i = 0; i < indexBuffers.size(); ++i)
    {
        graphicSystemBackEnd->removeBuffer(indexBuffers[i]->getId());
        delete indexBuffers[i];
    }

    indexBuffers.reset();
}

void GraphicSystem::clearShaderPrograms()
{
    for(unsigned int i = 0; i < shaderPrograms.size(); ++i)
    {
        graphicSystemBackEnd->removeShaderProgram(shaderPrograms[i]->getId());
        delete shaderPrograms[i];
    }

    shaderPrograms.reset();
}

void GraphicSystem::clearShaders()
{
    for(unsigned int i = 0; i < shaders.size(); ++i)
    {
        delete shaders[i];
    }

    shaders.reset();
}

void GraphicSystem::clearTextures()
{
    for(unsigned int i = 0; i < textures.size(); ++i)
    {
        graphicSystemBackEnd->removeTexture(textures[i]->getId());
        delete textures[i];
    }

    textures.reset();
}

void GraphicSystem::clearRenderTargets()
{
    for(unsigned int i = 0; i < renderTargets.size(); ++i)
    {
        graphicSystemBackEnd->removeRenderTarget(renderTargets[i]->getId());
        delete renderTargets[i];
    }

    renderTargets.reset();
}

void GraphicSystem::clearShaderParameterBlocks()
{
    for(unsigned int i = 0; i < shaderParameterBlocks.size(); ++i)
    {
        graphicSystemBackEnd->removeBuffer(shaderParameterBlocks[i]->getId());
        delete shaderParameterBlocks[i];
    }

    shaderParameterBlocks.reset();
}

ShaderProgram* GraphicSystem::getShaderCombination(unsigned int shaderCombinationTag)
{
    ShaderProgram* result;
    return shaderPrograms.findItem([shaderCombinationTag](const ShaderProgram* program){return program->getShaderCombinationTag() == shaderCombinationTag;}, result) ? result : nullptr;
}

ShaderProgram* GraphicSystem::getShaderCombination(const Vector<std::string>& shaderFileNames, const Vector<ShaderDefine>& shaderDefines)
{
    int combinationTag = generateShaderCombinationTag(shaderFileNames, shaderDefines);
    ShaderProgram* program = getShaderCombination(combinationTag);

    return program ? program : nullptr;
}

unsigned int GraphicSystem::generateShaderCombinationTag(const Vector<Shader*>& shaders)
{
    Vector<std::string> shaderFileNames;
    Vector<ShaderDefine> shaderDefines;

    for(unsigned int i = 0; i < shaders.size(); ++i)
    {
        Shader* shader = shaders[i];
        shaderFileNames.pushBack(shader->getSourceFileName());
        auto perShaderDefines = shader->getDefines();
        shaderDefines.pushBack(perShaderDefines);
    }

    return generateShaderCombinationTag(shaderFileNames, shaderDefines);
}

unsigned int GraphicSystem::generateShaderCombinationTag(const Vector<std::string>& shaderFileNames, const Vector<ShaderDefine>& shaderDefines)
{
    std::string id = "";
    for(unsigned int i = 0; i < shaderFileNames.size(); ++i)
        id.append(shaderFileNames[i]); 

    char tempBuffer[24];
    for(unsigned int i = 0; i < shaderDefines.size(); ++i)
    {
        sprintf(tempBuffer, "%d", shaderDefines[i].defineType);
        id.append(tempBuffer);
    }
 
    return generateHash((unsigned char*)id.c_str(), id.size()); 
}

Texture* GraphicSystem::getTextureBySlotIndex(TextureSlotIndex index)
{
    Texture* result;
    return textures.findItem([index](const Texture* texture){return texture->getSlotIndex() == index;}, result) ? result : nullptr;
}

ShaderParameterBlock* GraphicSystem::getShaderParameterBlockByName(const std::string& name)
{
    ShaderParameterBlock* result;
    return shaderParameterBlocks.findItem([name](const ShaderParameterBlock* block){return block->getName().compare(name) == 0;}, result) ? result : nullptr;
}

RenderTarget* GraphicSystem::getRenderTargetByName(const std::string& name)
{
    RenderTarget* result;
    return renderTargets.findItem([name](const RenderTarget* target){return target->getName().compare(name) == 0;}, result) ? result : nullptr;
}

}