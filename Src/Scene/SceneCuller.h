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

#ifndef SceneCuller_H
#define SceneCuller_H

#include "Scene/Scene.h"
#include "Scene/Light.h"

namespace Huurre3D
{

class SceneCuller
{
public:
    SceneCuller() = default;
    ~SceneCuller() = default;
    
    template<class BoundingVolume> void cullRenderItems(const Vector<RenderItem>& items, Vector<RenderItem>& result, const BoundingVolume& volume)
    {
        for(unsigned int i = 0; i < items.size(); ++i)
        {
            BoundingBox worlBoundingBox = items[i].geometry->getWorldBoundingBox();
            if(volume.isInsideNoIntersection(worlBoundingBox))
                result.pushBack(items[i]);
        }
    }

    template<class BoundingVolume> void cullRenderItems(const Scene* scene, Vector<RenderItem>& result, const BoundingVolume& volume)
    {
        Vector<Mesh*> meshes;
        scene->getSceneItemsByType<Mesh>(meshes);

        Matrix4x4 worldTransform;
        for(unsigned int i = 0; i < meshes.size(); ++i)
        {
            worldTransform = meshes[i]->getWorldTransform4x4();
            auto items = meshes[i]->getRenderItems();
            
            //Test if the mesh is inside the bounding volume.
            switch(volume.isInside(meshes[i]->getBoundingBox().transformed(worldTransform)))
            {	
                //case Intersection::Outside:
                    //continue;
                case Intersection::Inside:
                    //If mesh is fully inside the frustum, add all renderItems at once.
                    result.pushBack(items);
                    break;
                case Intersection::Intersects:
                    //Cull each render item.
                    cullRenderItems(items, result, volume);
                    break;
            }
        }
    }

    template<class BoundingVolume> void cullLights(const Scene* scene, Vector<Light*>& result, const BoundingVolume& volume)
    {
        Vector<Light*> lights;
        scene->getSceneItemsByType<Light>(lights);

        //Cull light bounding volumes against the given bounding volume.
        for(unsigned int i = 0; i < lights.size(); ++i)
        {
            Sphere worldSpaceBoundingSphere(lights[i]->getPosition(FrameOfReference::World), lights[i]->getRadius());

            if(volume.isInsideNoIntersection(worldSpaceBoundingSphere))
                result.pushBack(lights[i]);
        }
    }
};

}

#endif