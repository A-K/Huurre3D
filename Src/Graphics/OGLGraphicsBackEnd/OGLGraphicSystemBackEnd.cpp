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


#include "Graphics/OGLGraphicsBackEnd/OGLGraphicSystemBackEnd.h"
#include "Graphics/OGLGraphicsBackEnd/OGLDefs.h"
#include "Graphics/OGLGraphicsBackEnd/OGLShaderLoader.h"
#include "Graphics/VertexData.h"
#include "Graphics/ShaderParameterBlock.h"
#include "Graphics/RenderTarget.h"

#include <iostream>

namespace Huurre3D
{

const int uniformNameBufSize = 256;

void OGLGraphicSystemBackEnd::setViewPort(const ViewPort& viewPort)
{
    glViewport(viewPort.x, viewPort.y, viewPort.width, viewPort.height);
}

void OGLGraphicSystemBackEnd::clear(unsigned int flags, const Vector4& color)
{
    GLbitfield glFlags = 0;
	
    if((flags & CLEAR_COLOR) == CLEAR_COLOR)
    {
        glFlags |= GL_COLOR_BUFFER_BIT;
        if(currentClearColor != color)
        {
            glClearColor(color.x, color.y, color.z, color.w);
            currentClearColor = color;
        }
    }
    if((flags & CLEAR_DEPTH) == CLEAR_DEPTH)
        glFlags |= GL_DEPTH_BUFFER_BIT;

    glClear(glFlags);
}

unsigned int OGLGraphicSystemBackEnd::createVertexStream()
{
    return createBuffer();
}

unsigned int OGLGraphicSystemBackEnd::createIndexBuffer()
{
    return createBuffer();
}

unsigned int OGLGraphicSystemBackEnd::createShaderParameterBlock(const std::string& name)
{
    shaderParameterBlockNames.pushBack(name);
    return createBuffer();
}

unsigned int OGLGraphicSystemBackEnd::createBuffer()
{
    GLuint bufferId;
    glGenBuffers(1, &bufferId);
    return bufferId;
}

unsigned int OGLGraphicSystemBackEnd::createVertexData()
{
    GLuint vertexDataId;
    glGenVertexArrays(1, &vertexDataId);
    return vertexDataId;
}

unsigned int OGLGraphicSystemBackEnd::createTexture()
{
    GLuint textureId;
    glGenTextures(1, &textureId);
    return textureId;
}

unsigned int OGLGraphicSystemBackEnd::createShaderProgram()
{
    return glCreateProgram();
}

unsigned int OGLGraphicSystemBackEnd::createRenderTarget(int width, int height, int numBuffers, int numLayers)
{
    GLuint frameBufferId;
    glGenFramebuffers(1, &frameBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);

    GLenum drawBuffers[MaxRenderTargetBuffers];
    drawBuffers[0] = GL_NONE;

    for(int i = 0; i < numBuffers; ++i)
        drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;

    glDrawBuffers(numBuffers, drawBuffers);

    return frameBufferId;
}

void OGLGraphicSystemBackEnd::removeBuffer(unsigned int bufferId)
{
    glDeleteBuffers(1, &bufferId);
}

void OGLGraphicSystemBackEnd::removeVertexData(unsigned int vertexDataId)
{
    glDeleteVertexArrays(1, &vertexDataId);
}

void OGLGraphicSystemBackEnd::removeTexture(unsigned int textureId)
{
    glDeleteTextures(1, &textureId);
}

void OGLGraphicSystemBackEnd::removeShaderProgram(unsigned int shaderProgramId)
{
    glDeleteProgram(shaderProgramId);
}

void OGLGraphicSystemBackEnd::removeRenderTarget(unsigned int renderTargetId)
{
    glDeleteFramebuffers(1, &renderTargetId);
}

void OGLGraphicSystemBackEnd::updateVertexStream(VertexStream* stream)
{
    GLint numVertices = stream->getNumVertices();
    GLint vertexSize = stream->getVertexSize();

    glBindBuffer(GL_ARRAY_BUFFER, stream->getId());
    glBufferData(GL_ARRAY_BUFFER, vertexSize * numVertices , stream->getGraphicData(), GL_STATIC_DRAW);
    enableInterleavedAttributes(stream);
    stream->unDirty();
    stream->discardData();
}

void OGLGraphicSystemBackEnd::updateIndexBuffer(IndexBuffer* buffer)
{	
    GLint indexSize = glIndexSize[static_cast<int>(buffer->getIndexType())];
    GLenum usage = buffer->isDynamic() ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->getId());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * buffer->getNumIndices(), buffer->getGraphicData(), usage);
    buffer->unDirty();
    buffer->discardData();
}

