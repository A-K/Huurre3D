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

#include "Animation.h"
#include <iostream>

namespace Huurre3D
{

Animation::~Animation()
{
    removeAnimationClips(animationClips);
}

AnimationClip* Animation::createAnimationClip(const std::string& name, float animationLength, bool looped, const Vector<Track>& tracks)
{
    AnimationClip* animationClip = new AnimationClip(name, animationLength, looped, tracks);
    animationClips.pushBack(animationClip);
    return animationClip;
}

void Animation::removeAnimationClip(AnimationClip* animationClip)
{
    if(animationClip)
    {
        animationClips.eraseUnordered(animationClip);
        delete animationClip;
    }
}

void Animation::removeAnimationClips(Vector<AnimationClip*>& animationClips)
{
    for(unsigned int i = 0; i < animationClips.size(); ++i)
        removeAnimationClip(animationClips[i]);
}

void Animation::update(float timeStep)
{
    for(unsigned int i = 0; i < animationClips.size(); ++i)
    {
        if(animationClips[i]->isPlaying())
            animationClips[i]->advance(timeStep);
    }
}

}