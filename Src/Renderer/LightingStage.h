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

#ifndef LightingStage_H
#define LightingStage_H

#include "Renderer/RenderStage.h"
#include "Renderer/LightTileGrid.h"

namespace Huurre3D
{

class Light;
class Camera;

struct LightGridParameterValue
{
    FixedArray<int, 4> gridDimensions;
    Vector2 renderTargetSize;
};

struct SSAOParameterValue
{
    Vector4 SAOParameters = Vector4(500.0f, 0.55f, 0.026f, 5.0f);;
    Vector2 renderTargetSize;
};

class LightingStage : public RenderStage
{
public:
    LightingStage(Renderer* renderer);
    ~LightingStage() = default;
    
    void init(int tileWidth, int tileHeight, bool ssao, bool hdr);
    void resizeResources();
    void setData(const Vector<Light*>& lights, const Vector3& globalAmbientLight, Camera* camera);

private:
    LightTileGrid tileGrid;
};

}

#endif

