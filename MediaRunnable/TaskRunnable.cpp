/*
 * TaskRunnable.cpp
 *
 *  Created on: 2011-6-18
 *      Author: ruihaiji
 */

#include "TaskRunnable.h"
#include "Task.h"
#include "TaskQueue.h"



TaskRunnableH::TaskRunnableH()
: RunnableH(),
  _queue(NULL) {
}

TaskRunnableH::~TaskRunnableH() {
}


int TaskRunnableH::run() {
//    Hlr_trace(PMGW_TRACE_INFO, "Begin");

    while (!shouldStop()) {
        //Hlr_trace(PMGW_DEBUG, "thread name=%s wait for task", getThreadName().c_str());

        if (NULL == _queue) {
          //  Hlr_trace(PMGW_TRACE_INFO, "thread name=%s queue is null", getThreadName().c_str());
            break;
        }
        TaskH *task = _queue->getTask();

   //     Hlr_trace(PMGW_DEBUG, "thread name=%s got task", getThreadName().c_str());

        if (NULL != task) {
            task->process();

            /////////////////////////////////////////////////
            //Attention! make sure other threads doesn't delete the task

            if (task->shouldDelete()) {
      //          Hlr_trace(PMGW_DEBUG, "thread name=%s delete task", getThreadName().c_str());
                delete task;
                task = NULL;
            }
        }
       // Hlr_trace(PMGW_DEBUG, "thread name=%s task done", getThreadName().c_str());
    }

//    Hlr_trace(PMGW_TRACE_INFO, "End");
    return 0;

}

void TaskRunnableH::stop() {
    RunnableH::stop();
    if (_queue)
        _queue->singalQueue();
}

