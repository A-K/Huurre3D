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

#include "SSAODemo.h"
#include "Scene/Skybox.h"

void SSAODemo::init()
{
    scene = engine->createScene();
    sceneImporter = engine->getSceneImporter();
    input = engine->getInput();

    FixedArray<std::string, 6> skyBoxTextureFileNames = { Engine::getAssetPath() + "Textures/Skybox/miramar_ft.tga", Engine::getAssetPath() + "Textures/Skybox/miramar_bk.tga",
                                                          Engine::getAssetPath() + "Textures/Skybox/miramar_up.tga", Engine::getAssetPath() + "Textures/Skybox/miramar_dn.tga",
                                                          Engine::getAssetPath() + "Textures/Skybox/miramar_rt.tga", Engine::getAssetPath() + "Textures/Skybox/miramar_lf.tga" };

    scene->setGlobalAmbientLight(Vector3(1.0f, 1.0f, 1.0f));
    SkyBox* skyBox = (SkyBox*)scene->createSceneItem("SkyBox");
    skyBox->setTextureFiles(skyBoxTextureFileNames);

    Mesh* sponza = scene->createSceneItem<Mesh>();
    sceneImporter->importMesh(Engine::getAssetPath() + "Models/Sponza/sponza.obj", sponza);
    sponza->scale(0.05f);

    float width = static_cast<float>(engine->getRenderer()->getScreenViewPort().width);
    float height = static_cast<float>(engine->getRenderer()->getScreenViewPort().height);
    scene->getMainCamera()->setAspectRatio((float)width / (float)height);
    camera = scene->getMainCamera();
    camera->translate(Vector3(20.0f, 5.0f, 0.0f), FrameOfReference::World);
    camera->setRotation(Quaternion(-90.0f, Vector3::UNIT_Y));
}

void SSAODemo::update(float timeSinceLastUpdate)
{
    if(input->isKeyPressed(KEY_R))
    {
        ssaoOn = !ssaoOn;
        updateShaderParameters();
    }
    else if(input->isKeyPressed(KEY_T))
    {
        showSSAOTex = !showSSAOTex;
        updateShaderParameters();
    }

    float moveDistance = timeSinceLastUpdate * 20.0f;
    float moveRotation = timeSinceLastUpdate * 60.0f;

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

void SSAODemo::updateShaderParameters()
{
    ShaderParameterBlock* block = engine->getRenderer()->getGraphicSystem()->getShaderParameterBlockByName("u_showSSAOParameters");
    block->clearParameters();

    Vector<int> data(4);
    data.fill(0);

    if(showSSAOTex)
    {
        data[1] = 1;
        ssaoOn = true;
    }
    if(ssaoOn)
        data[0] = 1;

    block->setParameterData(std::move(data.getMemoryBuffer()));
}