void OGLGraphicSystemBackEnd::enableInterleavedAttributes(VertexStream* vertexStream)
{
    GLint vertexSize = vertexStream->getVertexSize();
    GLint offset = 0;
    auto attributeDescriptions = vertexStream->getAttributeDescriptions();

    glBindBuffer(GL_ARRAY_BUFFER, vertexStream->getId());
    for(unsigned int i = 0; i < attributeDescriptions.size(); ++i)
    {
        GLenum type = glAttributeType[static_cast<int>(attributeDescriptions[i].type)];
        glEnableVertexAttribArray(static_cast<int>(attributeDescriptions[i].semantic));
        glVertexAttribPointer(static_cast<int>(attributeDescriptions[i].semantic), attributeDescriptions[i].numComponentsPerVertex, type, attributeDescriptions[i].normalized, vertexSize, (GLvoid*)offset);
        offset += attributeDescriptions[i].stride;
    }
}

void OGLGraphicSystemBackEnd::setVertexData(VertexData *vertexData)
{
    glBindVertexArray(vertexData->getId());

    auto vertexStreams = vertexData->getVertexStreams();
    for(unsigned int i = 0; i < vertexStreams.size(); ++i)
        updateVertexStream(vertexStreams[i]);

    if(vertexData->isIndexed())
    {
        currentIndexType = vertexData->getIndexBuffer()->getIndexType();

        if(vertexData->getIndexBuffer()->isDirty())
            updateIndexBuffer(vertexData->getIndexBuffer());
    }
	
    currentPrimitiveType = vertexData->getPrimitiveType();
}

void OGLGraphicSystemBackEnd::updateTexture(Texture* texture)
{
    GLenum target = glTextureTarget[static_cast<int>(texture->getTargetMode())];
    glBindTexture(target, texture->getId());

    if(texture->isParamsDirty())
        updateTextureParameters(texture);

    if(texture->isDataDirty())
        texture->isCompressed() ? updateCompressedTextureData(texture) : updateTextureData(texture);
}

void OGLGraphicSystemBackEnd::updateTextureParameters(Texture* texture)
{	
    GLenum target = glTextureTarget[static_cast<int>(texture->getTargetMode())];

    //Wrapping
    int wrap = static_cast<int>(texture->getWrapMode());
    glTexParameteri(target, GL_TEXTURE_WRAP_S, glWrapMode[wrap]);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, glWrapMode[wrap]);

    if(target == GL_TEXTURE_3D || target == GL_TEXTURE_CUBE_MAP)
        glTexParameteri(target, GL_TEXTURE_WRAP_R, glWrapMode[wrap]);

    //Filtering.
    int filter = static_cast<int>(texture->getFilterMode());
    int numMipMaps = texture->getNumMipMaps();

    if(numMipMaps < 2)
    {
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, glMinFilter[filter]);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, glMaxFilter[filter]);
    }
    else
    {
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, glMinMipMapFilter[filter]);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, glMaxFilter[filter]);
    }

    if(!texture->isCompressed())
    {
        glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);

        numMipMaps == 0 ? glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, 0) : glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, numMipMaps - 1);
    }
    texture->unDirtyParams();
}

