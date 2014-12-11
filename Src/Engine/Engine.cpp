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

#include "Engine/Engine.h"
#include "Engine/App.h"
#include "Input/Input.h"
#include "Renderer/Renderer.h"
#include "Scene/Scene.h"
#include "Scene/SceneImporter.h"
#include "Scene/Camera.h"
#include "ThirdParty/cJSON/cJSON.h"
#include <fstream>

namespace Huurre3D
{

std::string Engine::assetPath;

Engine::~Engine()
{
    for(unsigned int i = 0; i < scenes.size(); ++i)
        delete scenes[i];

    delete sceneImporter;
    delete input;
    delete renderer;
}

void Engine::run()
{
    int windowWidth;
    int windowHeight;

    while(!input->isKeyPressed(KEY_ESCAPE) && !input->windowClosed())
    {
        if(input->windowResized(windowWidth, windowHeight))
        {
            renderer->resizeRenderWindow(windowWidth, windowHeight);

            for(unsigned int i = 0; i < scenes.size(); ++i)
                scenes[i]->getMainCamera()->setAspectRatio((float)windowWidth / (float)windowHeight);
        }

        update();
    }

    deInit();
}

void Engine::update()
{
    input->update();

    float currentFrame = timer.getElapsedTime();
    float timeSinceLastUpdate = max(0.0f, (currentFrame - lastFrame));
    lastFrame = currentFrame;

    for(unsigned int i = 0; i < apps.size(); ++i)
        apps[i]->update(timeSinceLastUpdate);

    for(unsigned int i = 0; i < scenes.size(); ++i)
        renderer->renderScene(scenes[i], scenes[i]->getMainCamera());
}

void Engine::setApp(App* app)
{
    app->setEngine(this);
    app->init();
    apps.pushBack(app);
}

void Engine::removeApp(App* app)
{
    app->setEngine(nullptr);
    app->deinit();
    apps.eraseUnordered(app);
}

Scene* Engine::createScene()
{
    Scene* scene = new Scene();
    scenes.pushBack(scene);
    return scene;
}

bool Engine::init(const std::string& engineConfigFile)
{
    std::ifstream ifs(engineConfigFile);

    if(!ifs)
    {
        std::cout << "Failed to open file " << engineConfigFile << std::endl;
        return false;
    }

    std::string fileContent((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    cJSON* engineConfigJson = cJSON_Parse(fileContent.c_str());
    if(!engineConfigJson)
    {
        std::cout << "Failed to parse config file. Error before: " << cJSON_GetErrorPtr() << std::endl;
        return false;
    }

    assetPath = cJSON_GetObjectItem(engineConfigJson, "assetPath")->valuestring;

    cJSON* rendererDescriptionValue = cJSON_GetObjectItem(engineConfigJson, "renderer");

    if(!rendererDescriptionValue)
    {
        std::cout << "Failed: Config file doesn't have renderer description." << std::endl;
        cJSON_Delete(engineConfigJson);
        return false;
    }

    RendererDescription rendererDescription;
    cJSON* deferredStageValue = cJSON_GetObjectItem(rendererDescriptionValue, "deferredStage");
    cJSON* lightingStageValue = cJSON_GetObjectItem(rendererDescriptionValue, "lightingStage");

    if(!deferredStageValue || !lightingStageValue)
    {
        std::cout << "Failed: Config file should include both deferrerd and lighting stages." << std::endl;
        cJSON_Delete(engineConfigJson);
        return false;
    }
    else
    {
        cJSON* SSAOPassValue = cJSON_GetObjectItem(lightingStageValue, "SSAOPass");
        cJSON* lightingPassValue = cJSON_GetObjectItem(lightingStageValue, "lightingPass");
        if(!lightingPassValue)
        {
            std::cout << "Failed: Lighting stage should include lighting pass." << std::endl;
            cJSON_Delete(engineConfigJson);
            return false;
        }

        rendererDescription.deferredStageDescription.vertexShader = cJSON_GetObjectItem(deferredStageValue, "vertexShader")->valuestring;
        rendererDescription.deferredStageDescription.fragmentShader = cJSON_GetObjectItem(deferredStageValue, "fragmentShader")->valuestring;

        rendererDescription.lightingStageDescription.lightingPass.vertexShader = cJSON_GetObjectItem(lightingPassValue, "vertexShader")->valuestring;
        rendererDescription.lightingStageDescription.lightingPass.fragmentShader = cJSON_GetObjectItem(lightingPassValue, "fragmentShader")->valuestring;
        rendererDescription.lightingStageDescription.lightingPass.lightTileWidth = cJSON_GetObjectItem(lightingPassValue, "lightTileWidth")->valueint;
        rendererDescription.lightingStageDescription.lightingPass.lightTileHeight = cJSON_GetObjectItem(lightingPassValue, "lightTileHeight")->valueint;
        rendererDescription.lightingStageDescription.lightingPass.hdr = cJSON_GetObjectItem(lightingPassValue, "HDR")->type;

        if(SSAOPassValue)
        {
            rendererDescription.lightingStageDescription.hasSSAOPass = true;
            rendererDescription.lightingStageDescription.ssaoPass.vertexShader = cJSON_GetObjectItem(SSAOPassValue, "vertexShader")->valuestring;
            rendererDescription.lightingStageDescription.ssaoPass.SSAOFragmentShader = cJSON_GetObjectItem(SSAOPassValue, "SSAOfragmentShader")->valuestring;
            rendererDescription.lightingStageDescription.ssaoPass.SSAOBlurFragmentShader = cJSON_GetObjectItem(SSAOPassValue, "SSAOBlurfragmentShader")->valuestring;
            rendererDescription.lightingStageDescription.ssaoPass.ssaoRadius = static_cast<float>(cJSON_GetObjectItem(SSAOPassValue, "SSAORadius")->valuedouble);
            rendererDescription.lightingStageDescription.ssaoPass.ssaoBias = static_cast<float>(cJSON_GetObjectItem(SSAOPassValue, "SSAOBias")->valuedouble);
            rendererDescription.lightingStageDescription.ssaoPass.ssaoIntensity = static_cast<float>(cJSON_GetObjectItem(SSAOPassValue, "SSAOIntensity")->valuedouble);
            rendererDescription.lightingStageDescription.ssaoPass.numSAOSamples = cJSON_GetObjectItem(SSAOPassValue, "numSAOSamples")->valueint;
            rendererDescription.lightingStageDescription.ssaoPass.numSAOSpiralTurns = cJSON_GetObjectItem(SSAOPassValue, "numSAOSpiralTurns")->valueint;
        }
        
    }

    //Parse optional stages
    cJSON* shadowStageValue = cJSON_GetObjectItem(rendererDescriptionValue, "shadowStage");
    cJSON* postProcessStageValue = cJSON_GetObjectItem(rendererDescriptionValue, "postProcessStage");

    cJSON* shadowDepthPassValue = cJSON_GetObjectItem(shadowStageValue, "shadowDepthPass");
    cJSON* shadowOcclusionPassValue = cJSON_GetObjectItem(shadowStageValue, "shadowOcclusionPass");

    if(shadowDepthPassValue && shadowOcclusionPassValue)
    {
        rendererDescription.shadowStageDescription.shadowDepthPass.vertexShader = cJSON_GetObjectItem(shadowDepthPassValue, "vertexShader")->valuestring;
        rendererDescription.shadowStageDescription.shadowDepthPass.fragmentShader = cJSON_GetObjectItem(shadowDepthPassValue, "fragmentShader")->valuestring;
        rendererDescription.shadowStageDescription.shadowDepthPass.ShadowMapSize = cJSON_GetObjectItem(shadowDepthPassValue, "shadowMapSize")->valueint;

        rendererDescription.shadowStageDescription.shadowOcclusionPass.vertexShader = cJSON_GetObjectItem(shadowOcclusionPassValue, "vertexShader")->valuestring;
        rendererDescription.shadowStageDescription.shadowOcclusionPass.fragmentShader = cJSON_GetObjectItem(shadowOcclusionPassValue, "fragmentShader")->valuestring;
        rendererDescription.shadowStageDescription.shadowOcclusionPass.maxNumShadowLights = cJSON_GetObjectItem(shadowOcclusionPassValue, "maxShadowLights")->valueint;
    }
    else
        std::cout << "Ignoring shadow stage, it should include both depth and occlusion passes." << std::endl;

    cJSON* environmentPassValue = cJSON_GetObjectItem(postProcessStageValue, "environmentPass");
    cJSON* antialiasingPassValue = cJSON_GetObjectItem(postProcessStageValue, "antiAliasingPass");

    if(environmentPassValue)
    {
        rendererDescription.postProcessStageDescription.environmentPass.vertexShader = cJSON_GetObjectItem(environmentPassValue, "vertexShader")->valuestring;
        rendererDescription.postProcessStageDescription.environmentPass.fragmentShader = cJSON_GetObjectItem(environmentPassValue, "fragmentShader")->valuestring;
    }
    if(antialiasingPassValue)
    {
        rendererDescription.postProcessStageDescription.antiAliasingPass.vertexShader = cJSON_GetObjectItem(antialiasingPassValue, "vertexShader")->valuestring;
        rendererDescription.postProcessStageDescription.antiAliasingPass.fragmentShader = cJSON_GetObjectItem(antialiasingPassValue, "fragmentShader")->valuestring;
        rendererDescription.postProcessStageDescription.antiAliasingPass.fxaaQuality = cJSON_GetObjectItem(antialiasingPassValue, "FXAAQuality")->valueint;
    }

    cJSON_Delete(engineConfigJson);
    renderer = new Renderer(rendererDescription);
    sceneImporter = new SceneImporter(renderer);
    input = new Input(renderer->getGraphicWindow());
    timer.start();

    return true;
}

void Engine::deInit()
{
    for(unsigned int i = 0; i < apps.size(); ++i)
        apps[i]->deinit();
}

}