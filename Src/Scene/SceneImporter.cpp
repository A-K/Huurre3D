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
#include "Scene/SceneImporter.h"
#include "Scene/Scene.h"
#include "Scene/Mesh.h"
#include "Scene/Light.h"
#include "Scene/Joint.h"
#include "Math/Quaternion.h"
#include "Math/Vector3.h"
#include "Renderer/Material.h"
#include "Renderer/Geometry.h"
#include <iostream>

namespace Huurre3D
{

SceneImporter::SceneImporter(Renderer& renderer, Animation& animation) :
renderer(renderer),
animation(animation)
{
}

void SceneImporter::importMultipleMeshes(const std::string& fileName, Vector<Mesh*>& destMeshes)
{
    const char* ext = ext = strrchr(fileName.c_str(), '.');
    Assimp::Importer assimpImporter;
    if(assimpImporter.IsExtensionSupported(ext))
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
            aiProcess_LimitBoneWeights |
            aiProcess_SplitByBoneCount |
            aiProcess_GenUVCoords |
            aiProcess_TransformUVCoords |
            aiProcess_OptimizeMeshes);

        if(!assimpScene)
        {
            std::cout << "Failed to import file: " << fileName << ". Error: " << assimpImporter.GetErrorString() << std::endl;
            return;
        }

        if(!assimpScene->mRootNode)
        {
            std::cout << "Failed to find rootNode: " << fileName << ". Error: " << assimpImporter.GetErrorString() << std::endl;
            return;
        }

        Vector<MaterialDescription> materialDescriptions;
        Vector<AssimpVertexData> assimpVertexDataVec;
        AssimpSkeletonData assimpSkeletonData;
        const aiNode* rootNode = assimpScene->mRootNode;
        extractDataFromAssimpNode(rootNode, materialDescriptions, assimpVertexDataVec, assimpSkeletonData);

        Vector3 position, scale;
        Quaternion rotation;
        getTransfrom(rootNode->mTransformation, position, rotation, scale);

        Vector<Joint*> skeleton;
        unsigned int numJoints = destMeshes[0]->getScene()->getNumSceneItemsByType("Joint");
        destMeshes[0]->getScene()->createSceneItems<Joint>(skeleton, assimpSkeletonData.boneNodes.size());
        readSkeleton(assimpSkeletonData, skeleton);
        readJointWeights(assimpSkeletonData, skeleton, assimpVertexDataVec, numJoints);
        Vector<AnimationClip*> animationClips;
        readSkeletalAnimations(animationClips, skeleton);

        Vector<Vector<RenderItem>> renderItems;
        Vector<GeometryDescription> geometryDescriptions;
        geometryDescriptions.reserve(assimpVertexDataVec.size());
        createGeometrydescriptions(assimpVertexDataVec, geometryDescriptions);
        renderer.createRenderItems(materialDescriptions, geometryDescriptions, renderItems, destMeshes.size());

        for(unsigned int i = 0; i < destMeshes.size(); ++i)
        {
            destMeshes[i]->setTransform(position, rotation, scale);
            if(assimpSkeletonData.bones.size() != 0)
            {
                destMeshes[i]->setSkeleton(skeleton);
                destMeshes[i]->setAnimationClips(animationClips);
            }
            destMeshes[i]->addRenderItems(renderItems[i]);
        }
    }
    else
    {
        std::cout << "Failed to import file: " << fileName << ", unsupported format." << std::endl;
        return;
    }
}

void SceneImporter::importMesh(const std::string& fileName, Mesh* destMesh)
{
    Vector<Mesh*> meshes;
    meshes.pushBack(destMesh);
    importMultipleMeshes(fileName, meshes);
}

