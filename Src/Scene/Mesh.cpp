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

#include "Scene/Mesh.h"
#include "Renderer/Geometry.h"

namespace Huurre3D
{

SCENEITEM_TYPE_IMPL(Mesh);

void Mesh::addRenderItem(const RenderItem& renderItem)
{
    renderItems.pushBack(renderItem);

    if(!dirty)
        setDirty();
}

void Mesh::addRenderItems(const Vector<RenderItem>& renderItems)
{
    this->renderItems.pushBack(renderItems);

    if(!dirty)
        setDirty();
}

void Mesh::addRenderItem(Geometry *geometry, Material* material)
{
    renderItems.pushBack(RenderItem(material, geometry));
		
    if(!dirty)
        setDirty();
}

void Mesh::updateItem()
{
    if(transformDirty)
        updateWorldTransform();

    if(dirty)
    {
        for(unsigned int i = 0; i < renderItems.size(); ++i)
            boundingBox.mergeBoundingBox(renderItems[i].geometry->getBoundingBox());
		
        dirty = false;
    }

    for(unsigned int i = 0; i < renderItems.size(); ++i)
        renderItems[i].geometry->setWorldTransform(getWorldTransform4x4());

    settedForUpdate = false;
}

void Mesh::setMaterial(Material* material, unsigned int itemIndex)
{
    if(itemIndex < renderItems.size())
        renderItems[itemIndex].material = material;
}

Material* Mesh::getMaterial(unsigned int itemIndex) const 
{
    return itemIndex < renderItems.size() ? renderItems[itemIndex].material : nullptr;
}

}