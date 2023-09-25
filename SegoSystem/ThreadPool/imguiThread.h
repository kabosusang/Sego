#pragma once
#include <thread>
#include <mutex>
#include <future>
#include "core.h"


template<class Function,class ARGs>
class Sego_API SGpack_thread
{
public:

    SGpack_thread(Function fuc ,ARGs fucAgs):f(fuc) ,Thread_Args(fucAgs)
    {
        Sg_thread = std::make_unique<std::thread>(RunThread, this);
    }

    SGpack_thread(const SGpack_thread&) = delete;
    SGpack_thread& operator=(const SGpack_thread&) = delete;

    ~SGpack_thread()
    {
        if (Sg_thread && Sg_thread->joinable())
    {
        Sg_thread->join();
    }
    }
 
    void RunThread()
    {
         f(Thread_Args);
    }
private:
    //只用于void
    using VoidFunction = void (*)(ARGs);
    VoidFunction f;
    
    ARGs Thread_Args;
    std::unique_ptr<std::thread> Sg_thread;
    //std::future Sg_future;
};







