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

#include "AnimationDemo.h"
#include "Scene/Skybox.h"
#include "Scene/Camera.h"

void AnimationDemo::init()
{
    scene = engine->createScene();

    createScene();
    createNinjas();

    Mesh* ninja = scene->createSceneItem<Mesh>();
    engine->getSceneImporter().importMesh(Engine::getAssetPath() + "Models/Ninja/Ninja.dae", ninja);
    mainNinja = NinjaEntity(Vector3(45.0f, 0.0f, 0.0f), ninjaScale, ninja);

    ninjaMovements->play();
    mainNinja.attachCamera(camera);
}

void AnimationDemo::update(float timeSinceLastUpdate)
{
    float moveDistance = timeSinceLastUpdate * 10.0f;
    float moveRotation = timeSinceLastUpdate * 80.0f;
    auto input = engine->getInput();

    if(input.isKeyDown(KEY_LEFT))
        mainNinja.rotate(moveRotation, Direction::Left);

    if(input.isKeyDown(KEY_RIGHT))
        mainNinja.rotate(moveRotation, Direction::Right);

    if(input.isKeyDown(KEY_UP))
        camera->rotate(Quaternion(moveRotation, Vector3::UNIT_X), FrameOfReference::Local);

    if(input.isKeyDown(KEY_DOWN))
        camera->rotate(Quaternion(-moveRotation, Vector3::UNIT_X), FrameOfReference::Local);

    if(input.isKeyDown(KEY_W))
    {
        mainNinja.setAction(Action::Walk);
        mainNinja.move(moveDistance, Direction::Forward);
    }
    else if(input.isKeyDown(KEY_S))
    {
        mainNinja.setAction(Action::Walk);
        mainNinja.move(moveDistance, Direction::Backward);
    }
    else if(input.isKeyDown(KEY_Q))
        mainNinja.setAction(Action::BackFlip);
    else if(input.isKeyDown(KEY_E))
        mainNinja.setAction(Action::Kick);
    else if(input.isKeyDown(KEY_D))
        mainNinja.setAction(Action::SwordSwipe);
    else
        mainNinja.setAction(Action::Idle);

}

void AnimationDemo::createScene()
{
    FixedArray<std::string, 6> skyBoxTextureFileNames = {Engine::getAssetPath() + "Textures/Skybox2/red_ft.tga", Engine::getAssetPath() + "Textures/Skybox2/red_bk.tga",
                                                         Engine::getAssetPath() + "Textures/Skybox2/red_up.tga", Engine::getAssetPath() + "Textures/Skybox2/red_dn.tga",
                                                         Engine::getAssetPath() + "Textures/Skybox2/red_rt.tga", Engine::getAssetPath() + "Textures/Skybox2/red_lf.tga"};

    scene->setGlobalAmbientLight(Vector3(0.5f, 0.25f, 0.25f));
    SkyBox* skyBox = (SkyBox*)scene->createSceneItem("SkyBox");
    skyBox->setTextureFiles(skyBoxTextureFileNames);

    Vector<Mesh*> cityTiles;
    scene->createSceneItems<Mesh>(cityTiles, 9);
    engine->getSceneImporter().importMultipleMeshes(Engine::getAssetPath() + "Models/TownCityTile/Street_environment_V01.obj", cityTiles);

    float startX = -150.0f;
    float startZ = -150.0f;
    for(unsigned int i = 0; i < 3; ++i)
    {
        for(unsigned int j = 0; j < 3; ++j)
            cityTiles[j + i * 3]->translate(Vector3(startX + 96.0f * float(i), 0.0f, startZ + 94.0f * float(j)), FrameOfReference::World);
    }

    float width = static_cast<float>(engine->getRenderer().getScreenViewPort().width);
    float height = static_cast<float>(engine->getRenderer().getScreenViewPort().height);
    scene->getMainCamera()->setAspectRatio(width / height);
    camera = scene->getMainCamera();
    camera->setFarClipDistance(300.0f);
    FixedArray<float, 4> splits = {8.0f, 20.0f, 60.0f, 300.0f};

    Light* lightDirectional = (Light*)scene->createSceneItem("Light");
    lightDirectional->setLightType(LightType::Directional);
    lightDirectional->setDirection(Vector3(0.5f, -1.0f, 0.0f));
    lightDirectional->setColor(Vector3(1.0f, 0.3f, 0.3f));
    lightDirectional->setCastShadow(true);
    lightDirectional->setShadowMinDistanceOffset(50.0f);
    lightDirectional->setCascadeSplits(splits);
    lightDirectional->setShadowBias(0.0007f);
}

