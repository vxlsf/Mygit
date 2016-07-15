
#include "TaskQueue.h"
#include "Task.h"


TaskQueueH::TaskQueueH(int maxQueueSize)
: _maxQueueSize(maxQueueSize) {
}

TaskQueueH::~TaskQueueH() {
    TaskH *task = NULL;
    MutexGuardH guard(&_queueMutex);
    while (!_queue.empty()) {
        task = _queue.front();
        _queue.pop();
        if (task->shouldDelete()) {
            delete task;
        }
    }
}

int TaskQueueH::addTask(TaskH *task) {
    if (NULL == task) {
        return 0;
    }

    MutexGuardH guard(&_queueMutex);
    if (_queue.size() >= _maxQueueSize) {
        return 1;
    }

    _queue.push(task);
    _cond.Singal();

    return 0;
}

TaskH *TaskQueueH::getTask() {
    TaskH * task = NULL;
    MutexGuardH guard(&_queueMutex);
    if (_queue.empty()) {
        _cond.Wait(_queueMutex);
        if (_queue.empty()) {
            return NULL;
        }
    }
    task = _queue.front();
    _queue.pop();

    return task;
}

int TaskQueueH::getQueueCount() {
    MutexGuardH guard(&_queueMutex);
    return _queue.size();
}

