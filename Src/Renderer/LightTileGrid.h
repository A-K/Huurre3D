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

#ifndef LightTileGrid_H
#define LightTileGrid_H

#include "Renderer/RendererDefs.h"
#include "Math/Rect.h"
#include "Math/Vector4.h"
#include "Util/FixedArray.h"
#include "Util/Vector.h"


namespace Huurre3D
{

static const unsigned int TileLightInfoArraySize = MaXNumLights * 2000;
class Light;
class Camera;

struct LightParameterValue
{
    Vector4 position;
    Vector4 color;
    Vector4 direction;
    Vector4 innerOuterAngles;
};

struct LightParameterBlockValues
{
    Vector4 numLightsAndGlobalAmbient;
    LightParameterValue lightParameterValues[MaXNumLights];
};

struct GridDimensions
{
    int tileWidth = 0;
    int tileHeight = 0;
    int widthResolution = 0;
    int heightResolution = 0;
};

struct Tile
{
    Rect rect;
    int numLights;
};

class LightTileGrid
{
public:
    LightTileGrid() = default;
    ~LightTileGrid();
	
    void setGridDimensions(int tileWidth, int tileHeight, int screenWidth, int screenHeight);
    void binLightsToTiles(const Vector<Light*>& lights, Camera* camera);
    const FixedArray<int, TileLightInfoArraySize>& getTileLightInfo() const { return tileLightInfo; }
    const LightParameterBlockValues& getLightParameterBlockValues() const {return lightParameterBlockValues;}
    const GridDimensions& getGridDimensions() const {return gridDimensions;}

private:
    GridDimensions gridDimensions;
    int numTiles = 0;
    Tile* tiles = nullptr;
    LightParameterBlockValues lightParameterBlockValues;
    //TileLightInfo data structure holds the information which lights affect on which tiles. Size is num tiles * num lights.
    //         num lights
    //       0 1000...0  <-light indices affecting tile 1.
    //num    1 1236...n  <-light indices affecting tile 2.
    //tiles  2 1200...0  <-light indices affecting tile 3.
    //       ...
    //       n 0000...0  <-light indices affecting tile n.
    //
    //tile numbers are counted starting from top left corner.
    FixedArray<int, TileLightInfoArraySize> tileLightInfo;
};

}

#endif