void AnimationDemo::createNinjas()
{
    Vector<Mesh*> ninjaMeshes;
    scene->createSceneItems<Mesh>(ninjaMeshes, numNinjas);
    engine->getSceneImporter().importMultipleMeshes(Engine::getAssetPath() + "Models/Ninja/Ninja.dae", ninjaMeshes);
    Vector<Track> ninjaMovementAnimationTracks;

    for(unsigned int i = 0; i < numNinjas; ++i)
    {
        //Create animation that moves the ninja meshes between start and end point. Each ninja has an own track.
        Track movement(*static_cast<SpatialSceneItem*>(ninjaMeshes[i]));
        //Get a random point on X-axis between the start (45.0f) and end (-155.0f) points. 
        float r = randomFloat(-155.0f, 45.0f);
        Vector3 startPosition = Vector3(r, 0.0f, -145.0f + i * 2.0f);
        ninjas.pushBack(NinjaEntity(startPosition, ninjaScale, ninjaMeshes[i]));
        Vector<KeyFrame> keyFrames(5);

        //First keyframe is the random position. Time starts at zero.
        keyFrames[0].position = startPosition;
        keyFrames[0].rotation = ninjaMeshes[i]->getRotation(FrameOfReference::Local);
        keyFrames[0].scale = ninjaScale;

        //Next keyframe is the end point. 
        //Value setted for time is how long it takes to move from the first keyframe's position to this keyframe's position,
        //when the time from start position to end position is defined in ninjaMovementStartToEndPosTime.
        keyFrames[1].time = (1 - abs(r - 45.0f) / 200.0f) * ninjaMovementStartToEndPosTime;
        keyFrames[1].position = Vector3(45.0f, 0.0f, -145.0f + i * 2.0f) - Vector3::UNIT_X * 200.0f;
        keyFrames[1].rotation = ninjaMeshes[i]->getRotation(FrameOfReference::Local);
        keyFrames[1].scale = ninjaScale;

        //Next keyframe rotates the mesh 180 degrees.
        keyFrames[2].time = (1 - abs(r - 45.0f) / 200.0f) * ninjaMovementStartToEndPosTime; + 0.2f;
        keyFrames[2].position = Vector3(45.0f, 0.0f, -145.0f + i * 2.0f) - Vector3::UNIT_X * 200.0f;
        keyFrames[2].rotation = ninjaMeshes[i]->getRotation(FrameOfReference::Local) * Quaternion(180, Vector3::UNIT_Z);
        keyFrames[2].scale = ninjaScale;

        //Next Keyframe is the starting point. 
        //Value setted for time is calculated from the time (keyFrames[1].time) when the end point was reached.
        keyFrames[3].time = ninjaMovementStartToEndPosTime + keyFrames[1].time;
        keyFrames[3].position = Vector3(45.0f, 0.0f, -145.0f + i * 2.0f);
        keyFrames[3].rotation = keyFrames[2].rotation;
        keyFrames[3].scale = ninjaScale;

        //Next Keyframe sets the rotation from first keyFrame, which rotates the mesh 180 degrees back to it's original rotation.
        keyFrames[4].time = ninjaMovementStartToEndPosTime + keyFrames[1].time + 0.2f;
        keyFrames[4].position = Vector3(45.0f, 0.0f, -145.0f + i * 2.0f);
        keyFrames[4].rotation = keyFrames[0].rotation;
        keyFrames[4].scale = ninjaScale;

        movement.keyFrames.pushBack(keyFrames);

        ninjaMovementAnimationTracks.pushBack(movement);
    }

    ninjaMovements = engine->getAnimation().createAnimationClip("NinjaMovement", ninjaMovementStartToEndPosTime * 2, true, ninjaMovementAnimationTracks);
}