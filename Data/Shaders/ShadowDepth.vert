#version 330
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

#include "Attributes.vert"
#include "Uniforms.vert"

void main()
{
    vec4 position = vec4(i_position, 1.0f);

#ifdef SKINNED
    ivec4 jointIndex = ivec4(i_jointIndices);
    mat4 skinnedTransform = u_skinMatrices[jointIndex.x] * i_jointWeights.x;
    skinnedTransform += u_skinMatrices[jointIndex.y] * i_jointWeights.y;
    skinnedTransform += u_skinMatrices[jointIndex.z] * i_jointWeights.z;
    skinnedTransform += u_skinMatrices[jointIndex.w] * i_jointWeights.w;
    position = skinnedTransform * position;
#endif

    gl_Position = u_lightViewProjectionMatrix * u_worldTransform * position;
}