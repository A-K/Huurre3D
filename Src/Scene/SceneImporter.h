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

#ifndef SceneImporterImporter_H
#define SceneImporterImporter_H

#include "Animation/Animation.h"
#include "Renderer/Renderer.h"
#include "Util/Vector.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <set>

namespace Huurre3D
{

class Mesh;
class Vector3;
class Quaternion;

struct AssimpVertexData
{
    int numVertices = 0;
    Vector<float> vertices;
    Vector<float> normals;
    Vector<float> tangents;
    Vector<float> bitTangents;
    Vector<float> jointIndices;
    Vector<float> jointWeights;
    FixedArray<Vector<float>, 4> texCoords;
    FixedArray<int, 4> numComp;
    int numUVChanels = 0;

    IndexType indexType;
    int numIndices = 0;
    Vector<unsigned short> indices16;
    Vector<unsigned int> indices32;
};

struct AssimpSkeletonData
{
    Vector<unsigned int> meshIndices;
    Vector<aiNode*> meshNodes;
    Vector<aiMesh*> boneMeshes;
    std::set<aiNode*> boneNodes;
    Vector<aiBone*> bones;
};

class SceneImporter
{
public:
    SceneImporter(Renderer* renderer, Animation* animation);
    ~SceneImporter() = default;
	
    void importMesh(const std::string& fileName, Mesh* destMesh);
    //Import multiple copies from one model. If the model has animations all meshes share same skeleton and animation clips.
    void importMultipleMeshes(const std::string& fileName, Vector<Mesh*>& destMeshes);

private:
    void extractDataFromAssimpNode(const aiNode* assimpNode, Vector<MaterialDescription>& materialDescriptions, Vector<AssimpVertexData>& assimpVertexDataVec,
        AssimpSkeletonData& assimpSkeletonData) const;
    MaterialDescription createMaterialDescription(const aiMaterial* assimpMaterial) const;
    void createGeometrydescriptions(Vector<AssimpVertexData>& assimpVertexDataVec, Vector<GeometryDescription>& geometryDescriptions) const;
    void readIndices(const aiMesh* assimpMesh, AssimpVertexData& assimpVertexData) const;
    void readVertexAttributes(const aiMesh* assimpMesh, AssimpVertexData& assimpVertexData) const;
    void readJointWeights(const AssimpSkeletonData& assimpSkeletonData, Vector<Joint*>& skeleton, Vector<AssimpVertexData>& assimpVertexDataVec, unsigned int jointStartIndex) const;
    void readSkeleton(const AssimpSkeletonData& assimpSkeletonData, Vector<Joint*>& skeleton) const;
    void buildSkeleton(const aiNode* boneNode, const Vector<aiBone*>& bones, const std::set<aiNode*>& boneNodes, Vector<Joint*>& skeleton, unsigned int &jointId) const;
    void readSkeletalAnimations(Vector<AnimationClip*>& animationClips, const Vector<Joint*>& skeleton) const;
    void getTransfrom(const aiMatrix4x4& assimpTransform, Vector3& pos, Quaternion& rot, Vector3& scale) const;
    Renderer* renderer;
    Animation* animation;
    const aiScene* assimpScene;
};

}

#endif