void OGLGraphicSystemBackEnd::updateTextureData(Texture* texture)
{
    GLenum target = glTextureTarget[static_cast<int>(texture->getTargetMode())];
    int pixelFormat = static_cast<int>(texture->getPixelFormat());
    GLenum format = glPixelFormat[pixelFormat];
    GLenum internalFormat = glPixelInternalFormat[pixelFormat];
    GLenum pixelType = glPixelType[pixelFormat];
    GLsizei width = texture->getWidth();
    GLsizei height = texture->getHeight();

    switch(target) 
    {
        case GL_TEXTURE_2D:
            glTexImage2D(target, 0, internalFormat, width, height, 0, format, pixelType, texture->getGraphicData());
            break;
        case GL_TEXTURE_3D:
        case GL_TEXTURE_2D_ARRAY:
            glTexImage3D(target, 0, internalFormat, width, height, texture->getDepth(), 0, format, pixelType, texture->getGraphicData());
            break;
        case GL_TEXTURE_CUBE_MAP:
            glTexImage2D(glCubeMapFaces[int(CubeMapFace::PositiveX)], 0, internalFormat, width, height, 0, format, pixelType, texture->getCubeMapFaceData(CubeMapFace::PositiveX));
            glTexImage2D(glCubeMapFaces[int(CubeMapFace::NegativeX)], 0, internalFormat, width, height, 0, format, pixelType, texture->getCubeMapFaceData(CubeMapFace::NegativeX));
            glTexImage2D(glCubeMapFaces[int(CubeMapFace::PositiveY)], 0, internalFormat, width, height, 0, format, pixelType, texture->getCubeMapFaceData(CubeMapFace::PositiveY));
            glTexImage2D(glCubeMapFaces[int(CubeMapFace::NegativeY)], 0, internalFormat, width, height, 0, format, pixelType, texture->getCubeMapFaceData(CubeMapFace::NegativeY));
            glTexImage2D(glCubeMapFaces[int(CubeMapFace::PositiveZ)], 0, internalFormat, width, height, 0, format, pixelType, texture->getCubeMapFaceData(CubeMapFace::PositiveZ));
            glTexImage2D(glCubeMapFaces[int(CubeMapFace::NegativeZ)], 0, internalFormat, width, height, 0, format, pixelType, texture->getCubeMapFaceData(CubeMapFace::NegativeZ));
            break;
    }

    glGenerateMipmap(target);
    texture->unDirtyData();

    texture->discardData();
}
void OGLGraphicSystemBackEnd::updateCompressedTextureData(Texture* texture)
{
    GLenum target = glTextureTarget[static_cast<int>(texture->getTargetMode())];
    int pixelFormat = static_cast<int>(texture->getPixelFormat());
    GLenum internalFormat = glPixelInternalFormat[pixelFormat];
    GLsizei width = texture->getWidth();
    GLsizei height = texture->getHeight();
    const unsigned char* data = texture->getGraphicData();
    GLsizei offset = 0;

    switch(target) 
    {
        case GL_TEXTURE_2D:
            for(int i = 0; i < texture->getNumMipMaps(); ++i)
            {
                GLsizei size = ((width + 3) / 4) * ((height + 3) / 4) * pixelFormatSizeInBytes[static_cast<int>(pixelFormat)];
                glCompressedTexImage2D(target, i, internalFormat, width, height, 0, size, data + offset);
                offset += size;
                width = max(1, width / 2);
                height = max(1, height / 2);
            }
            break;
    }

    texture->unDirtyData();
    texture->discardData();
}

