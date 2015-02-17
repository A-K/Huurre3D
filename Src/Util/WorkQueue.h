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

#ifndef WorkQueue_H
#define WorkQueue_H

#include "Util/Vector.h"
#include "Util/FixedArray.h"
#include <queue>
#include <thread>
#include <condition_variable>
#include <future>
#include <functional>

namespace Huurre3D
{

//Implementation is based on https://github.com/progschj/ThreadPool
template<unsigned int WorkerCount> class WorkQueue
{
public:
    WorkQueue()
    {
        createWorkerThreads();
    }

    ~WorkQueue()
    {
        stop = true;
        condition.notify_all();
        for(unsigned int i = 0; i < workers.size(); ++i)
            workers[i].join();
    }

    template<class F> std::future<typename std::result_of<F()>::type> submitTask(F&& func)
    {
        using resultType = typename std::result_of<F()>::type;
        auto task = std::make_shared<std::packaged_task<resultType()>>(std::move(func));
        std::future<resultType> result(task->get_future());
       
        std::unique_lock<std::mutex> lock(access);
        tasks.emplace([task](){(*task)(); });
        condition.notify_one();
        return result;
    }

private:
    void createWorkerThreads()
    {
        for(unsigned int i = 0; i < workers.size(); ++i)
        {
            std::thread worker([this]()
            {
                while(!stop)
                {
                    std::unique_lock<std::mutex> lock(access);
                    condition.wait(lock, [this]{return this->stop || !this->tasks.empty();});

                    if(!this->stop && !this->tasks.empty())
                    {
                        std::function<void()> task = std::move(this->tasks.front());
                        this->tasks.pop();
                        task();
                    }
                }
            });
            workers[i] = std::move(worker);
        }
    }

    FixedArray<std::thread, WorkerCount> workers;
    std::queue<std::function<void()>> tasks;//Vector<std::function<void()>> tasks;
    std::mutex access;
    std::condition_variable condition;
    bool stop = false;
};

}

#endif