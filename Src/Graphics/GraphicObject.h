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

#ifndef GraphicObject_H
#define GraphicObject_H

#include "Graphics/GraphicDefs.h"

namespace Huurre3D
{

class GraphicSystem;

class GraphicObject
{
public:
    GraphicObject() = default;
    virtual ~GraphicObject() = default;

    void setId(unsigned int id) {this->id = id;}
    unsigned int getId() const {return id;}
    bool isDirty() const {return dirty;}
    void unDirty() {dirty = false;}
    void discardData() {graphicData.resetBuffer();}
    unsigned char* getGraphicData() const {return graphicData.getData();}

protected:
    MemoryBuffer graphicData;
    unsigned int id = 0;
    bool dirty = false;
};

}

#endif