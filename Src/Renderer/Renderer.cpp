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
#include "Renderer/Renderer.h"
#include "Renderer/Geometry.h"
#include "Graphics/GraphicSystem.h"
#include "Scene/Scene.h"
#include "Scene/Camera.h"
#include "Scene/Light.h"
#include "Scene/Mesh.h"
#include "Scene/SkyBox.h"
#include "Math/Frustum.h"
#include <iostream>

namespace Huurre3D
{

Renderer::Renderer(void):
deferredStage(this),
shadowStage(this),
lightingStage(this),
postProcessStage(this)
{
    graphicWindow = new GraphicWindow();
    graphicSystem = new GraphicSystem();
}

Renderer::~Renderer(void)
{
    //graphicSystem->removeShaderParameterBlock(cameraShaderParameterBlock);
    //graphicSystem->removeShaderParameterBlock(materialParameterBlock);
    deferredStage.deInit();
    materials.reset();
    geometries.reset();
    delete graphicSystem;
    delete graphicWindow;
}

bool Renderer::createRenderWindow(int width, int height, const std::string& windowTitle, bool fullscreen, bool vsync)
{
    bool success = graphicWindow->create(width, height, windowTitle, fullscreen, vsync);
    if(success)
    {
        cameraShaderParameterBlock = graphicSystem->createShaderParameterBlock(sp_cameraParameters);
        materialParameterBlock = graphicSystem->createShaderParameterBlock(sp_materialProperties);
        createFullScreenQuad();
        screenViewPort.set(0, 0, width, height);
        deferredStage.init();
        shadowStage.init();
        lightingStage.init(LightTileWidth, LightTileHeight);
        postProcessStage.init();
    }
    return success;
}

void Renderer::resizeRenderWindow(int width, int height)
{
    screenViewPort.set(0, 0, width, height);
    deferredStage.resizeResources();
    shadowStage.resizeResources();
    lightingStage.resizeResources();
    postProcessStage.resizeResources();
}

void Renderer::renderScene(Scene* scene, Camera* camera)
{
    scene->update();

    deferredRenderItems.clear();
    shadowRenderItems.clear();
    frustumLights.clear();
    shadowLights.clear();

    deferredStage.clearStage();
    shadowStage.clearStage();

    camera->getCameraShaderParameterBlock(cameraShaderParameterBlock);

    Frustum worldSpaceCameraViewFrustum = camera->getViewFrustumInWorldSpace();

    auto deferredItemsCullResult = workQueue.submitTask([this, scene, worldSpaceCameraViewFrustum](){sceneCuller.cullRenderItems(scene, deferredRenderItems, worldSpaceCameraViewFrustum); });
    //sceneCuller.cullRenderItems(scene, deferredRenderItems, worldSpaceCameraViewFrustum);
    
    scene->getAllRenderItems(shadowRenderItems);
    sceneCuller.cullLights(scene, frustumLights, worldSpaceCameraViewFrustum);

    frustumLights.findItems([](const Light* light) {return light->getCastShadow(); }, shadowLights);

    SkyBox* skyBox = (SkyBox*)scene->getSceneItem("SkyBox");

    for(unsigned int i = 0; i < shadowLights.size(); ++i)
    {
        int mask = 1 << i;
        shadowLights[i]->setShadowOcclusionMask(mask);
    }

    deferredItemsCullResult.wait();
    auto deferredStageResult = workQueue.submitTask([this](){deferredStage.setData(deferredRenderItems); });
    //deferredStage.setData(deferredRenderItems);

    auto shadowStageResult = workQueue.submitTask([this, camera](){shadowStage.setData(shadowRenderItems, shadowLights, camera); });
    //shadowStage.setData(shadowRenderItems, shadowLights, camera);

    Vector3 globalAmbientLight = scene->getGlobalAmbientLight();
    auto lightingStageResult = workQueue.submitTask([this, globalAmbientLight, camera](){lightingStage.setData(frustumLights, globalAmbientLight, camera); });
    //lightingStage.setData(frustumLights, globalAmbientLight, camera);

    if(skyBox)
        postProcessStage.setSkybox(skyBox);

    deferredStageResult.wait();
    deferredStage.execute();

    shadowStageResult.wait();
    shadowStage.execute();

    lightingStageResult.wait();
    lightingStage.execute();

    postProcessStage.execute();

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

    Material* material = new Material(parameters, materialDescription.rasterState);

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
    shaderFileNames.pushBack("Gbuffer.vert");
    shaderFileNames.pushBack("Gbuffer.frag");
    Vector<ShaderDefine> materialShaderDefines = material->getShaderDefines();

    ShaderProgram* program = graphicSystem->getShaderCombination(shaderFileNames, materialShaderDefines);

    //If the program dont exist, create one.
    if(!program)
    {
        Shader* vShader = graphicSystem->createShader(ShaderType::Vertex, Engine::getShaderPath() + "Gbuffer.vert");
        Shader* fShader = graphicSystem->createShader(ShaderType::Fragment, Engine::getShaderPath() + "Gbuffer.frag");
        fShader->setDefines(materialShaderDefines);
        program = graphicSystem->createShaderProgram(vShader, fShader);
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
        vd->setAttributeBuffer(AttributeType::Float, AttributeSemantic::Position, 3, geometryDescription.numVertices * 3, geometryDescription.vertices, false, false);
    }
    if(geometryDescription.normals)
        vd->setAttributeBuffer(AttributeType::Float, AttributeSemantic::Normal, 3, geometryDescription.numVertices * 3, geometryDescription.normals, false, false);

    if(geometryDescription.tangents)
        vd->setAttributeBuffer(AttributeType::Float, AttributeSemantic::Tangent, 3, geometryDescription.numVertices * 3, geometryDescription.tangents, false, false);

    if(geometryDescription.bitTangents)
        vd->setAttributeBuffer(AttributeType::Float, AttributeSemantic::BiTanget, 3, geometryDescription.numVertices * 3, geometryDescription.bitTangents, false, false);

    for(int i = 0; i < geometryDescription.numUVChanels; ++i)
    {
        if(geometryDescription.texCoords[i])
        {
            int numComp = geometryDescription.numComp[i];
            AttributeSemantic semantic = static_cast<AttributeSemantic>(int(AttributeSemantic::TexCoord0) + i);
            vd->setAttributeBuffer(AttributeType::Float, semantic, numComp, geometryDescription.numVertices * numComp, geometryDescription.texCoords[i], false, false);
        }
    }

    if(geometryDescription.indices16)
        vd->setIndexBuffer(IndexType::Short, geometryDescription.numIndices, geometryDescription.indices16, false);
    else if(geometryDescription.indices32)
        vd->setIndexBuffer(IndexType::Int, geometryDescription.numIndices, geometryDescription.indices32, false);

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
        std::cout << "Loading image: " << texFileName << std::endl;
        TextureLoadResult result = textureLoader.loadFromFile(texFileName, true);

        if(result.pixelData[0])
        {
            texture = graphicSystem->createTexture(TextureTargetMode::Texture2D, TextureWrapMode::Repeat, TextureFilterMode::Trilinear, result.format, result.width, result.height);
            texture->setData(result);
            texture->setSlotIndex(slotIndex);
            texture->setMipMapLevel(10);
            TextureCacheItem cacheItem;
            cacheItem.fileNameHash = fileNameHash;
            cacheItem.texture = texture;
            materialTextureCache.pushBack(cacheItem);
        }

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
    fullScreenQuad->setAttributeBuffer(AttributeType::Float, AttributeSemantic::Position, 3, 12, vertices);
}

}