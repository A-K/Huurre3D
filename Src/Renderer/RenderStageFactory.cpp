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

#include "RenderStageFactory.h"

namespace Huurre3D
{

RenderStageCreator::RenderStageCreator(const std::string& renderStageType) :
renderStageType(renderStageType)
{
     RenderStageFactory::registerCreator(this);
}

RenderStage* RenderStageFactory::createRenderStage(Renderer* renderer, const std::string& renderStageType)
{
    int index = getCreatorContainer().getIndexToItem([renderStageType](const RenderStageCreator* creator){return creator->renderStageType.compare(renderStageType) == 0; });
    return index != -1 ? getCreatorContainer()[index]->createRenderStage(renderer) : nullptr;
}

void RenderStageFactory::registerCreator(RenderStageCreator* creator)
{
    getCreatorContainer().pushBack(creator);
}

Vector<RenderStageCreator*>& RenderStageFactory::getCreatorContainer()
{
    static Vector<RenderStageCreator*> creatorContainer;
    return creatorContainer;
}

}
