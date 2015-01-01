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

#include "Engine/App.h"
#include "Engine/Engine.h"
#include "Scene/Mesh.h"
#include "Input/Input.h"
#include "Scene/SceneImporter.h"

using namespace Huurre3D;

class LightDemoApp : public App
{
public:
    LightDemoApp() = default;
    ~LightDemoApp() = default;

    void init() override;
    void deinit() override {};
    void update(float timeSinceLastUpdate) override;

private:
    void moveCamera(float timeSinceLastUpdate);
    void moveLights(float timeSinceLastUpdate);
    void createDirectionalLight();
    void createPointLights();
    void createSpotLights();
    void createPointLight(Mesh* sphere, float lightScale, float radius, float fallOff, const Vector3& color, const Vector3& position, bool shadow);
    void createSpotLight(float radius, float fallOff, float innerConeAngle, float outerConeAngle, const Vector3& color, const Vector3& position, Vector3& direction, bool shadow);

    Camera* camera;
    Scene* scene;
    Input* input;
    SceneImporter* sceneImporter;
    Vector<Light*> pointLights;
    Vector<Light*> shadowCastingPointLights;
    float lightsMinHeight = 10.0f;
    float lightsMaxHeight = 300.0f;
    FixedArray<Vector3, 4> movingPointLightCorners;
    bool stopLights = false;
};
