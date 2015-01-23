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

#ifndef Joint_H
#define Joint_H

#include "Scene/SpatialSceneItem.h"

namespace Huurre3D
{

class ShaderParameterBlock;

class Joint : public SpatialSceneItem
{
    SCENEITEM_TYPE(Joint);

public:
    Joint() = default;
    ~Joint() = default;
    void setName(const std::string& name);
    void setOffsetMatrix(const Vector3& position, const Quaternion& rotation, const Vector3& scale);
    Matrix4x4 getSkinMatrix();
    void updateItem() override;
    const std::string& getName() const {return name;}

private:
    std::string name;
    Matrix4x4 offset = Matrix4x4::IDENTITY;
};

}

#endif