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

#ifndef RendererDefs_H
#define RendererDefs_H

namespace Huurre3D
{
    static const unsigned int MaxNumMaterials = 255; //Material index is stored in Gbuffer using one byte.
    static const unsigned int MaXNumLights = 1000; //The size of one light parameter is 64 bytes an 64k bytes is the maximum size of an an uniform buffer.
    static const unsigned int LightTileWidth = 32;
    static const unsigned int LightTileHeight = 32;
    static const unsigned int NumSAOSamples = 11;
    static const unsigned int NumSAOSpiralTurns = 9;
    static const unsigned int MaxSplits = 4;
    static const int ShadowMapSize = 2048; //4096; //1024;
    static const unsigned int MaxNumShadowLights = 32; //Int bitflag.
}

#endif