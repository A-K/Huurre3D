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

#ifndef OGLDefs_H
#define OGLDefs_H

#include "Graphics/OGLGraphicsBackEnd/OGLHeaders.h"

namespace Huurre3D
{

extern const GLenum glWrapMode[];
extern const GLenum glMinFilter[];
extern const GLenum glMinMipMapFilter[];
extern const GLenum glMaxFilter[];
extern const GLenum glPixelFormat[];
extern const GLenum glPixelInternalFormat[];
extern const GLenum glPixelType[];
extern const GLenum glTextureTarget[];
extern const GLenum glCubeMapFaces[];
extern const GLenum glAttributeType[];
extern const GLenum glPrimitiveType[];
extern const GLenum glIndexType[];
extern const GLsizei glIndexSize[];
extern const GLenum glBlendSourceFactor[];
extern const GLenum glBlendDestFactor[];
extern const GLenum glCompareMode[];
extern const GLenum glCullMode[];
extern const GLchar* glAttributeName[];
extern const GLenum glUniformType[];
extern const GLchar* glShaderDefines[];
extern const GLchar* glShaderParameters[];
extern const GLchar* glTextureSlots[];
extern const GLenum shaderTypes[];

}
#endif