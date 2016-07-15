/*
 * TaskRunnable.h
 *
 *  Created on: 2011-6-18
 *      Author: ruihaiji
 */

#ifndef PMGW_TASKRUNNABLE_H_
#define PMGW_TASKRUNNABLE_H_

#include "Runnable.h"


class TaskQueueH;

class TaskRunnableH : public RunnableH {
public:
    TaskRunnableH();
    virtual ~TaskRunnableH();

    virtual bool initialize() { return true;};
    virtual int run() ;
    virtual void clean(){}

    void stop();

    void setTaskQueue(TaskQueueH *queue) { _queue = queue;}

private:
    TaskQueueH *_queue;
};


#endif /* PMGW_TASKRUNNABLE_H_ */
