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

#ifndef AnimationClip_H
#define AnimationClip_H

#include "Util/Vector.h"
#include "Math/Quaternion.h"
#include "Scene/SpatialSceneItem.h"

namespace Huurre3D
{

struct KeyFrame
{
    float time = 0.0f;
    Vector3 position = Vector3::ZERO;
    Quaternion rotation = Quaternion::IDENTITY;
    Vector3 scale = Vector3::ONE;
};

struct Track
{
    unsigned int lastKeyFrameIndex = 0;
    Vector<KeyFrame> keyFrames;
    SpatialSceneItem& valueToModify;
    Track(SpatialSceneItem& valueToModify):
    valueToModify(valueToModify)
    {}
};

class AnimationClip
{
public:
    AnimationClip(const std::string& name, float animationLength, bool looped, const Vector<Track>& tracks);
    ~AnimationClip() = default;
    void setSpeed(float speed);
    //Sets a time position where the animation will start. 
    void setStartPosition(float time);
    //Sets a time position where the animation will end.
    void setEndPosition(float time);
    //Sets both start and end positions.
    void setRange(float startPosition, float endPosition);
    //sets the current time to the given time.
    void setTime(float time);
    void advance(float delta);
    void play() {playing = true;}
    void stop() {playing = false;}
    bool isPlaying() const {return playing;}
    const std::string& getname() const {return name;}
    float getLength() const {return animationLength;}

private:
    std::string name;
    float currentTime = 0.0f;
    float animationLength = 0.0f;
    float startPosition = 0.0f;
    float endPosition = 0.0f;
    float speed = 1.0f;
    bool playing = false;
    bool looped = true;
    Vector<Track> tracks;
};

}

#endif