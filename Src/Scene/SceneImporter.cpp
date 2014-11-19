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
#include "Scene/SceneImporter.h"
#include "Scene/Scene.h"
#include "Scene/Mesh.h"
#include "Scene/Light.h"
#include "Math/Quaternion.h"
#include "Math/Vector3.h"
#include "Renderer/Material.h"
#include "Renderer/Geometry.h"
#include <iostream>

namespace Huurre3D
{

SceneImporter::SceneImporter(Renderer* renderer):
renderer(renderer)
{
}

void SceneImporter::importMultipleMeshes(const std::string& fileName, Vector<Mesh*>& destMeshes)
{
    if(isAssimpFileType(fileName.c_str()))
    {
        if(importAssimpFile(fileName.c_str()))
        {
            Vector<MaterialDescription> materialDescriptions;
            Vector<GeometryDescription> geometryDescriptions;
            const aiNode* rootNode = assimpScene->mRootNode;
            readAssimpModelIntoDescriptions(rootNode, materialDescriptions, geometryDescriptions);

            Vector3 position, scale;
            Quaternion rotation;
            getTransfrom(rootNode->mTransformation, position, rotation, scale);

            Vector<Vector<RenderItem>> renderItems;
            renderer->createRenderItems(materialDescriptions, geometryDescriptions, renderItems, destMeshes.size());

            for(unsigned int i = 0; i < destMeshes.size(); ++i)
                destMeshes[i]->addRenderItems(renderItems[i]);

            for(unsigned int i = 0; i < geometryDescriptions.size(); ++i)
                geometryDescriptions[i].releaseData();

        }
    }
}

void SceneImporter::importMesh(const std::string& fileName, Mesh* destMesh)
{
    if(isAssimpFileType(fileName.c_str()))
    {
        if(importAssimpFile(fileName.c_str()))
        {
            Vector<MaterialDescription> materialDescriptions;
            Vector<GeometryDescription> geometryDescriptions;
            const aiNode* rootNode = assimpScene->mRootNode;
            readAssimpModelIntoDescriptions(rootNode, materialDescriptions, geometryDescriptions);

            Vector3 position, scale;
            Quaternion rotation;
            getTransfrom(rootNode->mTransformation, position, rotation, scale);
            destMesh->setTransform(position, rotation, scale);

            Vector<RenderItem> renderItems;
            renderer->createRenderItems(materialDescriptions, geometryDescriptions, renderItems);
            destMesh->addRenderItems(renderItems);

            for(unsigned int i = 0; i < geometryDescriptions.size(); ++i)
                geometryDescriptions[i].releaseData();

        }
    }
}

void SceneImporter::readAssimpModelIntoDescriptions(const aiNode* assimpNode, Vector<MaterialDescription>& materialDescriptions,
    Vector<GeometryDescription>& geometryDescriptions)
{
    if(assimpNode->mNumMeshes > 0)
    {
        for(unsigned int i = 0; i < assimpNode->mNumMeshes; ++i)
        {
            const aiMesh* assimpMesh = assimpScene->mMeshes[assimpNode->mMeshes[i]];
            const aiMaterial* assimpMaterial = assimpScene->mMaterials[assimpMesh->mMaterialIndex];

            GeometryDescription description;
            description.primitiveType = PrimitiveType::Triangles;
            readIndices(assimpMesh, description);
            readVertexAttributes(assimpMesh, description);

            materialDescriptions.pushBack(createMaterialDescription(assimpMaterial));
            geometryDescriptions.pushBack(description);
        }
    }

    //TODO: bake the child transforms into the attributes.
    for(unsigned int i = 0; i < assimpNode->mNumChildren; ++i)
    {
        readAssimpModelIntoDescriptions(assimpNode->mChildren[i], materialDescriptions, geometryDescriptions);
    }
}

MaterialDescription SceneImporter::createMaterialDescription(const aiMaterial* assimpMaterial)
{
    MaterialDescription description;

    aiColor3D diffuse(1.0f, 1.0f, 1.0f);
    aiColor3D specular(1.0f, 1.0f, 1.0f);
    aiColor3D ambient(1.0f, 1.0f, 1.0f);
    aiColor3D emissive(0.0f, 0.0f, 0.0f);
    aiString textureName;
    float alpha = 0.0f;
    float roughness = 0.0f;
    float reflectance = 0.0f;
    float shininess = 0.0f;
    //float specularStrength = 1.0f;
    int twoSided;

    if(assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse) == AI_SUCCESS)
        description.diffuseColor = Vector3(diffuse.r, diffuse.g, diffuse.b);

