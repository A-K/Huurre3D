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

//Material Textures.
uniform sampler2D u_diffuseTexture;
uniform sampler2D u_specularTexture;
uniform sampler2D u_normalTexture;
uniform sampler2D u_alphaTexture;

//G-buffer
uniform sampler2D u_diffuseBuffer;
uniform sampler2D u_specularBuffer;
uniform sampler2D u_normalBuffer;

//
uniform sampler2DArray u_shadowDepth;

//Texture which includes light id's for each tile.
uniform isampler2D u_tileLightInfo;

//Texture which includes the shadow occlusion for 32 lights is accessed usign the light's shadow occlusion mask. 
uniform sampler2D u_shadowOcclusion;

uniform sampler2DArray u_ssao;

//Texture which includes the result from lighting calculations.
uniform sampler2D u_lightingTexture;

uniform samplerCube u_skyBox;