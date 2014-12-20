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

#ifndef RenderConfig_H
#define RenderConfig_H

#include "Util/Vector.h"

namespace Huurre3D
{
    
struct DeferredStageDescription
{
    std::string vertexShader;
    std::string fragmentShader;
};

struct ShadowStageDescription
{
    struct ShadowDepthPass
    {
        std::string vertexShader;
        std::string fragmentShader;
        int ShadowMapSize;
    };

    struct ShadowOcclusionPass
    {
        std::string vertexShader;
        std::string fragmentShader;
        int maxNumShadowLights;
    };

    ShadowDepthPass shadowDepthPass;
    ShadowOcclusionPass shadowOcclusionPass;
};

struct LightingStageDescription
{
    struct SSAOPass
    {
        std::string vertexShader;
        std::string SSAOFragmentShader;
        std::string SSAOBlurFragmentShader;
        float ssaoRadius;
        float ssaoBias;
        float ssaoIntensity;
        int numSAOSamples;
        int numSAOSpiralTurns;
    };

    struct LightingPass
    {
        std::string vertexShader;
        std::string fragmentShader;
        int lightTileWidth;
        int lightTileHeight;
        bool hdr;
    };

    bool hasSSAOPass = false;
    SSAOPass ssaoPass;
    LightingPass lightingPass;
};

struct PostProcessStageDescription
{
    struct EnvironmentPass
    {
        std::string vertexShader;
        std::string fragmentShader;
    };

    struct AntiAliasingPass
    {
        std::string vertexShader;
        std::string fragmentShader;
        int fxaaQuality;
    };

    bool hasEnvironmentPass = false;
    bool hasAntiAliasingPass = false;
    EnvironmentPass environmentPass;
    AntiAliasingPass antiAliasingPass;
};

struct RendererDescription
{
    bool hasShadowStage = false;
    bool hasPostProcessStage = false;
    DeferredStageDescription deferredStageDescription;
    ShadowStageDescription shadowStageDescription;
    LightingStageDescription lightingStageDescription;
    PostProcessStageDescription postProcessStageDescription;
};

}

#endif