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

struct BlendState
{
    bool enabled;
    BlendFunction blendFunction;
    BlendState(bool enabled, BlendFunction function):
    enabled(enabled),
    blendFunction(function)
    {}

    bool operator == (const BlendState& rhs) const
    {
        return enabled == rhs.enabled && blendFunction == rhs.blendFunction;
    }

    bool operator != (const BlendState& rhs) const
    {
        return enabled != rhs.enabled || blendFunction != rhs.blendFunction;
    }
};

struct CompareState
{
    bool enabled = false;
    CompareFunction compareFunction = CompareFunction::Less;
    CompareState(bool enabled, CompareFunction function) :
    enabled(enabled),
    compareFunction(function)
    {}

    bool operator == (const CompareState& rhs) const
    {
        return enabled == rhs.enabled && compareFunction == rhs.compareFunction;
    }

    bool operator != (const CompareState& rhs) const
    {
        return enabled != rhs.enabled || compareFunction != rhs.compareFunction;
    }
};

struct CullState
{
    bool enabled = false;
    CullFace cullFace = CullFace::Back;
    CullState(bool enabled, CullFace cullFace) :
    enabled(enabled),
    cullFace(cullFace)
    {}

    bool operator == (const CullState& rhs) const
    {
        return (enabled == rhs.enabled) && (cullFace == rhs.cullFace);
    }

    bool operator != (const CullState& rhs) const
    {
        return (enabled != rhs.enabled) || (cullFace != rhs.cullFace);
    }
};

struct RasterState
{
    unsigned int stateId = 0;
    BlendState blendState = BlendState(false, BlendFunction::Replace);
    CompareState compareState = CompareState(false, CompareFunction::Never);
    CullState cullState = CullState(false, CullFace::Back);

    RasterState() = default;
    RasterState(BlendState blendState, CompareState compareState, CullState cullState) :
    blendState(blendState),
    compareState(compareState),
    cullState(cullState)
    {
        if(blendState.enabled)
        {
            stateId |= 1 << 1;
            stateId |= 1 << (2 + static_cast<unsigned int>(blendState.blendFunction));
        }
        if(compareState.enabled)
        {
            stateId |= 1 << 7;
            stateId |= 1 << (8 + static_cast<unsigned int>(compareState.compareFunction));
        }
        if(cullState.enabled)
        {
            stateId |= 1 << 16;
            stateId |= 1 << (17 + static_cast<unsigned int>(cullState.cullFace));
        }
    }

    bool operator == (const RasterState& rhs) const
    {
        return stateId == rhs.stateId;
    }

    bool operator != (const RasterState& rhs) const
    {
        return stateId != rhs.stateId;
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