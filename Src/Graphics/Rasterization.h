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

#ifndef Rasterization_H
#define Rasterization_H

#include "Graphics/GraphicDefs.h"

namespace Huurre3D
{

struct RasterState
{
    BlendMode blendMode;
    CompareMode compareMode;
    CullMode cullMode;

    RasterState(BlendMode blendMode, CompareMode compareMode, CullMode cullMode):
    blendMode(blendMode),
    compareMode(compareMode),
    cullMode(cullMode)
    {
    }

    bool operator == (const RasterState& rhs) const
    {
        return blendMode == rhs.blendMode && compareMode == rhs.compareMode && cullMode == rhs.cullMode;
    }

    bool operator != (const RasterState& rhs) const
    {
        return blendMode != rhs.blendMode || compareMode != rhs.compareMode || cullMode != cullMode;
    }
};

struct ViewPort
{
    int x; 
    int y; 
    int width; 
    int height;

    bool operator == (const ViewPort& rhs) const
    {
        return x == rhs.x && y == rhs.y && width == rhs.width && height == rhs.height;
    }

    bool operator != (const ViewPort& rhs) const
    {
        return x != rhs.x || y != rhs.y || width != rhs.width || height != rhs.height;
    }

    void set(int x, int y, int width, int height)
    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }
};

static const unsigned int CLEAR_COLOR = 1;
static const unsigned int CLEAR_DEPTH = 4;
static const unsigned int CLEAR_STENCIL = 8;

}

#endif