    if(assimpMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specular) == AI_SUCCESS)
        description.specularColor = Vector3(specular.r, specular.g, specular.b);

    if(assimpMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambient) == AI_SUCCESS)
        description.ambientColor = Vector3(ambient.r, ambient.g, ambient.b);
    
    if(assimpMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, emissive) == AI_SUCCESS)
        description.emissiveColor = Vector3(emissive.r, emissive.g, emissive.b);

    /*if(assimpMaterial->Get(AI_MATKEY_SHININESS_STRENGTH, specularStrength)== AI_SUCCESS)
        materialOut->setSpecularPower(specularStrength);*/

    if(assimpMaterial->Get(AI_MATKEY_OPACITY, alpha) == AI_SUCCESS)
        description.alpha = alpha;

    if(assimpMaterial->Get(AI_MATKEY_REFLECTIVITY, reflectance) == AI_SUCCESS)
        description.reflectance = reflectance;

    textureName.Clear();
    if(assimpMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), textureName) == AI_SUCCESS)
    {
        description.diffuseTextureFile = Engine::getAssetPath() + std::string(textureName.C_Str());

        //Sometimes when material has a diffuse texture, the diffuse color has been set to zero,
        //in these cases set the diffuse color to one, so that the color of the diffuse texture will be the final diffuse color.
        if(description.diffuseColor == Vector3::ZERO)
            description.diffuseColor = DefaultColor;
    }

    textureName.Clear();
    if(assimpMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_SPECULAR, 0), textureName) == AI_SUCCESS)
    {
        description.specularTextureFile = Engine::getAssetPath() + std::string(textureName.C_Str());

        //Sometimes when material has a specular texture, the specular color has been set to zero,
        //in these cases set the specular color to one, so that the color of the specular texture will be the final specular color.
        if(description.specularColor == Vector3::ZERO)
            description.specularColor = DefaultColor;
    }

    textureName.Clear();
    if(assimpMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_OPACITY, 0), textureName) == AI_SUCCESS)
        description.alphaTextureFile = Engine::getAssetPath() + std::string(textureName.C_Str());

    textureName.Clear();
    if(assimpMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_NORMALS, 0), textureName) == AI_SUCCESS || assimpMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_HEIGHT, 0), textureName) == AI_SUCCESS)
        description.normalMapTextureFile = Engine::getAssetPath() + std::string(textureName.C_Str());

    if(assimpMaterial->Get(AI_MATKEY_TWOSIDED, twoSided) == AI_SUCCESS)
    {
        if(twoSided != 0)
            description.rasterState.cullState.enabled = false;
    }

    return description;
}

void SceneImporter::readIndices(const aiMesh* assimpMesh, GeometryDescription& geometryDescription)
{
    if(assimpMesh->HasFaces())
    {
        geometryDescription.numIndices = assimpMesh->mNumFaces * 3;

        if(assimpMesh->mNumVertices < 65535)
            geometryDescription.indices16 = new unsigned short[assimpMesh->mNumFaces * 3];
        else
            geometryDescription.indices32 = new unsigned int[assimpMesh->mNumFaces * 3];
    }

    if(assimpMesh->mNumVertices < 65535)
    {
        for(unsigned int i = 0; i < assimpMesh->mNumFaces; ++i)
        {
            geometryDescription.indices16[i * 3] = static_cast<unsigned short>(assimpMesh->mFaces[i].mIndices[0]);
            geometryDescription.indices16[i * 3 + 1] = static_cast<unsigned short>(assimpMesh->mFaces[i].mIndices[1]);
            geometryDescription.indices16[i * 3 + 2] = static_cast<unsigned short>(assimpMesh->mFaces[i].mIndices[2]);
        }
    }
    else
    {
        for(unsigned int i = 0; i < assimpMesh->mNumFaces; ++i)
        {
            geometryDescription.indices32[i * 3] = assimpMesh->mFaces[i].mIndices[0];
            geometryDescription.indices32[i * 3 + 1] = assimpMesh->mFaces[i].mIndices[1];
            geometryDescription.indices32[i * 3 + 2] = assimpMesh->mFaces[i].mIndices[2];
        }
    }
}

