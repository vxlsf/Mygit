
#include <stdlib.h>
#include <stdio.h>
#include "TaskThreadPool.h"
#include "TaskRunnable.h"

using namespace std;


TaskThreadPoolH::TaskThreadPoolH(const std::string &prefix, int threadnumber)
: _stop(false),
  _threadnumber(threadnumber),
  _threadNamePrefix(prefix),
  _taskQueue(NULL),
  _threads(NULL) {

    _threads = new ThreadH[_threadnumber];
}

TaskThreadPoolH::~TaskThreadPoolH() {
    if (!_stop) {
        stop();
    }
    delete[] _threads;
}

bool TaskThreadPoolH::start() {
    char buffer[100];
    for (int i = 0; i < _threadnumber; ++i) {
        TaskRunnableH *runnable = new TaskRunnableH();
        runnable->setTaskQueue(_taskQueue);
        runnable->setDelFlag(true);     // The thread end will be delete this runnable.
        sprintf(buffer, "%s-%d", _threadNamePrefix.c_str(), i);
        _threads[i].setThreadName(buffer);
        _threads[i].start(runnable);
    }
    return true;
}

void TaskThreadPoolH::stop() {
    if (!_stop) {
        for (int i = 0; i < _threadnumber; ++i) {
            _threads[i].stop();
        }
    }
    _stop = true;
}

int TaskThreadPoolH::getWorkThreadCount() {
    int count = 0;
    for (int i = 0; i < _threadnumber; ++i) {
        if (RUNNING == _threads[i].getStatus()) {
            ++count;
        }
    }
    return count;
}

int TaskThreadPoolH::getIdleThreadCount() {
    return (_threadnumber - getWorkThreadCount());
}

