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

#include "SimpleSceneDemo.h"

void SimpleSceneDemo::init()
{
    scene = engine->createScene();
    sceneImporter = engine->getSceneImporter();
    engine->getRenderer()->createRenderWindow(width, height, title);
    input = engine->getInput();

    FixedArray<std::string, 6> skyBoxTextureFileNames;
    skyBoxTextureFileNames[0] = Engine::getAssetPath() + "Textures/Skybox2/red_ft.tga";
    skyBoxTextureFileNames[1] = Engine::getAssetPath() + "Textures/Skybox2/red_bk.tga";
    skyBoxTextureFileNames[2] = Engine::getAssetPath() + "Textures/Skybox2/red_up.tga";
    skyBoxTextureFileNames[3] = Engine::getAssetPath() + "Textures/Skybox2/red_dn.tga";
    skyBoxTextureFileNames[4] = Engine::getAssetPath() + "Textures/Skybox2/red_rt.tga";
    skyBoxTextureFileNames[5] = Engine::getAssetPath() + "Textures/Skybox2/red_lf.tga";

    scene->setGlobalAmbientLight(Vector3(0.5f, 0.25f, 0.25f));
    SkyBox* skyBox = (SkyBox*)scene->createSceneItem("SkyBox");
    skyBox->setTextureFiles(skyBoxTextureFileNames);

    Vector<Mesh*> cityTiles;
    scene->createSceneItems<Mesh>(cityTiles, 9);
    sceneImporter->importMultipleMeshes(Engine::getAssetPath() + "Models/TownCityTile/Street_environment_V01.obj", cityTiles);

    float startX = -150.0f;
    float startZ = -150.0f;
    for(unsigned int i = 0; i < 3; ++i)
    {
        for(unsigned int j = 0; j < 3; ++j)
            cityTiles[j + i * 3]->translate(Vector3(startX + 96.0f * float(i), 0.0f, startZ + 94.0f * float(j)), FrameOfReference::World);
    }

    scene->getMainCamera()->setAspectRatio((float)width / (float)height);
    camera = scene->getMainCamera();
    camera->setPosition(Vector3(15.0f, 10.0f, 0.0f));

    FixedArray<float, 4> splits;
    splits[0] = 50.0f;
    splits[1] = 200.0f;
    splits[2] = 500.0f;
    splits[3] = 1000.0f;
    Light* lightDirectional = (Light*)scene->createSceneItem("Light");
    lightDirectional->setLightType(LightType::Directional);
    lightDirectional->setDirection(Vector3(0.5f, -1.0f, 0.0f));
    lightDirectional->setColor(Vector3(1.0f, 0.3f, 0.3f));
    lightDirectional->setCastShadow(true);
    lightDirectional->setShadowMinDistanceOffset(300.0f);
    lightDirectional->setCascadeSplits(splits);
    lightDirectional->setShadowBias(0.0007f);
}

void SimpleSceneDemo::update(float timeSinceLastUpdate)
{
    float moveDistance = timeSinceLastUpdate * 40.0f;
    float moveRotation = timeSinceLastUpdate * 60.0f;

    Vector3 camPos = camera->getPosition(FrameOfReference::Local);
    Vector3 camCeil = Vector3(80.0f, 50.0f, 80.0f);
    Vector3 camFloor = Vector3(-185.0f, 3.0f, -185.0f);
    camera->setPosition(clamp(camPos, camFloor, camCeil));

    if(input->isKeyDown(KEY_LEFT))
        camera->rotate(Quaternion(moveRotation, Vector3::UNIT_Y), FrameOfReference::World);
 
    if(input->isKeyDown(KEY_RIGHT))
        camera->rotate(Quaternion(-moveRotation, Vector3::UNIT_Y), FrameOfReference::World);

    if(input->isKeyDown(KEY_UP))
        camera->rotate(Quaternion(moveRotation, Vector3::UNIT_X), FrameOfReference::Local);

    if(input->isKeyDown(KEY_DOWN))
        camera->rotate(Quaternion(-moveRotation, Vector3::UNIT_X), FrameOfReference::Local);

    if(input->isKeyDown(KEY_W))
        camera->translate((Vector3(0.0f, 0.0f, -moveDistance)), FrameOfReference::Local);

    if(input->isKeyDown(KEY_S))
        camera->translate(Vector3(0.0f, 0.0f, moveDistance), FrameOfReference::Local);

    if(input->isKeyDown(KEY_A))
        camera->translate(Vector3(-moveDistance, 0.0f, 0.0f), FrameOfReference::Local);

    if(input->isKeyDown(KEY_D))
        camera->translate(Vector3(moveDistance, 0.0f, 0.0f), FrameOfReference::Local);
}