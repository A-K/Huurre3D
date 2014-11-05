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

#ifndef SceneItemFactory_H
#define SceneItemFactory_H

#include "Util/Vector.h"
#include <string>

namespace Huurre3D
{

class SceneItem;

//Base class for SceneItem creator.
class SceneItemCreator
{
public:
    SceneItemCreator(const std::string& sceneItemType);
    virtual ~SceneItemCreator() = default;
    virtual SceneItem* createSceneItem() = 0;

    std::string sceneItemType;
};

//Template implementation of the SceneItem creator.
template <class T> class SceneItemCreatorImpl : public SceneItemCreator
{
public:
    SceneItemCreatorImpl<T>(const std::string& sceneItemType): 
    SceneItemCreator(sceneItemType)
    {}

    ~SceneItemCreatorImpl<T>() = default;
    SceneItem* createSceneItem() override {return new T;}
};

//The actual factory class.
class SceneItemFactory
{
public:
    static SceneItem* createSceneItem(const std::string& sceneItemType);
    static void registerCreator(SceneItemCreator* creator);
private:
    static Vector<SceneItemCreator*>& getCreatorContainer();
};

//Macro for definining the creator for different types of SceneItems.
#define SCENEITEM_TYPE(SceneItemType) \
    public: \
    static std::string getSceneItemTypeStatic() {static const std::string type(#SceneItemType); return type;} \
    private: \
    static const SceneItemCreatorImpl<SceneItemType> sceneItemCreator;

//Macro which will instantiate the creator.
#define SCENEITEM_TYPE_IMPL(SceneItemType) \
    const SceneItemCreatorImpl<SceneItemType> SceneItemType::sceneItemCreator(#SceneItemType);


}

#endif