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

#include "Scene/Scene.h"
#include "Scene/SceneItemFactory.h"
#include "Scene/Camera.h"
#include "Scene/Mesh.h"
#include <iostream>

namespace Huurre3D
{

Scene::Scene()
{
    mainCamera = static_cast<Camera*>(createSceneItem("Camera"));
}

Scene::~Scene()
{
    removeAllSceneItem();
}

void Scene::update()
{
    for(unsigned int i = 0; i < dirtySceneItems.size(); ++i)
        dirtySceneItems[i]->updateItem();

    dirtySceneItems.clear();
}

SceneItem* Scene::createSceneItem(const std::string& sceneItemType)
{
    unsigned int id = getUniqueId();
    SceneItem* sceneItem = SceneItemFactory::createSceneItem(sceneItemType);
    if(sceneItem)
    {
        sceneItem->setId(id);
        sceneItem->setScene(this);
        sceneItem->setSceneItemType(sceneItemType);
        int index = sceneItems.getIndexToItem([sceneItemType](Vector<SceneItem*>& items){return items[0]->getSceneItemType().compare(sceneItemType) == 0; });
        if(index == -1)
        {
            Vector<SceneItem*> itemTypeArray;
            itemTypeArray.pushBack(sceneItem);
            sceneItems.pushBack(itemTypeArray);
        }
        else
            sceneItems[index].pushBack(sceneItem);
    }
    return sceneItem;
}

void Scene::createSceneItems(const Vector<const std::string>& sceneItemTypes, Vector<SceneItem*>& itemsOut)
{
    for(unsigned int i = 0; i < sceneItemTypes.size(); ++i)
        itemsOut.pushBack(createSceneItem(sceneItemTypes[i]));
}

void Scene::getSceneItemsByType(const std::string& sceneItemType, Vector<SceneItem*>& itemsOut) const
{
    sceneItems.findItem([sceneItemType](Vector<SceneItem*>& items){return items[0]->getSceneItemType() == sceneItemType;}, itemsOut);
}

void Scene::getAllRenderItems(Vector<RenderItem>& renderItemsOut) const
{
    Vector<Mesh*> meshes;
    getSceneItemsByType<Mesh>(meshes);

    for(unsigned int i = 0; i < meshes.size(); ++i)
        renderItemsOut.pushBack(meshes[i]->getRenderItems());
}

unsigned int Scene::getNumSceneItemsByType(const std::string& sceneItemType) const
{
    Vector<SceneItem*> itemTypeArray;
    getSceneItemsByType(sceneItemType, itemTypeArray);
    return itemTypeArray.size();
}

void Scene::setGlobalAmbientLight(const Vector3& ambientLight)
{
    globalAmbientLight = ambientLight;
}

void Scene::removeSceneItem(SceneItem* sceneItem)
{
    if(sceneItem)
    {
        std::string sceneItemType = sceneItem->getSceneItemType();
        int index = sceneItems.getIndexToItem([sceneItemType](Vector<SceneItem*>& items){return items[0]->getSceneItemType().compare(sceneItemType) == 0; });
        sceneItems[index].eraseUnordered(sceneItem);
        delete sceneItem;
    }
}

void Scene::removeAllSceneItem()
{
    for(unsigned int i = 0; i < sceneItems.size(); ++i)
    {
        for(unsigned int j = 0; j < sceneItems[i].size(); ++j)
        {
            sceneItems[i].eraseUnordered(sceneItems[i][j]);
            delete sceneItems[i][j];
        }
    }
}

}