void OGLGraphicSystemBackEnd::updateShaderProgram(ShaderProgram* program)
{
    if(!program->getVertexShader() || !program->getFragmentShader())
        return;

    GLuint programID = program->getId();
    Shader* vertexShader = program->getVertexShader();
    Shader* fragmentShader = program->getFragmentShader();

    if(!vertexShader->isCompiled())
    {	
        vertexShader->setId(glCreateShader(GL_VERTEX_SHADER));
        compileShader(vertexShader);
        glAttachShader(programID, vertexShader->getId());
    }
	
    if(!fragmentShader->isCompiled())
    {
        fragmentShader->setId(glCreateShader(GL_FRAGMENT_SHADER));
        compileShader(fragmentShader);
        glAttachShader(programID, fragmentShader->getId());
    }

    //Bind vertex attribute locations using specified layout.
    for(int i = 0; i < static_cast<int>(AttributeSemantic::NumSemantics); ++i)
        glBindAttribLocation(programID, i, glAttributeName[i]);

    //Linking
    glLinkProgram(programID);

    GLint linked = 0;
    glGetProgramiv(programID, GL_LINK_STATUS, &linked);
    if(!linked)
    {
        GLint infoLogLength = 0;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if(infoLogLength > 1)
        {
            GLchar* linkerLog = (GLchar*)malloc(sizeof(GLchar) * infoLogLength);

            glGetShaderInfoLog(programID, infoLogLength, NULL, linkerLog);
            std::cout << "Failed to link the program: " << linkerLog <<std::endl;
            free(linkerLog);
        }
    }
    else
    {
        fetchUniformBlocks(program);
        fetchUniforms(program);
        program->setLinked(true);
		
        //Shader program is linked and ready to be used, there is no need to keep shaders in memory.
        vertexShader->getSource().clear();
        fragmentShader->getSource().clear();
        glDetachShader(programID, vertexShader->getId());
        glDetachShader(programID, fragmentShader->getId());

        glDeleteShader(vertexShader->getId());
        glDeleteShader(fragmentShader->getId());
    }
}

void OGLGraphicSystemBackEnd::setShaderProgram(ShaderProgram* program)
{
    if(!program->isLinked())
        updateShaderProgram(program);

    currentShaderProgramId = program->getId();
    glUseProgram(currentShaderProgramId);
}

void OGLGraphicSystemBackEnd::compileShader(Shader* shader)
{
    OGLShaderLoader shaderLoader;
    shaderLoader.load(shader);

    GLuint shaderID = shader->getId();
    const GLchar *source = shader->getSource().c_str();
    glShaderSource(shaderID, 1, &source, 0);
    glCompileShader(shaderID);
    GLint compiled = 0;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);

    if(!compiled)
    {
        GLint infoLogLength = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

        if(infoLogLength > 1)
        {
            GLchar* compilerLog = (GLchar*)malloc(sizeof(GLchar) * infoLogLength);

            glGetShaderInfoLog(shaderID, infoLogLength, NULL, compilerLog);
            std::cout << "Failed to compile shader: " << compilerLog <<std::endl;
            free(compilerLog);
        }
    }
    else
        shader->setCompiled(true);
}

void OGLGraphicSystemBackEnd::fetchUniformBlocks(ShaderProgram* program)
{
    GLuint programId = program->getId();
    GLint numBlocks;
    glGetProgramiv(programId, GL_ACTIVE_UNIFORM_BLOCKS, &numBlocks);

    GLchar name[uniformNameBufSize];
    GLint activeUniforms = 0;

    for(int i = 0; i < numBlocks; ++i)
    {
        GLsizei nameSize = 0;
        glGetActiveUniformBlockName(programId, i, uniformNameBufSize, &nameSize, name);

        ShaderParameterBlockDescription paramBlockDesc;
        paramBlockDesc.setName(std::string(name, nameSize));
 
        GLuint index = glGetUniformBlockIndex(programId, paramBlockDesc.name.c_str());
        paramBlockDesc.shaderIndex = index;

        glGetActiveUniformBlockiv(programId, paramBlockDesc.shaderIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &paramBlockDesc.size);

        //search the buffer binding point.
        for(unsigned int j = 0; j < shaderParameterBlockNames.size(); ++j)
        {
            if(shaderParameterBlockNames[j] == paramBlockDesc.name)
            {
                paramBlockDesc.bindingPoint = j;
                break;
            }
        }
        
        program->setShaderParameterBlockDescription(paramBlockDesc);
    }
}

