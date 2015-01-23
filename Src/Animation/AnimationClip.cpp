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

#include "AnimationClip.h"
#include <iostream>

namespace Huurre3D
{

AnimationClip::AnimationClip(const std::string& name, float animationLength, bool looped, const Vector<Track>& tracks):
name(name),
animationLength(animationLength),
endPosition(animationLength),
looped(looped),
tracks(tracks)
{
}

void AnimationClip::setSpeed(float speed)
{
    this->speed = speed;
}

void AnimationClip::setStartPosition(float time)
{
    startPosition = clamp(time, 0.0f, animationLength);
    currentTime = startPosition;
}

void AnimationClip::setEndPosition(float time)
{
    endPosition = clamp(time, startPosition, animationLength);
}

void AnimationClip::setRange(float startPosition, float endPosition)
{
    setStartPosition(startPosition);
    setEndPosition(endPosition);
}

void AnimationClip::setTime(float time)
{
    currentTime = clamp(time, startPosition, endPosition);
}

void AnimationClip::advance(float delta)
{
    currentTime += delta * speed;

    currentTime = currentTime <= startPosition ? startPosition : currentTime;

    if(currentTime > endPosition)
        currentTime = looped ? startPosition : endPosition;

    for(unsigned int i = 0; i < tracks.size(); ++i)
    {
        unsigned int keyFrameIndex = tracks[i].lastKeyFrameIndex;
        unsigned int keyFramesSize = tracks[i].keyFrames.size();

        if(keyFramesSize == 1)
        {
            tracks[i].valueToModify.setTransform(tracks[i].keyFrames[0].position,
                                                 tracks[i].keyFrames[0].rotation,
                                                 tracks[i].keyFrames[0].scale);
        }
        else
        {
            
            //Find the keyFrame index
            while(keyFrameIndex > 0 && currentTime < tracks[i].keyFrames[keyFrameIndex].time)
                --keyFrameIndex;

            while(keyFrameIndex < keyFramesSize - 1 && currentTime >= tracks[i].keyFrames[keyFrameIndex + 1].time)
                ++keyFrameIndex;

            unsigned int nextKeyFrameIndex = keyFrameIndex + 1;
            if(nextKeyFrameIndex >= keyFramesSize)
            {
                if(looped)
                    nextKeyFrameIndex = 0;
                else
                {
                    tracks[i].valueToModify.setTransform(tracks[i].keyFrames[keyFrameIndex].position,
                                                         tracks[i].keyFrames[keyFrameIndex].rotation,
                                                         tracks[i].keyFrames[keyFrameIndex].scale);

                    break;
                }
            }

            float timeInterval = tracks[i].keyFrames[nextKeyFrameIndex].time - tracks[i].keyFrames[keyFrameIndex].time;
            if(timeInterval < 0.0f)
                timeInterval += endPosition;

            //Calculate a weight for interpolation, which is the percent of the time interval that has passed.
            float t = timeInterval > 0.0f ? (currentTime - tracks[i].keyFrames[keyFrameIndex].time) / timeInterval : 1.0f;
            tracks[i].valueToModify.setTransform(tracks[i].keyFrames[keyFrameIndex].position.lerp(tracks[i].keyFrames[nextKeyFrameIndex].position, t),
                                                 tracks[i].keyFrames[keyFrameIndex].rotation.slerp(tracks[i].keyFrames[nextKeyFrameIndex].rotation, t),
                                                 tracks[i].keyFrames[keyFrameIndex].scale.lerp(tracks[i].keyFrames[nextKeyFrameIndex].scale, t));

            tracks[i].lastKeyFrameIndex = nextKeyFrameIndex;
            
        } 
    }
}

}