void SceneImporter::extractDataFromAssimpNode(const aiNode* assimpNode, Vector<MaterialDescription>& materialDescriptions, Vector<AssimpVertexData>& assimpVertexDataVec,
    AssimpSkeletonData& assimpSkeletonData) const
{
    if(assimpNode->mNumMeshes > 0)
    {
        for(unsigned int i = 0; i < assimpNode->mNumMeshes; ++i)
        {
            aiMesh* assimpMesh = assimpScene->mMeshes[assimpNode->mMeshes[i]];
            aiMaterial* assimpMaterial = assimpScene->mMaterials[assimpMesh->mMaterialIndex];

            AssimpVertexData assimpVertexData;
            readIndices(assimpMesh, assimpVertexData);
            readVertexAttributes(assimpMesh, assimpVertexData);

            MaterialDescription materialDescription = createMaterialDescription(assimpMaterial);
            
            if(assimpMesh->HasBones())
            {
                materialDescription.skinned = true;
                assimpSkeletonData.boneMeshes.pushBack(assimpMesh);
                assimpSkeletonData.meshIndices.pushBack(i);
            }

            materialDescriptions.pushBack(materialDescription);
            assimpVertexDataVec.pushBack(assimpVertexData);

            //Add new bones and bone nodes into the assimpSkeletonData struct.
            for(unsigned int j = 0; j < assimpScene->mMeshes[i]->mNumBones; ++j)
            {
                assimpSkeletonData.bones.pushBack(assimpScene->mMeshes[i]->mBones[j]);
                aiNode* boneNode = assimpScene->mRootNode->FindNode(assimpScene->mMeshes[i]->mBones[j]->mName);

                if(boneNode)
                {
                    while(boneNode && boneNode != assimpNode && boneNode != assimpNode->mParent)
                    {
                        assimpSkeletonData.boneNodes.insert(boneNode);                            
                        boneNode = boneNode->mParent;
                    }
                }
            }
        }
    }

    //TODO: bake the child transforms into the attributes.
    for(unsigned int i = 0; i < assimpNode->mNumChildren; ++i)
    {
        extractDataFromAssimpNode(assimpNode->mChildren[i], materialDescriptions, assimpVertexDataVec, assimpSkeletonData);
    }
}

MaterialDescription SceneImporter::createMaterialDescription(const aiMaterial* assimpMaterial) const
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
void SceneImporter::createGeometrydescriptions(Vector<AssimpVertexData>& assimpVertexDataVec, Vector<GeometryDescription>& geometryDescriptions) const
{
    for(unsigned int i = 0; i < assimpVertexDataVec.size(); ++i)
    {
        GeometryDescription geometryDescription;
        Vector<float*> attributes;

        geometryDescription.numVertices = assimpVertexDataVec[i].numVertices;

        if(assimpVertexDataVec[i].numIndices != 0)
        {
            geometryDescription.indexType = assimpVertexDataVec[i].indexType;
            geometryDescription.numIndices = assimpVertexDataVec[i].numIndices;
            
            if(assimpVertexDataVec[i].indexType == IndexType::Short)
                geometryDescription.indices = std::move(assimpVertexDataVec[i].indices16.getMemoryBuffer());
            else
                geometryDescription.indices = std::move(assimpVertexDataVec[i].indices32.getMemoryBuffer());
        }

        if(!assimpVertexDataVec[i].vertices.empty())
        {
            geometryDescription.boundingBox.mergePoints(assimpVertexDataVec[i].vertices.getData(), geometryDescription.numVertices);
            geometryDescription.attributeDescriptions.pushBack({ AttributeType::Float, AttributeSemantic::Position, 3, 3 * attributeSize[static_cast<int>(AttributeType::Float)], false });
            attributes.pushBack(assimpVertexDataVec[i].vertices.getData());
        }

        if(!assimpVertexDataVec[i].normals.empty())
        {
            geometryDescription.attributeDescriptions.pushBack({ AttributeType::Float, AttributeSemantic::Normal, 3, 3 * attributeSize[static_cast<int>(AttributeType::Float)], false });
            attributes.pushBack(assimpVertexDataVec[i].normals.getData());
        }

        if(!assimpVertexDataVec[i].tangents.empty())
        {
            geometryDescription.attributeDescriptions.pushBack({ AttributeType::Float, AttributeSemantic::Tangent, 3, 3 * attributeSize[static_cast<int>(AttributeType::Float)], false });
            attributes.pushBack(assimpVertexDataVec[i].tangents.getData());
        }

        if(!assimpVertexDataVec[i].bitTangents.empty())
        {
            geometryDescription.attributeDescriptions.pushBack({ AttributeType::Float, AttributeSemantic::BiTanget, 3, 3 * attributeSize[static_cast<int>(AttributeType::Float)], false });
            attributes.pushBack(assimpVertexDataVec[i].bitTangents.getData());
        }

        if(!assimpVertexDataVec[i].jointIndices.empty())
        {
            geometryDescription.attributeDescriptions.pushBack({ AttributeType::Float, AttributeSemantic::JointIndices, 4, 4 * attributeSize[static_cast<int>(AttributeType::Float)], false });
            attributes.pushBack(assimpVertexDataVec[i].jointIndices.getData());
        }

        if(!assimpVertexDataVec[i].jointWeights.empty())
        {
            geometryDescription.attributeDescriptions.pushBack({ AttributeType::Float, AttributeSemantic::JointWeights, 4, 4 * attributeSize[static_cast<int>(AttributeType::Float)], false });
            attributes.pushBack(assimpVertexDataVec[i].jointWeights.getData());
        }

        for(int j = 0; j < assimpVertexDataVec[i].numUVChanels; ++j)
        {
            if(!assimpVertexDataVec[i].texCoords[j].empty())
            {
                int numComp = assimpVertexDataVec[i].numComp[j];
                AttributeSemantic semantic = static_cast<AttributeSemantic>(int(AttributeSemantic::TexCoord0) + j);
                geometryDescription.attributeDescriptions.pushBack({ AttributeType::Float, semantic, numComp, numComp * attributeSize[static_cast<int>(AttributeType::Float)], false });
                attributes.pushBack(assimpVertexDataVec[i].texCoords[j].getData());
            }
        }

        int vertexSize = 0;
        for(unsigned int j = 0; j < geometryDescription.attributeDescriptions.size(); ++j)
            vertexSize += geometryDescription.attributeDescriptions[j].stride;

        geometryDescription.vertexData.reserve(vertexSize * assimpVertexDataVec[i].numVertices);

        //Interleave attributes.
        for(unsigned int k = 0; k < assimpVertexDataVec[i].numVertices; ++k)
        {
            for(unsigned int n = 0; n < geometryDescription.attributeDescriptions.size(); ++n)
                geometryDescription.vertexData.append(std::move(&attributes[n][k * geometryDescription.attributeDescriptions[n].numComponentsPerVertex]), geometryDescription.attributeDescriptions[n].stride);

        }

        geometryDescriptions.pushBack(std::move(geometryDescription));
    }
}

