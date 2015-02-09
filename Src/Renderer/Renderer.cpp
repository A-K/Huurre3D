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

#include "Engine/Engine.h"
#include "Renderer/DeferredStage.h"
#include "Renderer/LightingStage.h"
#include "Renderer/ShadowStage.h"
#include "Renderer/PostProcessStage.h"
#include "Renderer/Renderer.h"
#include "Renderer/Geometry.h"
#include "Graphics/GraphicSystem.h"
#include "Scene/Scene.h"
#include "Scene/Camera.h"
#include "Scene/Light.h"
#include "Scene/Mesh.h"
#include "Scene/Joint.h"
#include "Scene/SkyBox.h"
#include "Math/Frustum.h"
#include <iostream>

namespace Huurre3D
{

RENDERSTAGE_TYPE_IMPL(DeferredStage);
RENDERSTAGE_TYPE_IMPL(ShadowStage);
RENDERSTAGE_TYPE_IMPL(LightingStage);
RENDERSTAGE_TYPE_IMPL(PostProcessStage);

Renderer::Renderer()
{
    graphicWindow = new GraphicWindow();
    graphicSystem = new GraphicSystem();
}

Renderer::~Renderer()
{
    for(unsigned int i = 0; i < renderStages.size(); ++i)
        delete renderStages[i];

    materials.reset();
    geometries.reset();
    delete graphicSystem;
    delete graphicWindow;
}
bool Renderer::init(const JSONValue& rendererJSON)
{
    auto renderWindowJSON = rendererJSON.getJSONValue("renderWindow");

    if(renderWindowJSON.isNull())
    {
        std::cout << "Failed to create render window. The mandatory value renderWindow is missing" << std::endl;
        return false;
    }

    bool success = createRenderWindow(renderWindowJSON);
    if(success)
    {
        int width = graphicWindow->getWidth();
        int height = graphicWindow->getHeight();
        cameraShaderParameterBlock = graphicSystem->createShaderParameterBlock(sp_cameraParameters);
        materialParameterBlock = graphicSystem->createShaderParameterBlock(sp_materialProperties);
        renderTargetSizeBlock = graphicSystem->createShaderParameterBlock(sp_renderTargetSize);
        skinMatrixArray = graphicSystem->createShaderParameterBlock(sp_skinMatrixArray);
        Vector4 renderTargetSizeValue = Vector4(float(width), float(height), (1.0f / float(width)), (1.0f / float(height)));
        renderTargetSizeBlock->addParameter(renderTargetSizeValue);

        createFullScreenQuad();
        screenViewPort.set(0, 0, width, height);

        auto materialVertexShaderJSON = rendererJSON.getJSONValue("materialVertexShader");
        auto materialFragmentShaderJSON = rendererJSON.getJSONValue("materialFragmentShader");

        if(!materialVertexShaderJSON.isNull())
            materialVertexShader = materialVertexShaderJSON.getString();

        if(!materialFragmentShaderJSON.isNull())
            materialFragmentShader = materialFragmentShaderJSON.getString();

        auto renderStagesJSON = rendererJSON.getJSONValue("renderStages");
        if(renderStagesJSON.isNull())
        {
            std::cout << "Failed to init renderer. The mandatory value renderStages is missing" << std::endl;
        }
        else
        {
            for(unsigned int i = 0; i < renderStagesJSON.getSize(); ++i)
            {
                auto renderStageJSON = renderStagesJSON.getJSONArrayItem(i);
                auto renderStageNameJSON = renderStageJSON.getJSONValue("name");
                auto renderTargetImplementationJSON = renderStageJSON.getJSONValue("implementation");

                if(renderStageNameJSON.isNull() || renderTargetImplementationJSON.isNull())
                {
                    std::cout << "Failed to create renderStage " << i << ". One of the mandatory values: name, implementation is missing" << std::endl;
                }
                else
                {
                    RenderStage* renderStage = RenderStageFactory::createRenderStage(this, renderStageNameJSON.getString());
                    if(renderStage)
                    {
                        renderStage->init(renderTargetImplementationJSON);
                        renderStages.pushBack(renderStage);
                    }
                    else
                        std::cout << "RenderStage " << renderStageNameJSON.getString()<<" have not been registered." << std::endl;
                }
            }
        }
    }

    return success;
}

bool Renderer::createRenderWindow(const JSONValue& renderWindowJSON)
{
    bool success = false;
    auto widthJSON = renderWindowJSON.getJSONValue("width");
    auto heightJSON = renderWindowJSON.getJSONValue("height");
    auto windowTitleJSON = renderWindowJSON.getJSONValue("title");
    auto fullscreenJSON = renderWindowJSON.getJSONValue("fullscreen");
    auto vsyncJSON = renderWindowJSON.getJSONValue("vsync");

    if(widthJSON.isNull() || heightJSON.isNull() || windowTitleJSON.isNull())
        std::cout << "Failed to create render window from JSON. One of the mandatory values: width, height, title is missing" << std::endl;
    else
    {
        bool fullscreen = fullscreenJSON.isNull() ? false : fullscreenJSON.getBool();
        bool vsync = vsyncJSON.isNull() ? true : vsyncJSON.getBool();
        success = createRenderWindow(widthJSON.getInt(), heightJSON.getInt(), windowTitleJSON.getString(), fullscreen, vsync);
    }

    return success;
}

bool Renderer::createRenderWindow(int width, int height, const std::string& windowTitle, bool fullscreen, bool vsync)
{
    return graphicWindow->create(width, height, windowTitle, fullscreen, vsync);
}

void Renderer::resizeRenderWindow(int width, int height)
{
    screenViewPort.set(0, 0, width, height);
    Vector4 renderTargetSizeValue = Vector4(float(width), float(height), (1.0f / float(width)), (1.0f / float(height)));
    renderTargetSizeBlock->clearParameters();
    renderTargetSizeBlock->addParameter(renderTargetSizeValue);

    for(unsigned int i = 0; i < renderStages.size(); ++i)
        renderStages[i]->resizeResources();
}

void Renderer::renderScene(Scene* scene, Camera* camera)
{
    scene->update();
    cameraShaderParameterBlock->clearParameters();
    camera->getCameraShaderParameterBlock(cameraShaderParameterBlock);
    skinMatrixArray->clearParameters();

    Vector<Joint*> joints;
    scene->getSceneItemsByType<Joint>(joints);

    for(unsigned int i = 0; i < joints.size(); ++i)
    {
        Matrix4x4 skinMatrix = joints[i]->getSkinMatrix();
        skinMatrixArray->addParameter(skinMatrix);
    }

    for(unsigned int i = 0; i < renderStages.size(); ++i)
        renderStages[i]->clearStage();

    for(unsigned int i = 0; i < renderStages.size(); ++i)
    {
        RenderStage* renderStage = renderStages[i];
        stageupdateResults[i] = workQueue.submitTask([renderStage, scene](){renderStage->update(scene); });
    }
   
    for(unsigned int i = 0; i < renderStages.size(); ++i)
    {
        stageupdateResults[i].wait();
        renderStages[i]->execute();
    }

    graphicWindow->swapBuffers();
}

void Renderer::createRenderItems(const Vector<MaterialDescription>& materialDescriptions, const Vector<GeometryDescription>& geometryDescriptions, Vector<RenderItem>& renderItemsOut)
{
    if(materialDescriptions.size() == geometryDescriptions.size())
    {
        for(unsigned int i = 0; i < materialDescriptions.size(); ++i)
            renderItemsOut.pushBack(RenderItem(createMaterial(materialDescriptions[i]), createGeometry(geometryDescriptions[i])));
    }
}

void Renderer::createRenderItems(const Vector<MaterialDescription>& materialDescriptions, const Vector<GeometryDescription>& geometryDescriptions, Vector<Vector<RenderItem>>& renderItemsOut, unsigned int numItemsPerDescription)
{
    if(materialDescriptions.size() == geometryDescriptions.size())
    {
        Vector<Vector<Geometry*>> allGeometries;
        Vector<Vector<Material*>> allMaterials;

        for(unsigned int i = 0; i < materialDescriptions.size(); ++i)
        {
            Vector<Geometry*> geometriesPerDescription;
            Vector<Material*> materialsPerDescription;

            createMaterials(materialDescriptions[i], materialsPerDescription, numItemsPerDescription);
            createGeometries(geometryDescriptions[i], geometriesPerDescription, numItemsPerDescription);

            allGeometries.pushBack(geometriesPerDescription);
            allMaterials.pushBack(materialsPerDescription);
        }

        //Arrange the renderItems to correct order
        for(unsigned int i = 0; i < numItemsPerDescription; ++i)
        {
            Vector<RenderItem> items;
            for(unsigned int j = 0; j < allGeometries.size(); ++j)
            {
                items.pushBack(RenderItem(allMaterials[j][i], allGeometries[j][i]));
            }

            renderItemsOut.pushBack(items);
        }
    }
}

Material* Renderer::createMaterial(const MaterialDescription& materialDescription)
{
    Matrix4x4 parameters(Vector4(materialDescription.diffuseColor, materialDescription.roughness),
                         Vector4(materialDescription.specularColor, 0.0f),
                         Vector4(materialDescription.emissiveColor, materialDescription.reflectance),
                         Vector4(materialDescription.ambientColor, materialDescription.alpha));

    Material* material = new Material(parameters, materialDescription.rasterState, materialDescription.skinned);

    if(!materialDescription.diffuseTextureFile.empty())
        material->setDiffuseTexture(createMaterialTexture(materialDescription.diffuseTextureFile, TextureSlotIndex::Diffuse));

    if(!materialDescription.specularTextureFile.empty())
        material->setSpecularTexture(createMaterialTexture(materialDescription.specularTextureFile, TextureSlotIndex::Specular));

    if(!materialDescription.normalMapTextureFile.empty())
        material->setNormalMap(createMaterialTexture(materialDescription.normalMapTextureFile, TextureSlotIndex::NormalMap));

    if(!materialDescription.alphaTextureFile.empty())
        material->setAlphaTexture(createMaterialTexture(materialDescription.alphaTextureFile, TextureSlotIndex::Alpha));

    //Create the needed graphic resources
    unsigned int materialParameterId = material->getParameterId();
    int materialBufferIndex = materialBufferIndicies.getIndexToItem(materialParameterId);

    //Check if the material parameters are already in the buffer, if not, add the parameters to the buffer.
    if(materialBufferIndex == -1)
    {
        ShaderParameterBlock* materialParameterBlock = graphicSystem->getShaderParameterBlockByName(sp_materialProperties);
        materialParameterBlock->addParameter(material->getParameters());
        materialBufferIndicies.pushBack(materialParameterId);
        materialBufferIndex = materialBufferIndicies.size() - 1;
    }

    //The material has no program set, try if the shader program exist.
    Vector<std::string> shaderFileNames;
    shaderFileNames.pushBack(materialVertexShader);
    shaderFileNames.pushBack(materialFragmentShader);
    Vector<std::string> fragmentShaderDefines = material->getShaderDefines(ShaderType::Fragment);
    Vector<std::string> vertexShaderDefines = material->getShaderDefines(ShaderType::Vertex);
    Vector<std::string> combinedDefines = vertexShaderDefines;
    combinedDefines.pushBack(fragmentShaderDefines);

    ShaderProgram* program = graphicSystem->getShaderCombination(shaderFileNames, combinedDefines);

    //If the program dont exist, create one.
    if(!program)
    {
        Shader* vShader = graphicSystem->createShader(ShaderType::Vertex, materialVertexShader);
        Shader* fShader = graphicSystem->createShader(ShaderType::Fragment, materialFragmentShader);
        vShader->setDefines(vertexShaderDefines);
        fShader->setDefines(fragmentShaderDefines);
        program = graphicSystem->createShaderProgram(vShader, fShader);
        graphicSystem->setShaderProgram(program);
    }

    material->setCurrentShaderCombinationTag(program->getShaderCombinationTag());

    materials.pushBack(material);
    return material;
}

void Renderer::createMaterials(const MaterialDescription& materialDescription, Vector<Material*>& materialsOut, unsigned int numMaterials)
{
    Material* material = createMaterial(materialDescription);
    materialsOut.pushBack(material);

    for(unsigned int i = 1; i < numMaterials; ++i)
    {
        Material* copy = new Material(*material);
        materials.pushBack(copy);
        materialsOut.pushBack(copy);
    }
}

Geometry* Renderer::createGeometry(const GeometryDescription& geometryDescription)
{
    Geometry* geometry = new Geometry();
    BoundingBox boundingBox;
    VertexData* vd = graphicSystem->createVertexData(geometryDescription.primitiveType, geometryDescription.numVertices);

    if(geometryDescription.vertices)
    {
        boundingBox.mergePoints(geometryDescription.vertices, geometryDescription.numVertices);
        graphicSystem->setAttributesToVertexData(vd, AttributeType::Float, AttributeSemantic::Position, 3, geometryDescription.numVertices * 3, geometryDescription.vertices, false, false);
    }
    if(geometryDescription.normals)
        graphicSystem->setAttributesToVertexData(vd, AttributeType::Float, AttributeSemantic::Normal, 3, geometryDescription.numVertices * 3, geometryDescription.normals, false, false);

    if(geometryDescription.tangents)
        graphicSystem->setAttributesToVertexData(vd, AttributeType::Float, AttributeSemantic::Tangent, 3, geometryDescription.numVertices * 3, geometryDescription.tangents, false, false);

    if(geometryDescription.bitTangents)
        graphicSystem->setAttributesToVertexData(vd, AttributeType::Float, AttributeSemantic::BiTanget, 3, geometryDescription.numVertices * 3, geometryDescription.bitTangents, false, false);

    if(geometryDescription.jointIndices)
        graphicSystem->setAttributesToVertexData(vd, AttributeType::Float, AttributeSemantic::JointIndices, 4, geometryDescription.numVertices * 4, geometryDescription.jointIndices, false, false);
    
    if(geometryDescription.jointWeights)
        graphicSystem->setAttributesToVertexData(vd, AttributeType::Float, AttributeSemantic::JointWeights, 4, geometryDescription.numVertices * 4, geometryDescription.jointWeights, false, false);

    for(int i = 0; i < geometryDescription.numUVChanels; ++i)
    {
        if(geometryDescription.texCoords[i])
        {
            int numComp = geometryDescription.numComp[i];
            AttributeSemantic semantic = static_cast<AttributeSemantic>(int(AttributeSemantic::TexCoord0) + i);
            graphicSystem->setAttributesToVertexData(vd, AttributeType::Float, semantic, numComp, geometryDescription.numVertices * numComp, geometryDescription.texCoords[i], false, false);
        }
    }

    if(geometryDescription.indices16)
        graphicSystem->setIndicesToVertexData(vd, IndexType::Short, geometryDescription.numIndices, geometryDescription.indices16, false);
    else if(geometryDescription.indices32)
        graphicSystem->setIndicesToVertexData(vd, IndexType::Int, geometryDescription.numIndices, geometryDescription.indices32, false);

    graphicSystem->setVertexData(vd);
    geometry->setBoundingBox(boundingBox);
    geometry->setVertexData(vd);

    geometries.pushBack(geometry);
    return geometry;
}

void Renderer::createGeometries(const GeometryDescription& geometryDescription, Vector<Geometry*>& geometriesOut, unsigned int numGeometries)
{
    Geometry* geometry = createGeometry(geometryDescription);
    geometriesOut.pushBack(geometry);

    for(unsigned int i = 1; i < numGeometries; ++i)
    { 
        Geometry* copy = new Geometry(*geometry);
        geometries.pushBack(copy);
        geometriesOut.pushBack(copy);
    }
}

void Renderer::removeMaterial(Material* material)
{
    if(material)
    {
        materials.eraseUnordered(material);
        delete material;
        material = nullptr;
    }
}

void Renderer::removeGeometry(Geometry* geometry)
{
    if(geometry)
    {
        geometries.eraseUnordered(geometry);
        delete geometry;
        geometry = nullptr;
    }
}

Texture* Renderer::createMaterialTexture(const std::string& texFileName, TextureSlotIndex slotIndex)
{
    unsigned int fileNameHash = generateHash((unsigned char*)texFileName.c_str(), texFileName.size());
    int index = materialTextureCache.getIndexToItem([fileNameHash](const TextureCacheItem& item){return item.fileNameHash == fileNameHash;});
    Texture* texture = nullptr;

    if(index == -1)
    {
        TextureLoadResult result = textureLoader.loadFromFile(texFileName, true);

        if(result.pixelData[0])
        {
            texture = graphicSystem->createTexture(TextureTargetMode::Texture2D, TextureWrapMode::Repeat, TextureFilterMode::Trilinear, result.format, result.width, result.height);
            texture->setData(result);
            texture->setSlotIndex(slotIndex);
            //texture->setNumMipMaps(10);
            TextureCacheItem cacheItem;
            cacheItem.fileNameHash = fileNameHash;
            cacheItem.texture = texture;
            materialTextureCache.pushBack(cacheItem);
            graphicSystem->setTexture(texture);
            std::cout << "Loaded Texture: " << texFileName << std::endl;
        }
        else
            std::cout << "Failed to load texture: " << texFileName << std::endl;

        textureLoader.releasedata(result);
    }
    else
        texture = materialTextureCache[index].texture;

    return texture;
}

void Renderer::createFullScreenQuad()
{
    float vertices[] = { -1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f };
    fullScreenQuad = graphicSystem->createVertexData(PrimitiveType::TriangleStrip, 4);
    graphicSystem->setAttributesToVertexData(fullScreenQuad, AttributeType::Float, AttributeSemantic::Position, 3, 12, vertices);
}

}