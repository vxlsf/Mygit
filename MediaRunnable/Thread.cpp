
#include "Thread.h"
#include <cstdio>


ThreadH::ThreadH()
: _status(NONE),
  _runnable(NULL) {

    pthread_attr_init(&_attr);
}

ThreadH::~ThreadH() {
    stop();
    pthread_attr_destroy(&_attr);
}

int ThreadH::start(RunnableH * runnable) {
    if (NULL == runnable) {
        //Hlr_trace(PMGW_TRACE_INFO, "Runnalbe is NULL, didn't create a new thread.");
        return 1;
    }
    // Call stop() to release resources
    stop();

    if (RUNNING == getStatus()) {
        //Hlr_trace(PMGW_TRACE_INFO, "[%d] Thread is working.", _pthread);
        return 2;
    }

    setRunnable(runnable);
    setStatus(CREATED);
    int rc = pthread_create(&_pthread, &_attr, &threadFunction, (void*)this);
    if (rc != 0) {
        //Hlr_trace(PMGW_TRACE_INFO, "pthread_create() failed[%d].", rc);
        setStatus(NONE);
        return -1;
    }

    return 0;
}

void ThreadH::stop() {
    // The runnable can stop by it, so that thread no need to stop runnable.
    if (_runnable && !_runnable->shouldStop()) {
        _runnable->stop();
    }
    join();
    delRunnable();
}

void* ThreadH::threadFunction(void *argv) {
    ThreadH *thread = static_cast<ThreadH*>(argv);
    RunnableH *runnable = thread->getRunnable();

    if (NULL == runnable) {
        //Hlr_trace(PMGW_TRACE_INFO, "There is no runnable, thread exit.");
        return (void*)0;
    }

    runnable->setThreadName(thread->getThreadName());
    if (!runnable->initialize()) {
        return (void*)0;
    }

    thread->setStatus(RUNNING);
    runnable->run();
    runnable->clean();
    thread->setStatus(STOPPED);
    return (void*)0;
}


void ThreadH::delRunnable() {
    if (_runnable && _runnable->shouldDel()) {
        delete _runnable;
        _runnable = NULL;
    }
}