void OGLGraphicSystemBackEnd::fetchUniforms(ShaderProgram* program)
{
    GLuint programId = program->getId();
    GLint numUniforms;
    glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &numUniforms);

    GLchar name[uniformNameBufSize];
    for(int i = 0; i < numUniforms; ++i)
    {
        GLint arraySize = 0;
        GLenum type = 0;
        GLsizei nameSize = 0;
        glGetActiveUniform(programId, i, uniformNameBufSize, &nameSize, &arraySize, &type, name);
        GLint index = glGetUniformLocation(programId, name);

        if(index < 0)
            continue;

        ShaderParameterDescription paramDesc;
        paramDesc.setName(std::string(name, nameSize));
        paramDesc.arraySize = arraySize;

        //search the correct uniform type.
        for(int j = 0; j < static_cast<int>(ShaderParameterType::NumTypes); ++j)
        {
            if(glUniformType[j] == type)
            {
                paramDesc.type = static_cast<ShaderParameterType>(j);
                break;
            }
        }

        //TODO: Fix this, there has to be better way to check if the uniform is a sampler than compare the type every sampler type.
        //Check from gl specs that does it work if one only checks that the type is between [GL_SAMPLER_1D, GL_UNSIGNED_INT_SAMPLER_2D_RECT]
        if(type == GL_SAMPLER_2D || type == GL_INT_SAMPLER_2D || type == GL_SAMPLER_2D_ARRAY || type == GL_SAMPLER_CUBE)
        {
            for(int unit = 0; unit < static_cast<int>(TextureSlotIndex::NumSlots); ++unit)
            {
                if(glTextureSlots[unit] == paramDesc.name)
                {
                    glUseProgram(program->getId());
                    glUniform1iv(index, 1, &unit);
                    break;
                }
            }
        }
        paramDesc.shaderIndex = index;
        program->setShaderParameterDescription(paramDesc); 
    }
}

void OGLGraphicSystemBackEnd::setShaderParameter(ShaderParameterDescription* description, const ShaderParameter& shaderParameter)
{    
    switch(description->type)
    {
        case ShaderParameterType::Float:
            glProgramUniform1fv(currentShaderProgramId, description->shaderIndex, description->arraySize, (const GLfloat*)shaderParameter.value);
            break;
        case ShaderParameterType::FloatVector2:
            glProgramUniform2fv(currentShaderProgramId, description->shaderIndex, description->arraySize, (const GLfloat*)shaderParameter.value);
            break;
        case ShaderParameterType::FloatVector3:
            glProgramUniform3fv(currentShaderProgramId, description->shaderIndex, description->arraySize, (const GLfloat*)shaderParameter.value);
            break;
        case ShaderParameterType::FloatVector4:
            glProgramUniform4fv(currentShaderProgramId, description->shaderIndex, description->arraySize, (const GLfloat*)shaderParameter.value);
            break;
        case ShaderParameterType::Int:
            glProgramUniform1iv(currentShaderProgramId, description->shaderIndex, description->arraySize, (const GLint*)shaderParameter.value);
            break;
        case ShaderParameterType::IntVector2:
            glProgramUniform2iv(currentShaderProgramId, description->shaderIndex, description->arraySize, (const GLint*)shaderParameter.value);
            break;
        case ShaderParameterType::IntVector3:
            glProgramUniform3iv(currentShaderProgramId, description->shaderIndex, description->arraySize, (const GLint*)shaderParameter.value);
            break;
        case ShaderParameterType::IntVector4:
            glProgramUniform4iv(currentShaderProgramId, description->shaderIndex, description->arraySize, (const GLint*)shaderParameter.value);
            break;
        case ShaderParameterType::FloatMatrix2:
            glProgramUniformMatrix2fv(currentShaderProgramId, description->shaderIndex, description->arraySize, false, (const GLfloat*)shaderParameter.value);
            break;
        case ShaderParameterType::FloatMatrix3:
            glProgramUniformMatrix3fv(currentShaderProgramId, description->shaderIndex, description->arraySize, false, (const GLfloat*)shaderParameter.value);
            break;
        case ShaderParameterType::FloatMatrix4:
            glProgramUniformMatrix4fv(currentShaderProgramId, description->shaderIndex, description->arraySize, false, (const GLfloat*)shaderParameter.value);
            break;
    }
}

