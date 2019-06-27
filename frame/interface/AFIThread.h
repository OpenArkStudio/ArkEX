/*
* This source file is part of ARK
* For the latest info, see https://github.com/QuadHex
*
* Copyright (c) 2013-2018 QuadHex authors.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/

#ifndef _AFITHREAD_H
#define _AFITHREAD_H

#include "base/AFPlatform.hpp"
#include "interface/AFIPluginContainer.h"
#include "interface/AFIThreadEventsManager.h"

#if ARK_PLATFORM == PLATFORM_WIN
#include <windows.h>
#include <process.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#else
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#endif

#include "AFIPluginContainer.h"

namespace ark
{
    class AFILogicThreadManager;

    //Thread state
    enum ThreadState
    {
        ARK_THREAD_STATE_NONE = 0,
        ARK_THREAD_STATE_INIT,
        ARK_THREAD_STATE_LOGIC_RUN_BEGIN,
        ARK_THREAD_STATE_LOGIC_RUN_END,
        ARK_THREAD_STATE_LOGIC_PAUSE,
        ARK_THREAD_STATE_LOGIC_ERROR,
        ARK_THREAD_STATE_LOGIC_CLOSE,
        ARK_THREAD_STATE_WAIT_OTHER_THREAD,
        ARK_THREAD_STATE_LOGIC_FINISH
    };

    enum ThreadError
    {
        ARK_THREAD_ERROR_CLOSE = 0,
        ARK_THREAD_ERROR_CONTINUE
    };

    enum ThreadLogicErrorType
    {
        ARK_THREAD_LOGIC_ERROR = 0,
        ARK_THREAD_LOGIC_TIMEOUT
    };

    enum ThreadEventGetType
    {
        ARK_THREAD_EVENT_GET_SINGLE = 0,
        ARK_THREAD_EVENT_GET_ALL,
    };

#define PARAM_THREAD_ID "threadid"
#define PARAM_THREAD_TYPE "threadtype"

    //manager point
    class AFIManager
    {
    public:
        AFIManager() : thread_event_manager_(NULL), logic_thread_manager_(NULL)
        {
        };

        ~AFIManager() {};

        void Init(AFIThreadEventsManager* thread_event_manager, AFILogicThreadManager* logic_thread_manager)
        {
            thread_event_manager_ = thread_event_manager;
            logic_thread_manager_ = logic_thread_manager;
        };

        AFIThreadEventsManager* GetThreadEventManager()
        {
            return thread_event_manager_;
        }

        AFILogicThreadManager* GetLogicThreadManager()
        {
            return logic_thread_manager_;
        }

    private:
        AFIThreadEventsManager* thread_event_manager_;
        AFILogicThreadManager* logic_thread_manager_;
    };

    //logic thread
    class AFIThread
    {
    public:
        AFIThread() {};
        virtual ~AFIThread() {}

        virtual int GetThreadLogicID() = 0;

        virtual void Lock() = 0;

        virtual void UnLock() = 0;

        virtual void SaveLastRunTimeBegin() = 0;

        virtual void SaveLastRunTimeEnd() = 0;

        virtual void SetThreadState(ThreadState thread_state) = 0;

        virtual ThreadState GetThreadState() = 0;

        virtual AFIManager* GetManager() = 0;

        virtual void SetCond(int interval_timeout) = 0;

        virtual ThreadEventGetType GetThreadEventGetType() = 0;
    };

#if ARK_PLATFORM == PLATFORM_WIN
    typedef HANDLE ThreadID;
    typedef CRITICAL_SECTION ThreadMutex;
    typedef CONDITION_VARIABLE ThreadCond;
#define HANDEL_ERROR_VALUE INVALID_HANDLE_VALUE
#else
    typedef pthread_t ThreadID;
    typedef pthread_mutex_t ThreadMutex;
    typedef pthread_cond_t ThreadCond;
#define HANDEL_ERROR_VALUE -1
#endif
}

#endif //_AFITHREAD_H