void SceneImporter::readIndices(const aiMesh* assimpMesh, AssimpVertexData& assimpVertexData) const
{
    if(assimpMesh->HasFaces())
    {
        assimpVertexData.numIndices = assimpMesh->mNumFaces * 3;
        if(assimpMesh->mNumVertices < 65535)
        {
            assimpVertexData.indexType = IndexType::Short;
            assimpVertexData.indices16 = Vector<unsigned short>(assimpMesh->mNumFaces * 3);
        }
        else
        {
            assimpVertexData.indexType = IndexType::Int;
            Vector<unsigned int>(assimpMesh->mNumFaces * 3);
        }

        for(unsigned int i = 0; i < assimpMesh->mNumFaces; ++i)
        {
            if(assimpVertexData.indexType == IndexType::Short)
            {
                assimpVertexData.indices16[i * 3] = static_cast<unsigned short>(assimpMesh->mFaces[i].mIndices[0]);
                assimpVertexData.indices16[i * 3 + 1] = static_cast<unsigned short>(assimpMesh->mFaces[i].mIndices[1]);
                assimpVertexData.indices16[i * 3 + 2] = static_cast<unsigned short>(assimpMesh->mFaces[i].mIndices[2]);
            }
            else
            {
                assimpVertexData.indices32[i * 3] = assimpMesh->mFaces[i].mIndices[0];
                assimpVertexData.indices32[i * 3 + 1] = assimpMesh->mFaces[i].mIndices[1];
                assimpVertexData.indices32[i * 3 + 2] = assimpMesh->mFaces[i].mIndices[2];
            }
        }
    }
}

