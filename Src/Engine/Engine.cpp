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

namespace Huurre3D
{

std::string Engine::shaderPath;
std::string Engine::assetPath;

Engine::Engine()
{
    init();
}

Engine::~Engine()
{
    for(unsigned int i = 0; i < scenes.size(); ++i)
        delete scenes[i];

    for(unsigned int i = 0; i < apps.size(); ++i)
        delete apps[i];

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

void Engine::init()
{
    //Set deafault pathes.
    setShaderPath("../../Data/Shaders/");
    setAssetPath("../Assets/");
    renderer = new Renderer();
    sceneImporter = new SceneImporter(renderer);
    input = new Input(renderer->getGraphicWindow());
    timer.start();
}

void Engine::deInit()
{
    for(unsigned int i = 0; i < apps.size(); ++i)
        apps[i]->deinit();
}

void Engine::setAssetPath(const std::string& path)
{
    assetPath = path;
}

void Engine::setShaderPath(const std::string& path)
{
    shaderPath = path;
}

}