void OGLGraphicSystemBackEnd::setShaderParameterBlock(ShaderParameterBlockDescription* description, ShaderParameterBlock* block)
{
   
    unsigned int blockId = block->getId();
    if(currentBindedShaderBlockId != blockId)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, blockId);
        currentBindedShaderBlockId = blockId;
    }

    if(!block->hasBindingIndex() || !description->sourceBlockSetted)
    {
        unsigned int bindingPoint = description->bindingPoint;

        //glBindBuffer(GL_UNIFORM_BUFFER, block->getId());
        glUniformBlockBinding(currentShaderProgramId, description->shaderIndex, bindingPoint);
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, blockId);
        block->setBindingIndex(bindingPoint);
        description->sourceBlockSetted = true;
    }

    if(block->isDirty())
        updateShaderParameterBlock(block);
}

void OGLGraphicSystemBackEnd::updateShaderParameterBlock(ShaderParameterBlock* block)
{
    glBufferData(GL_UNIFORM_BUFFER, block->getSizeInBytes(), block->getGraphicData(), GL_STREAM_DRAW);
    block->unDirty();
}

void OGLGraphicSystemBackEnd::setTexture(Texture* texture)
{
    int textureSlot = static_cast<int>(texture->getSlotIndex());

    if(texturesInUse[textureSlot] != texture)
    {
        if(currentlyActiveTextureSlot != textureSlot)
        {
            glActiveTexture(GL_TEXTURE0 + textureSlot);
            currentlyActiveTextureSlot = textureSlot;
        }
	
        GLenum target = glTextureTarget[static_cast<int>(texture->getTargetMode())];
        glBindTexture(target, texture->getId());

        if(texture->isParamsDirty() || texture->isDataDirty())
            updateTexture(texture);

        texturesInUse[textureSlot] = texture;
    }
    else
    {
        if(currentlyActiveTextureSlot != textureSlot)
        {
            glActiveTexture(GL_TEXTURE0 + textureSlot);
            currentlyActiveTextureSlot = textureSlot;
        }

        if(texture->isParamsDirty() || texture->isDataDirty())
        {
            GLenum target = glTextureTarget[static_cast<int>(texture->getTargetMode())];
            glBindTexture(target, texture->getId());
            updateTexture(texture);
        }
    }
}