void SceneImporter::readVertexAttributes(const aiMesh* assimpMesh, AssimpVertexData& assimpVertexData) const
{
    assimpVertexData.numVertices = assimpMesh->mNumVertices;

    if(assimpMesh->HasPositions())
        assimpVertexData.vertices = Vector<float>(assimpMesh->mNumVertices * 3);

    if(assimpMesh->HasNormals())
        assimpVertexData.normals = Vector<float>(assimpMesh->mNumVertices * 3);

    if(assimpMesh->HasTangentsAndBitangents())
    {
        assimpVertexData.tangents = Vector<float>(assimpMesh->mNumVertices * 3);
        assimpVertexData.bitTangents = Vector<float>(assimpMesh->mNumVertices * 3);
    }

    assimpVertexData.numUVChanels = assimpMesh->GetNumUVChannels();
    for(int i = 0; i < assimpVertexData.numUVChanels; ++i)
    {
        if(assimpMesh->mTextureCoords[i])
            assimpVertexData.texCoords[i] = (assimpMesh->mNumUVComponents[i] == 3) ? Vector<float>(assimpMesh->mNumVertices * 3) : Vector<float>(assimpMesh->mNumVertices * 2);
    }

    for(unsigned int i = 0; i < assimpMesh->mNumVertices; ++i)
    {
        if(assimpMesh->mVertices)
        {
            assimpVertexData.vertices[i * 3] = assimpMesh->mVertices[i].x;
            assimpVertexData.vertices[i * 3 + 1] = assimpMesh->mVertices[i].y;
            assimpVertexData.vertices[i * 3 + 2] = assimpMesh->mVertices[i].z;
        }
        if(assimpMesh->mNormals)
        {
            assimpVertexData.normals[i * 3] = assimpMesh->mNormals[i].x;
            assimpVertexData.normals[i * 3 + 1] = assimpMesh->mNormals[i].y;
            assimpVertexData.normals[i * 3 + 2] = assimpMesh->mNormals[i].z;
        }
        if(assimpMesh->mTangents && assimpMesh->mBitangents)
        {
            //Make sure that the tangents and bit tangents arrays don't have any NaNs. 
            assimpVertexData.tangents[i * 3] = isNaN(assimpMesh->mTangents[i].x) ? 0.0f : assimpMesh->mTangents[i].x;
            assimpVertexData.tangents[i * 3 + 1] = isNaN(assimpMesh->mTangents[i].y) ? 0.0f : assimpMesh->mTangents[i].y;
            assimpVertexData.tangents[i * 3 + 2] = isNaN(assimpMesh->mTangents[i].z) ? 0.0f : assimpMesh->mTangents[i].z;

            assimpVertexData.bitTangents[i * 3] = isNaN(assimpMesh->mBitangents[i].x) ? 0.0f : assimpMesh->mBitangents[i].x;
            assimpVertexData.bitTangents[i * 3 + 1] = isNaN(assimpMesh->mBitangents[i].y) ? 0.0f : assimpMesh->mBitangents[i].y;
            assimpVertexData.bitTangents[i * 3 + 2] = isNaN(assimpMesh->mBitangents[i].z) ? 0.0f : assimpMesh->mBitangents[i].z;
        }
        if(assimpMesh->mTextureCoords)
        {
            for(int j = 0; j < assimpVertexData.numUVChanels; ++j)
            {
                if(assimpMesh->mTextureCoords[j])
                {
                    assimpVertexData.numComp[j] = assimpMesh->mNumUVComponents[j];
                    if(assimpVertexData.numComp[j] == 3)
                    {
                        assimpVertexData.texCoords[j][i * 3] = assimpMesh->mTextureCoords[j][i].x;
                        assimpVertexData.texCoords[j][i * 3 + 1] = assimpMesh->mTextureCoords[j][i].y;
                        assimpVertexData.texCoords[j][i * 3 + 2] = assimpMesh->mTextureCoords[j][i].z;
                    } 
                    else
                    {
                        assimpVertexData.texCoords[j][i * 2] = assimpMesh->mTextureCoords[j][i].x;
                        assimpVertexData.texCoords[j][i * 2 + 1] = assimpMesh->mTextureCoords[j][i].y;
                    }
                }
            }
        }
    }
}

