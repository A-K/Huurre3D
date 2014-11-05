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

#ifndef Scene_H
#define Scene_H

#include "Renderer/RenderItem.h"
#include "Math/Frustum.h"
#include "Util/Vector.h"

namespace Huurre3D
{

class Camera;
class SceneItem;
class Light;
class SpatialSceneItem;

class Scene
{
public:
    Scene();
    ~Scene();
	
    void update();
    SceneItem* createSceneItem(const std::string& sceneItemType);
    void createSceneItems(const Vector<const std::string>& sceneItemTypes, Vector<SceneItem*>& itemsOut);
    void createSceneItems(const std::string& sceneItemType, Vector<SceneItem*>& itemsOut, unsigned int numItems);
    SceneItem* getSceneItem(unsigned int id) const;
    SceneItem* getSceneItem(const std::string& sceneItemType) const;
    void getSceneItemsByType(const std::string& sceneItemType, Vector<SceneItem*>& itemsOut) const;
    void getAllRenderItems(Vector<RenderItem>& renderItemsOut);
    void setGlobalAmbientLight(const Vector3& ambientLight);
    void setSceneItemForUpdate(SceneItem* sceneItem) {dirtySceneItems.pushBack(sceneItem);}
    Camera* getMainCamera() const {return mainCamera;}
    const Vector3& getGlobalAmbientLight() const {return globalAmbientLight;}
    template<class T> T* createSceneItem() { return static_cast<T*>(createSceneItem(T::getSceneItemTypeStatic())); }
    template<class T> void createSceneItems(Vector<T*>& itemsOut, unsigned int numItems)
    {
        for(unsigned int i = 0; i < numItems; ++i)
            itemsOut.pushBack(static_cast<T*>(createSceneItem(T::getSceneItemTypeStatic())));
    }
    template<class T> void getSceneItemsByType(Vector<T*>& itemsOut) const
    {
        Vector<SceneItem*> items;
        getSceneItemsByType(T::getSceneItemTypeStatic(), items);

        for(unsigned int i = 0; i < items.size(); ++i)
            itemsOut.pushBack(static_cast<T*>(items[i]));
    }

private:
    unsigned int getUniqueId() {return uniqueId++;}
    unsigned int uniqueId = 0;
    Vector<SceneItem*> sceneItems;
    Vector<SceneItem*> dirtySceneItems;
    Camera* mainCamera = nullptr;
    Vector3 globalAmbientLight = Vector3::ONE;
};

}

#endif