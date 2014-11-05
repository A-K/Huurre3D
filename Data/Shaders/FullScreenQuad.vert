#version 330 core
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

#include "Attributes.vert"
#include "Uniforms.vert"

out	vec2 f_texCoord0;
out vec3 f_frustumCorner;
out vec3 f_worldSpaceViewRay;
out vec3 f_cameraWorldPosition;
out vec3 f_frustumFarPlaneLeftBottomCorner;
out vec2 f_frustumFarPlaneSize;

void main()
{
    vec4 position = vec4(i_position, 1.0f);
    f_texCoord0 = position.xy * 0.5f + 0.5f;
    f_frustumCorner = u_frustumFarPlaneCorners[gl_VertexID].xyz;
    f_frustumFarPlaneSize = vec2(abs(u_frustumFarPlaneCorners[0].x - u_frustumFarPlaneCorners[2].x), abs(u_frustumFarPlaneCorners[2].y - u_frustumFarPlaneCorners[3].y));
    f_frustumFarPlaneLeftBottomCorner = u_frustumFarPlaneCorners[1].xyz;

#ifdef USE_WORLD_PARAMETERS
    mat3 invViewRot = transpose(mat3(u_viewMatrix));
    f_worldSpaceViewRay = invViewRot * u_frustumFarPlaneCorners[gl_VertexID].xyz;
    f_cameraWorldPosition = -invViewRot * u_viewMatrix[3].xyz;
#endif

    gl_Position = position;
}