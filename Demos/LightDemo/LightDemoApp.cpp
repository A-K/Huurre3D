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

#include "LightDemoApp.h"
#include "Scene/Skybox.h"

void LightDemoApp::init()
{
    scene = engine->createScene();
    sceneImporter = engine->getSceneImporter();
    input = engine->getInput();

    FixedArray<std::string, 6> skyBoxTextureFileNames;
    skyBoxTextureFileNames[0] = Engine::getAssetPath() + "Textures/Skybox/miramar_ft.tga";
    skyBoxTextureFileNames[1] = Engine::getAssetPath() + "Textures/Skybox/miramar_bk.tga";
    skyBoxTextureFileNames[2] = Engine::getAssetPath() + "Textures/Skybox/miramar_up.tga";
    skyBoxTextureFileNames[3] = Engine::getAssetPath() + "Textures/Skybox/miramar_dn.tga";
    skyBoxTextureFileNames[4] = Engine::getAssetPath() + "Textures/Skybox/miramar_rt.tga";
    skyBoxTextureFileNames[5] = Engine::getAssetPath() + "Textures/Skybox/miramar_lf.tga";

    scene->setGlobalAmbientLight(Vector3(0.05f, 0.05f, 0.05f));
    SkyBox* skyBox = (SkyBox*)scene->createSceneItem("SkyBox");
    skyBox->setTextureFiles(skyBoxTextureFileNames);

    Mesh* sponza = scene->createSceneItem<Mesh>();
    sceneImporter->importMesh(Engine::getAssetPath() + "Models/Sponza/sponza.obj", sponza);
    sponza->scale(0.25f);

    float width = static_cast<float>(engine->getRenderer()->getScreenViewPort().width);
    float height = static_cast<float>(engine->getRenderer()->getScreenViewPort().height);
    scene->getMainCamera()->setAspectRatio(width / height);
    camera = scene->getMainCamera();
    camera->translate(Vector3(-250.0f, 350.0f, -10.0f), FrameOfReference::World);
    camera->setRotation(Quaternion(-90.0f, Vector3::UNIT_Y));

    movingPointLightCorners[0] = Vector3(175.0f, 170.0f, 25.0f);
    movingPointLightCorners[1] = Vector3(-205.0f, 170.0f, 25.0f);
    movingPointLightCorners[2] = Vector3(-205.0f, 170.0f, -40.0f);
    movingPointLightCorners[3] = Vector3(175.0f, 170.0f, -40.0f);

    createDirectionalLight();
    createPointLights();
    createSpotLights();
}

