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

#ifndef Renderer_H
#define Renderer_H

#include "Graphics/GraphicWindow.h"
#include "Graphics/GraphicSystem.h"
#include "Renderer/Material.h"
#include "Renderer/TextureLoader.h"
#include "Util/WorkQueue.h"
#include "Scene/SceneCuller.h"

namespace Huurre3D
{

static const unsigned int WorkQueueSize = 4; //static_cast<int>(std::thread::hardware_concurrency());

class RenderStage;
class Scene;
class Light;
class Camera;

struct MaterialDescription
{
    Vector3 ambientColor = DefaultColor;
    Vector3 diffuseColor = DefaultColor;
    Vector3 specularColor = DefaultColor;
    Vector3 emissiveColor = Vector3::ZERO;
    float roughness = DefaultRoughness;
    float reflectance = DefaultReflectance;
    float alpha = DefaultAlpha;
    bool skinned = false;
    std::string diffuseTextureFile;
    std::string specularTextureFile;
    std::string normalMapTextureFile;
    std::string alphaTextureFile;

    RasterState rasterState;

    MaterialDescription():
    rasterState(BlendState(false, BlendFunction::Replace), CompareState(true, CompareFunction::Less), CullState(true, CullFace::Back))
    {}
};

struct GeometryDescription
{
    BoundingBox boundingBox;
    PrimitiveType primitiveType = PrimitiveType::Triangles;
    int numVertices = 0;
    Vector<AttributeDescription> attributeDescriptions;
    MemoryBuffer vertexData;
    int numIndices = 0;
    IndexType indexType;
    MemoryBuffer indices;
    GeometryDescription() = default;
    GeometryDescription(GeometryDescription&& geometryDescription)
    {
        boundingBox = geometryDescription.boundingBox;
        primitiveType = geometryDescription.primitiveType;
        numVertices = geometryDescription.numVertices;
        attributeDescriptions = geometryDescription.attributeDescriptions;
        vertexData = std::move(geometryDescription.vertexData);
        numIndices = geometryDescription.numIndices;
        indexType = geometryDescription.indexType;
        indices = std::move(geometryDescription.indices);
    }
};

struct TextureCacheItem
{
    unsigned int fileNameHash;
    Texture* texture;
};

class Renderer
{
public:
    Renderer();
    ~Renderer();

    bool init(const JSONValue& rendererJSON);
    bool createRenderWindow(const JSONValue& renderWindowJSON);
    bool createRenderWindow(int width, int height, const std::string& windowTitle, bool fullscreen = false, bool vsync = true);
    //Resizes all the needed graphics resources allocated by the renderstages.
    void resizeRenderWindow(int width, int height);
    void renderScene(Scene* scene);
    //Creates as many render items as the number of descriptions.
    void createRenderItems(const Vector<MaterialDescription>& materialDescriptions, const Vector<GeometryDescription>& geometryDescriptions, Vector<RenderItem>& renderItemsOut);
    //Creates multiple render items from one material and geometry description.
    void createRenderItems(const Vector<MaterialDescription>& materialDescriptions, const Vector<GeometryDescription>& geometryDescriptions, Vector<Vector<RenderItem>>& renderItemsOut, unsigned int numItemsPerDescription);
    Material* createMaterial(const MaterialDescription& materialDescription);
    void createMaterials(const MaterialDescription& materialDescription, Vector<Material*>& materialsOut, unsigned int numMaterials);
    Geometry* createGeometry(const GeometryDescription& geometryDescription);
    void createGeometries(const GeometryDescription& geometryDescription, Vector<Geometry*>& geometriesOut, unsigned int numGeometries);
    void removeMaterial(Material* material);
    void removeGeometry(Geometry* geometry);
    VertexData* getFullScreenQuad() const {return fullScreenQuad;}
    const ViewPort& getScreenViewPort() const {return screenViewPort;}
    GraphicSystem& getGraphicSystem() {return graphicSystem;}
    const GraphicWindow& getGraphicWindow() const {return graphicWindow;}
    const Vector<unsigned int>& getMaterialBufferIndicies() const {return materialBufferIndicies;}
    const TextureLoader& getTextureLoader() const {return textureLoader;}
   
private:
    Texture* createMaterialTexture(const std::string& texFileName, TextureSlotIndex slotIndex);
    void createFullScreenQuad();

    FixedArray<std::future<void>, 4> stageupdateResults;
    Vector<RenderStage*> renderStages;
    ViewPort screenViewPort;
    VertexData* fullScreenQuad;
    ShaderParameterBlock* cameraShaderParameterBlock;
    ShaderParameterBlock* materialParameterBlock;
    ShaderParameterBlock* renderTargetSizeBlock;
    ShaderParameterBlock* skinMatrixArray;

    Vector<Material*> materials;
    Vector<Geometry*> geometries;
    Vector<unsigned int> materialBufferIndicies;
    Vector<TextureCacheItem> materialTextureCache;

    GraphicSystem graphicSystem;
    GraphicWindow graphicWindow;
    WorkQueue<WorkQueueSize> workQueue;
    TextureLoader textureLoader;
    std::string materialVertexShader;
    std::string materialFragmentShader;
};

}

#endif