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

#include "Graphics/ShaderParameters.h"

namespace Huurre3D
{

const std::string sp_worldTransform = "u_worldTransform";
const std::string sp_cameraParameters = "u_cameraParameters";
const std::string sp_lightParameters = "u_lightParameters";
const std::string sp_lightGridParameters = "u_lightGridParameters";
const std::string sp_materialProperties = "u_materialProperties";
const std::string sp_materialParameterIndex = "u_materialParameterIndex";
const std::string sp_lightViewProjectionMatrix = "u_lightViewProjectionMatrix";
const std::string sp_shadowOcclusionParameters = "u_shadowOcclusionParameters";
const std::string sp_shadowOcclusionParameterIndex = "u_shadowOcclusionParameterIndex";
const std::string sp_PostProcessParameters = "u_postProcessParameters";

}