void SceneImporter::readJointWeights(const AssimpSkeletonData& assimpSkeletonData, Vector<Joint*>& skeleton, Vector<AssimpVertexData>& assimpVertexDataVec, unsigned int jointStartIndex) const
{
    for(unsigned int i = 0; i < assimpSkeletonData.boneMeshes.size(); ++i)
    {
        assimpVertexDataVec[assimpSkeletonData.meshIndices[i]].jointIndices = Vector<float>(assimpSkeletonData.boneMeshes[i]->mNumVertices * 4);
        assimpVertexDataVec[assimpSkeletonData.meshIndices[i]].jointWeights = Vector<float>(assimpSkeletonData.boneMeshes[i]->mNumVertices * 4);
        assimpVertexDataVec[assimpSkeletonData.meshIndices[i]].jointIndices.fill(0.0f);
        assimpVertexDataVec[assimpSkeletonData.meshIndices[i]].jointWeights.fill(0.0f);

        std::string boneName;
        //vertexIDTable keeps track that how many jointIndices and weights a vertex in given index has. 
        Vector<int> vertexIDtable(assimpSkeletonData.boneMeshes[i]->mNumVertices);
        vertexIDtable.fill(0);
        for(unsigned int j = 0; j < assimpSkeletonData.boneMeshes[i]->mNumBones; ++j)
        {
            boneName = assimpSkeletonData.boneMeshes[i]->mBones[j]->mName.C_Str();
            unsigned int jointIndex = jointStartIndex;
            jointIndex += skeleton.getIndexToItem([boneName](const Joint* joint){return joint->getName().compare(boneName) == 0; });

            for(unsigned int k = 0; k < assimpSkeletonData.boneMeshes[i]->mBones[j]->mNumWeights; ++k)
            {
                unsigned int vertexIndex = assimpSkeletonData.boneMeshes[i]->mBones[j]->mWeights[k].mVertexId;
                assimpVertexDataVec[assimpSkeletonData.meshIndices[i]].jointIndices[vertexIndex * 4 + vertexIDtable[vertexIndex]] = static_cast<float>(jointIndex);
                assimpVertexDataVec[assimpSkeletonData.meshIndices[i]].jointWeights[vertexIndex * 4 + vertexIDtable[vertexIndex]] = assimpSkeletonData.boneMeshes[i]->mBones[j]->mWeights[k].mWeight;
                vertexIDtable[vertexIndex] = vertexIDtable[vertexIndex] + 1;
            }
        }
    }
}

void SceneImporter::readSkeleton(const AssimpSkeletonData& assimpSkeletonData, Vector<Joint*>& skeleton) const
{
    //Find the skeleton root Node. Node that is closest to the scene root node is the skeleton root node.
    aiNode* boneRoot = nullptr;
    unsigned int distanceToRoot = 0;
    unsigned int shortestDistToRoot = 10000;
    for(auto &bn : assimpSkeletonData.boneNodes)
    {
        distanceToRoot = 0;
        aiNode* tempNode = bn;
        while(tempNode != assimpScene->mRootNode)
        {
            tempNode = tempNode->mParent;
            distanceToRoot++;
        }

        if(distanceToRoot < shortestDistToRoot)
        {
            boneRoot = bn;
            shortestDistToRoot = distanceToRoot;
        }
    }
    unsigned int jointId = 0;
    buildSkeleton(boneRoot, assimpSkeletonData.bones, assimpSkeletonData.boneNodes, skeleton, jointId);
}

void SceneImporter::buildSkeleton(const aiNode* boneNode, const Vector<aiBone*>& bones, const std::set<aiNode*>& boneNodes, Vector<Joint*>& skeleton, unsigned int &jointId) const
{
    if(!boneNode)
        return;

    //Set the name, local transform and offset matrix to the joint.
    Vector3 position, scale;
    Quaternion rotation;
    getTransfrom(boneNode->mTransformation, position, rotation, scale);
    skeleton[jointId]->setTransform(position, rotation, scale);

    aiBone* bone;
    aiString boneName = boneNode->mName;
    Joint* curJoint = skeleton[jointId];
    curJoint->setName(boneName.C_Str());

    if(bones.findItem([boneName](aiBone* bone){return bone->mName == boneName; }, bone))
    {
        aiMatrix4x4 offsetMatrix = bone->mOffsetMatrix;
        getTransfrom(offsetMatrix, position, rotation, scale);
        curJoint->setOffsetMatrix(position, rotation, scale);
    }

    for(unsigned int i = 0; i < boneNode->mNumChildren; ++i)
    {
        //On some occcasion the bone node might have children nodes which don't have bones and have no effect on the skeleton.
        //Don't add them to the skeleton hierarchy.
        if(boneNodes.find(boneNode->mChildren[i]) != boneNodes.end())
        {
            jointId++;
            curJoint->addChild(skeleton[jointId]);
            buildSkeleton(boneNode->mChildren[i], bones, boneNodes, skeleton, jointId);
        }
    }
}


