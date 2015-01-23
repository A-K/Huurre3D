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

#ifndef Engine_H
#define Engine_H

#include "Util/Vector.h"
#include "Util/Timer.h"

namespace Huurre3D
{

class Animation;
class Scene;
class Renderer;
class SceneImporter;
class Input;
class App;

const std::string defaultConfigFile = "../DefaultHuurre3DConfig.json";

class Engine
{
public:
    Engine() = default;
    ~Engine();
	
    bool init(const std::string& engineConfigFile = defaultConfigFile);
    void run();
    void setApp(App* app);
    void removeApp(App* app);
    Scene* createScene();
    Scene* getScene(unsigned int id = 0) const {return scenes[id];}
    SceneImporter* getSceneImporter() const {return sceneImporter;}
    Renderer* getRenderer() const {return renderer;}
    Animation* getAnimation() const {return animation;}
    Input* getInput() const {return input;}
    static std::string& getAssetPath() {return assetPath;}

private:
    void deInit();
    void update();
    Animation* animation = nullptr;
    Renderer* renderer = nullptr;
    Vector<Scene*> scenes;
    Vector<App*> apps;
    SceneImporter* sceneImporter = nullptr;
    Input* input = nullptr;
    Timer timer;
    float lastFrame = 0.0f;
    static std::string assetPath;
};

}
#endif