void OGLGraphicSystemBackEnd::setRasterState(const RasterState& state)
{
    if(currentBlendState != state.blendState)
    {
        if(!state.blendState.enabled)
        {
            glDisable(GL_BLEND);
            currentBlendState.enabled = state.blendState.enabled;
        }
        else
        {
            if(!currentBlendState.enabled)
            {
                glEnable(GL_BLEND);
                currentBlendState.enabled = state.blendState.enabled;
            }

            if(currentBlendState.blendFunction != state.blendState.blendFunction)
            {
                glBlendFunc(glBlendSourceFactor[static_cast<int>(state.blendState.blendFunction)], glBlendDestFactor[static_cast<int>(state.blendState.blendFunction)]);
                currentBlendState.blendFunction = state.blendState.blendFunction;
            }
        }
    }

    if(currentCompareState != state.compareState)
    {
        if(!state.compareState.enabled)
        {
            glDisable(GL_DEPTH_TEST);
            currentCompareState.enabled = state.compareState.enabled;
        }
        else
        {
            if(!currentCompareState.enabled)
            {
                glEnable(GL_DEPTH_TEST);
                currentCompareState.enabled = state.compareState.enabled;
            }
        
            if(currentCompareState.compareFunction != state.compareState.compareFunction)
            {
                glDepthFunc(glCompareMode[static_cast<int>(state.compareState.compareFunction)]);
                currentCompareState.compareFunction = state.compareState.compareFunction;
            }
        }
    }

    if(currentCullState != state.cullState)
    {
        if(!state.cullState.enabled)
        {
            glDisable(GL_CULL_FACE);
            currentCullState.enabled = state.cullState.enabled;
        }
        else
        {
            if(!currentCullState.enabled)
            {
                glEnable(GL_CULL_FACE);
                currentCullState.enabled = state.cullState.enabled;
            }
            
            if(currentCullState.cullFace != state.cullState.cullFace)
            {
                glCullFace(glCullMode[static_cast<int>(state.cullState.cullFace)]);
                currentCullState.cullFace = state.cullState.cullFace;
            }
        }
    }
}

void OGLGraphicSystemBackEnd::setDepthWrite(bool enable)
{
    glDepthMask(enable);
}

void OGLGraphicSystemBackEnd::setColorWrite(bool enable)
{
    enable ? glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE) : glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
}

void OGLGraphicSystemBackEnd::updateRenderTarget(RenderTarget* renderTarget)
{
    unsigned int i = 0;
    Vector<Texture*> colorBuffers = renderTarget->getColorBuffers();
    for(i = 0; i <colorBuffers.size(); ++i)
    {
        renderTarget->isLayered() ? glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, colorBuffers[i]->getId(), 0, renderTarget->getRenderLayer()) : 
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, colorBuffers[i]->getId(), 0);
    }

    Texture* depthTexture = renderTarget->getDepthTexture();
    if(depthTexture)
    {
        renderTarget->isLayered() ? glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture->getId(), 0, renderTarget->getRenderLayer()) : 
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture->getId(), 0);
    }

    renderTarget->unDirty();
}

void OGLGraphicSystemBackEnd::setOffLineRenderTarget(RenderTarget* renderTarget)
{
    unsigned int fboId = renderTarget->getId();

    if(currentFBOId != fboId)
    {
        Vector<Texture*> colorBuffers = renderTarget->getColorBuffers();
        for(unsigned int i = 0; i < colorBuffers.size(); ++i)
            setTexture(colorBuffers[i]);

        Texture* depthTexture = renderTarget->getDepthTexture();
        if(depthTexture)
            setTexture(depthTexture);

        glBindFramebuffer(GL_FRAMEBUFFER, fboId);
        currentFBOId = fboId;
    }

    if(renderTarget->isDirty())
        updateRenderTarget(renderTarget);
}

void OGLGraphicSystemBackEnd::setMainRenderTarget()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    currentFBOId = 0;
}

void OGLGraphicSystemBackEnd::draw(int numVertices, int vertexOffset)
{
    glDrawArrays(glPrimitiveType[static_cast<int>(currentPrimitiveType)], vertexOffset, numVertices);
}

void OGLGraphicSystemBackEnd::drawIndexed(int numIndices, int indexOffset)
{    
    int indexType = static_cast<int>(currentIndexType);
    glDrawElements(glPrimitiveType[static_cast<int>(currentPrimitiveType)], numIndices, glIndexType[indexType], (GLvoid*)(indexOffset * glIndexSize[indexType]));
}

void OGLGraphicSystemBackEnd::drawInstanced(int numIndices, int indexOffset, int instancesCount)
{
    int indexType = static_cast<int>(currentIndexType);
    glDrawElementsInstanced(glPrimitiveType[static_cast<int>(currentPrimitiveType)], numIndices, glIndexType[indexType], (GLvoid*)(indexOffset * glIndexSize[indexType]), instancesCount);
}

}