void SceneImporter::readSkeletalAnimations(Vector<AnimationClip*>& animationClips, const Vector<Joint*>& skeleton) const
{
    for(unsigned int i = 0; i < assimpScene->mNumAnimations; ++i)
    {
        std::string name = std::string(assimpScene->mAnimations[i]->mName.data, assimpScene->mAnimations[i]->mName.length);
        if(name.size() == 0)
            name = "Animation" + std::to_string(i);
        
        float ticksPerSecond = static_cast<float>(assimpScene->mAnimations[i]->mTicksPerSecond);
        ticksPerSecond = ticksPerSecond != 0.0f ? ticksPerSecond : 60.0f;
        float invTicksperSecond = (1.0f / ticksPerSecond);
        float animationLenght = static_cast<float>(assimpScene->mAnimations[i]->mDuration) * invTicksperSecond;

        int tracknum = 0;
        Vector<Track> tracks;
        for(unsigned int j = 0; j < assimpScene->mAnimations[i]->mNumChannels; ++j)
        {
            aiNodeAnim* channel = assimpScene->mAnimations[i]->mChannels[j];
            std::string channelName(channel->mNodeName.C_Str());

            //Last key frame values are needed because the aiProcess_FindInvalidData flags removes redundant keyframe values from animation channel. 
            //This leads to differing amounts of position, rotation and scaling keys. 
            //If only one of those values per keyFrame is setted and others are left to they default values, 
            //it causes incorrect transformations between the joints during the animation.
            Vector3 lastKeyFramePosition;
            Quaternion lastKeyFrameRotation;
            Vector3 lastKeyFrameScale;
            
            unsigned int numKeys = channel->mNumPositionKeys;
            if(channel->mNumPositionKeys != channel->mNumRotationKeys || channel->mNumPositionKeys != channel->mNumScalingKeys || channel->mNumRotationKeys != channel->mNumScalingKeys)
                numKeys = max((int)numKeys, max((int)channel->mNumRotationKeys, (int)channel->mNumScalingKeys));

            Joint* jointToModify;
            if(skeleton.findItem([channelName](Joint* joint){return joint->getName().compare(channelName) == 0; }, jointToModify))
            {
                Track track(*jointToModify);
                for(unsigned int k = 0; k < numKeys; ++k)
                {
                    KeyFrame keyFrame;
                    if(channel->mNumPositionKeys > k)
                    {
                        keyFrame.time = static_cast<float>(channel->mPositionKeys[k].mTime) * invTicksperSecond;
                        keyFrame.position = Vector3(channel->mPositionKeys[k].mValue.x, channel->mPositionKeys[k].mValue.y, channel->mPositionKeys[k].mValue.z);
                        lastKeyFramePosition = keyFrame.position;
                    }
                    else
                        keyFrame.position = lastKeyFramePosition;

                    if(channel->mNumRotationKeys > k)
                    {
                        keyFrame.time = static_cast<float>(channel->mRotationKeys[k].mTime) * invTicksperSecond;
                        keyFrame.rotation = Quaternion(channel->mRotationKeys[k].mValue.w, channel->mRotationKeys[k].mValue.x, channel->mRotationKeys[k].mValue.y, channel->mRotationKeys[k].mValue.z);
                        lastKeyFrameRotation = keyFrame.rotation;
                    }
                    else
                        keyFrame.rotation = lastKeyFrameRotation;

                    if(channel->mNumScalingKeys > k)
                    {
                        keyFrame.time = static_cast<float>(channel->mScalingKeys[k].mTime) * invTicksperSecond;
                        keyFrame.scale = Vector3(channel->mScalingKeys[k].mValue.x, channel->mScalingKeys[k].mValue.y, channel->mScalingKeys[k].mValue.z);
                        lastKeyFrameScale = keyFrame.scale;
                    }
                    else
                        keyFrame.scale = lastKeyFrameScale;

                    track.keyFrames.pushBack(keyFrame);
                }
                tracks.pushBack(track);
                tracknum++;
            }
            else
            {
                std::cout << "Channel " << channelName << " not included into the animation, because it has no effect on the skeleton" << std::endl;
            }
        }

        AnimationClip* animationClip = animation.createAnimationClip(name, animationLenght, true, tracks);
        animationClips.pushBack(animationClip);
    }
}

void SceneImporter::getTransfrom(const aiMatrix4x4& transform, Vector3& pos, Quaternion& rot, Vector3& scale) const
{
    aiVector3D aiPos;
    aiQuaternion aiRot;
    aiVector3D aiScale;
    transform.Decompose(aiScale, aiRot, aiPos);
    pos = Vector3(aiPos.x, aiPos.y, aiPos.z);
    rot = Quaternion(aiRot.w, aiRot.x, aiRot.y, aiRot.z);
    scale = Vector3(aiScale.x, aiScale.y, aiScale.z);
}

}