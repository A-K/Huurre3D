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

#ifndef SceneImporterImporter_H
#define SceneImporterImporter_H

#include "Renderer/Renderer.h"
#include "Util/Vector.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Huurre3D
{

class Mesh;
class Vector3;
class Quaternion;

class SceneImporter
{
public:
    SceneImporter(Renderer* renderer);
    ~SceneImporter() = default;
	
    void importMesh(const std::string&, Mesh* destMesh);
    //Import multiple copies from one model.
    void importMultipleMeshes(const std::string&, Vector<Mesh*>& destMeshes);

private:
    void readAssimpModelIntoDescriptions(const aiNode* assimpNode, Vector<MaterialDescription>& materialDescriptions, Vector<GeometryDescription>& geometryDescriptions);
    MaterialDescription createMaterialDescription(const aiMaterial* assimpMaterial);
    void readIndices(const aiMesh* assimpMesh, GeometryDescription& geometryDescription);
    void readVertexAttributes(const aiMesh* assimpMesh, GeometryDescription& geometryDescription);
    bool importAssimpFile(const char* fileName);
    bool isAssimpFileType(const char* fileName);
    void getTransfrom(const aiMatrix4x4& assimpTransform, Vector3& pos, Quaternion& rot, Vector3& scale);
    Renderer* renderer;
    Assimp::Importer assimpImporter;
    const aiScene* assimpScene;
};

}

#endif