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

ivec4 readAndDecodeShadowOcclusion2x2(in vec2 texCoord, in vec2 texOffset, in sampler2D texSampler)
{
    mat4 kernel;
    ivec4 decodedValue;
    kernel[0] = texture(texSampler, texCoord + vec2(-texOffset.x, -texOffset.y)) * 255.0f;  
    kernel[1] = texture(texSampler, texCoord + vec2(texOffset.x, -texOffset.y)) * 255.0f;
    kernel[2] = texture(texSampler, texCoord + vec2(-texOffset.x, texOffset.y)) * 255.0f;
    kernel[3] = texture(texSampler, texCoord + vec2(texOffset.x, texOffset.y)) * 255.0f;

    decodedValue.x = (int(kernel[0].r) << 24 | (int(kernel[0].g) << 16) | (int(kernel[0].b) << 8) | int(kernel[0].a));
    decodedValue.y = (int(kernel[1].r) << 24 | (int(kernel[1].g) << 16) | (int(kernel[1].b) << 8) | int(kernel[1].a));
    decodedValue.z = (int(kernel[2].r) << 24 | (int(kernel[2].g) << 16) | (int(kernel[2].b) << 8) | int(kernel[2].a));
    decodedValue.w = (int(kernel[3].r) << 24 | (int(kernel[3].g) << 16) | (int(kernel[3].b) << 8) | int(kernel[3].a));
    return decodedValue;
}

float PCF4(in ivec4 occlusionValues, in int mask)
{
    float value = ((occlusionValues.x & mask) == mask) ? 0.0f : 1.0f; 
    value += ((occlusionValues.y & mask) == mask) ? 0.0f : 1.0f;
    value += ((occlusionValues.z & mask) == mask) ? 0.0f : 1.0f;
    value += ((occlusionValues.w & mask) == mask) ? 0.0f : 1.0f;
    return value / 4.0f;
}