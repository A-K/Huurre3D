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
    removeSceneItems(sceneItems);
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
        sceneItems.pushBack(sceneItem);
    }
    return sceneItem;
}

void Scene::createSceneItems(const Vector<const std::string>& sceneItemTypes, Vector<SceneItem*>& itemsOut)
{
    for(unsigned int i = 0; i < sceneItemTypes.size(); ++i)
        itemsOut.pushBack(createSceneItem(sceneItemTypes[i]));
}

SceneItem* Scene::getSceneItem(unsigned int id) const
{
    SceneItem* result;
    return sceneItems.findItem([id](SceneItem* item){return item->getId() == id;}, result) ? result : nullptr;
}

SceneItem* Scene::getSceneItem(const std::string& sceneItemType) const
{
    SceneItem* result;
    return sceneItems.findItem([sceneItemType](SceneItem* item){return item->getSceneItemType() == sceneItemType;}, result) ? result : nullptr;
}

void Scene::getSceneItemsByType(const std::string& sceneItemType, Vector<SceneItem*>& itemsOut) const
{
    sceneItems.findItems([sceneItemType](SceneItem* item){return item->getSceneItemType() == sceneItemType;}, itemsOut);
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
    unsigned int numItems = 0;

    for(unsigned int i = 0; i < sceneItems.size(); ++i)
    {
        if(sceneItems[i]->getSceneItemType().compare(sceneItemType) == 0)
            numItems++;
    }

    return numItems;
}

void Scene::setGlobalAmbientLight(const Vector3& ambientLight)
{
    globalAmbientLight = ambientLight;
}

void Scene::removeSceneItem(SceneItem* sceneItem)
{
    if(sceneItem)
    {
        sceneItems.eraseUnordered(sceneItem);
        delete sceneItem;
    }
}

}