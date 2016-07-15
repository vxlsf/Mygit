
#ifndef __TASKTHREADPOOL_H_
#define __TASKTHREADPOOL_H_

#include <string>
#include "Thread.h"
#include "TaskQueue.h"


const int DEFAULT_THREADNUMBER = 4;

class TaskThreadPoolH {
public:
    TaskThreadPoolH(const std::string &prefix, int threadnumber = DEFAULT_THREADNUMBER);
    virtual ~TaskThreadPoolH();

    virtual bool start();
    virtual void stop();

    TaskQueueH *getTaskQueue() { return _taskQueue; }
    void setTaskQueue(TaskQueueH * queue) { _taskQueue = queue;}
    int getThreadCount() { return _threadnumber; }
    int getWorkThreadCount();
    int getIdleThreadCount();

private:
    bool _stop;	//when this is true, stop each thread
    int _threadnumber;
    std::string _threadNamePrefix;
    TaskQueueH *_taskQueue;
    ThreadH *_threads;
};


#endif /* TASKTHREADPOOL_H_ */