void SceneImporter::readVertexAttributes(const aiMesh* assimpMesh, GeometryDescription& geometryDescription)
{
    geometryDescription.numVertices = assimpMesh->mNumVertices;

    if(assimpMesh->HasPositions())
        geometryDescription.vertices = new float[assimpMesh->mNumVertices * 3];

    if(assimpMesh->HasNormals())
        geometryDescription.normals = new float[assimpMesh->mNumVertices * 3];

    if(assimpMesh->HasTangentsAndBitangents())
    {
        geometryDescription.tangents = new float[assimpMesh->mNumVertices * 3];
        geometryDescription.bitTangents = new float[assimpMesh->mNumVertices * 3];
    }

    geometryDescription.numUVChanels = assimpMesh->GetNumUVChannels();
    geometryDescription.texCoords = new float*[geometryDescription.numUVChanels];

    for(int i = 0; i < geometryDescription.numUVChanels; ++i)
    {
        if(assimpMesh->mTextureCoords[i])
            geometryDescription.texCoords[i] = (assimpMesh->mNumUVComponents[i] == 3) ? new float[assimpMesh->mNumVertices * 3] : geometryDescription.texCoords[i] = new float[assimpMesh->mNumVertices * 2];
    }

    for(unsigned int i = 0; i < assimpMesh->mNumVertices; ++i)
    {
        if(assimpMesh->mVertices)
        {
            geometryDescription.vertices[i * 3] = assimpMesh->mVertices[i].x;
            geometryDescription.vertices[i * 3 + 1] = assimpMesh->mVertices[i].y;
            geometryDescription.vertices[i * 3 + 2] = assimpMesh->mVertices[i].z;
        }
        if(assimpMesh->mNormals)
        {
            geometryDescription.normals[i * 3] = assimpMesh->mNormals[i].x;
            geometryDescription.normals[i * 3 + 1] = assimpMesh->mNormals[i].y;
            geometryDescription.normals[i * 3 + 2] = assimpMesh->mNormals[i].z;
        }
        if(assimpMesh->mTangents && assimpMesh->mBitangents)
        {
            //Make sure that the tangents and bit tangents arrays don't have any NaNs. 
            geometryDescription.tangents[i * 3] = isNaN(assimpMesh->mTangents[i].x) ? 0.0f : assimpMesh->mTangents[i].x;
            geometryDescription.tangents[i * 3 + 1] = isNaN(assimpMesh->mTangents[i].y) ? 0.0f : assimpMesh->mTangents[i].y;
            geometryDescription.tangents[i * 3 + 2] = isNaN(assimpMesh->mTangents[i].z) ? 0.0f : assimpMesh->mTangents[i].z;

            geometryDescription.bitTangents[i * 3] = isNaN(assimpMesh->mBitangents[i].x) ? 0.0f : assimpMesh->mBitangents[i].x;
            geometryDescription.bitTangents[i * 3 + 1] = isNaN(assimpMesh->mBitangents[i].y) ? 0.0f : assimpMesh->mBitangents[i].y;
            geometryDescription.bitTangents[i * 3 + 2] = isNaN(assimpMesh->mBitangents[i].z) ? 0.0f : assimpMesh->mBitangents[i].z;
        }
        if(assimpMesh->mTextureCoords)
        {
            for(int j = 0; j < geometryDescription.numUVChanels; ++j)
            {
                if(assimpMesh->mTextureCoords[j])
                {
                    geometryDescription.numComp[j] = assimpMesh->mNumUVComponents[j];
                    if(geometryDescription.numComp[j] == 3)
                    {
                        geometryDescription.texCoords[j][i * 3] = assimpMesh->mTextureCoords[j][i].x;
                        geometryDescription.texCoords[j][i * 3 + 1] = assimpMesh->mTextureCoords[j][i].y;
                        geometryDescription.texCoords[j][i * 3 + 2] = assimpMesh->mTextureCoords[j][i].z;
                    } 
                    else
                    {
                        geometryDescription.texCoords[j][i * 2] = assimpMesh->mTextureCoords[j][i].x;
                        geometryDescription.texCoords[j][i * 2 + 1] = assimpMesh->mTextureCoords[j][i].y;
                    }
                }
            }
        }
    }
}

bool SceneImporter::importAssimpFile(const char* fileName)
{
    assimpScene = assimpImporter.ReadFile(fileName,
        aiProcess_CalcTangentSpace |
        aiProcess_JoinIdenticalVertices |
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_ImproveCacheLocality |
        aiProcess_RemoveRedundantMaterials |
        aiProcess_SortByPType |
        //aiProcess_FixInfacingNormals |
        aiProcess_FindInvalidData |
        aiProcess_GenUVCoords |
        aiProcess_TransformUVCoords |
        aiProcess_OptimizeMeshes);

    if(!assimpScene)
    {
        std::cout << "Failed to import file: " << fileName << ". Error: " <<assimpImporter.GetErrorString() << std::endl;
        return false;
    }

    if(!assimpScene->mRootNode)
    {
        std::cout << "Failed to find rootNode: " << fileName << ". Error: " <<assimpImporter.GetErrorString() << std::endl;
        return false;
    }

    return true;
}

void SceneImporter::getTransfrom(const aiMatrix4x4& transform, Vector3& pos, Quaternion& rot, Vector3& scale)
{
    aiVector3D aiPos;
    aiQuaternion aiRot;
    aiVector3D aiScale;
    transform.Decompose(aiScale, aiRot, aiPos);
    pos = Vector3(aiPos.x, aiPos.y, aiPos.z);
    rot = Quaternion(aiRot.w, aiRot.x, aiRot.y, aiRot.z);
    scale = Vector3(aiScale.x, aiScale.y, aiScale.z);
}

bool SceneImporter::isAssimpFileType(const char* fileName)
{
    const char* ext = ext = strrchr(fileName, '.');
    return assimpImporter.IsExtensionSupported(ext); 
}

}