void LightDemoApp::update(float timeSinceLastUpdate)
{

    moveCamera(timeSinceLastUpdate);

    if(!stopLights)
        moveLights(timeSinceLastUpdate);

    if(input->isMouseButtonPressed(MOUSE_BUTTON_LEFT))
        stopLights = true;
    else if(input->isMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        stopLights = false;
}

void LightDemoApp::moveCamera(float timeSinceLastUpdate)
{
    float moveDistance = timeSinceLastUpdate * 50.0f;
    float moveRotation = timeSinceLastUpdate * 50.0f;

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

void LightDemoApp::moveLights(float timeSinceLastUpdate)
{
    Vector3 lightPosition;
    Vector3 lightDirection;

    for(unsigned int i = 0; i < pointLights.size(); ++i)
    {
        lightPosition = pointLights[i]->getPosition(FrameOfReference::World);
        if(lightPosition.y > lightsMaxHeight)
            pointLights[i]->setDirection(-Vector3::UNIT_Y);
        else if(lightPosition.y < lightsMinHeight)
            pointLights[i]->setDirection(Vector3::UNIT_Y);

        lightDirection = pointLights[i]->getDirection();
        pointLights[i]->translate(lightDirection * Vector3(0.0f, 10.0f * timeSinceLastUpdate, 0.0f), FrameOfReference::World);
    }

    for(unsigned int i = 0; i < shadowCastingPointLights.size(); ++i)
    {
        lightPosition = shadowCastingPointLights[i]->getPosition(FrameOfReference::World);

        if(lightPosition.x < movingPointLightCorners[1].x && lightPosition.z >= movingPointLightCorners[1].z)
            shadowCastingPointLights[i]->setDirection(-Vector3::UNIT_Z);

        if(lightPosition.x <= movingPointLightCorners[2].x && lightPosition.z < movingPointLightCorners[2].z)
            shadowCastingPointLights[i]->setDirection(Vector3::UNIT_X);
        
        if(lightPosition.x > movingPointLightCorners[3].x && lightPosition.z <= movingPointLightCorners[2].z)
            shadowCastingPointLights[i]->setDirection(Vector3::UNIT_Z);

        if(lightPosition.x >= movingPointLightCorners[0].x && lightPosition.z > movingPointLightCorners[0].z)
            shadowCastingPointLights[i]->setDirection(-Vector3::UNIT_X);

        lightDirection = shadowCastingPointLights[0]->getDirection();
        shadowCastingPointLights[i]->translate(lightDirection * 16.0f * timeSinceLastUpdate, FrameOfReference::World);
    }
}

void LightDemoApp::createDirectionalLight()
{
    FixedArray<float, 4> splits;
    splits[0] = 50.0f;
    splits[1] = 150.0f;
    splits[2] = 500.0f;
    splits[3] = 1000.0f;
    Light* lightDirectional = (Light*)scene->createSceneItem("Light");
    lightDirectional->setLightType(LightType::Directional);
    lightDirectional->setDirection(Vector3(0.6f, -1.0f, 0.0f));
    lightDirectional->setColor(Vector3(1.0f, 1.0f, 1.0f));
    lightDirectional->setCastShadow(true);
    lightDirectional->setShadowMinDistanceOffset(400.0f);
    lightDirectional->setCascadeSplits(splits);
    lightDirectional->setShadowBias(0.005f);
}
void LightDemoApp::createSpotLights()
{
    createSpotLight(113.0f, 0.7f, 20.0f, 30.0f, Vector3(1.0f, 1.0f, 0.5f), Vector3(-350.0f, 100.0f, -10.0f), Vector3(0.0f, -1.0f, 0.0f), true);
    createSpotLight(113.0f, 0.7f, 20.0f, 30.0f, Vector3(1.0f, 1.0f, 0.5f), Vector3(315.0f, 100.0f, -10.0f), Vector3(0.0f, -1.0f, 0.0f), true);

    for(int i = 0; i < 5; ++i)
    {
        createSpotLight(110.0f, 0.7f, 20.0f, 30.0f, Vector3(1.0f, 1.0f, 0.7f), Vector3((-300.0f + float(i) * 145.0f), 100.0f, -145.0f), Vector3(0.0f, -1.0f, 0.0f), false);
        createSpotLight(110.0f, 0.7f, 20.0f, 30.0f, Vector3(1.0f, 1.0f, 0.7f), Vector3((-300.0f + float(i) * 145.0f), 100.0f, 130.0f), Vector3(0.0f, -1.0f, 0.0f), false);
    }
}

void LightDemoApp::createPointLights()
{
    Vector<Mesh*> lightSpheres;
    scene->createSceneItems<Mesh>(lightSpheres, 601);
    sceneImporter->importMultipleMeshes(Engine::getAssetPath() + "Models/Sphere/Sphere.obj", lightSpheres);

    for(int i = 0; i < 30; ++i)
    {
        float r = randomFloat(0.1f, 1.0f);
        float g = randomFloat(0.1f, 1.0f);
        float b = randomFloat(0.1f, 1.0f);

        for(int j = 0; j < 20; ++j)
        {
            float h = randomFloat(lightsMinHeight, lightsMaxHeight);
            createPointLight(lightSpheres[i + j * 30], 1.0f, 35.0f, 0.5f, Vector3(r, g, b), Vector3((-340.0f + float(i) * 22.0f), h, -150.0f + float(j) * 15.0f), false);
        }
    }

    createPointLight(lightSpheres[600], 4.0f, 100.0f, 0.15f, Vector3(1.0f, 0.9f, 0.0f), movingPointLightCorners[0], true);
    shadowCastingPointLights[0]->setDirection(-Vector3::UNIT_X);
}

void LightDemoApp::createPointLight(Mesh* sphere, float lightScale, float radius, float fallOff, const Vector3& color, const Vector3& position, bool shadow)
{
    Light *light = (Light*)scene->createSceneItem("Light");
    light->setRadius(radius);
    light->setFallOffExponent(fallOff);
    light->setColor(color);
    light->setPosition(position);
    light->setDirection(Vector3::UNIT_Y); //Direction is used for indicating the movement direction when moving the lights.
    light->setCastShadow(shadow);
    light->setShadowBias(0.00015f);

    sphere->scale(lightScale);
    MaterialDescription description;
    description.diffuseColor = color;
    description.specularColor = color;
    description.emissiveColor = color;
    Material* lightMaterial = engine->getRenderer()->createMaterial(description);
    sphere->setMaterial(lightMaterial);
    light->addChild((SpatialSceneItem*)sphere);

    shadow ? shadowCastingPointLights.pushBack(light) : pointLights.pushBack(light);
}

void LightDemoApp::createSpotLight(float radius, float fallOff, float innerConeAngle, float outerConeAngle, const Vector3& color, const Vector3& position, Vector3& direction, bool shadow)
{
    Light *light = (Light*)scene->createSceneItem("Light");
    light->setLightType(LightType::Spot);
    light->setRadius(radius);
    light->setFallOffExponent(fallOff);
    light->setInnerConeAngle(innerConeAngle);
    light->setOuterConeAngle(outerConeAngle);
    light->setColor(color);
    light->setPosition(position);
    light->setDirection(direction);
    light->setCastShadow(shadow);
    light->setShadowBias(0.0004f);
}