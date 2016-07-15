/*
 * TaskQueue.h
 *
 *  Created on: 2011-5-15
 *      Author: ruihaiji
 */

#ifndef PMGW_TASKQUEUE_H_
#define PMGW_TASKQUEUE_H_

#include <queue>
#include <string>
#include "ThreadSync.h"


const int DEFAULT_MAXQUEUESIZE = 100;   // Max task count

class TaskH;

/*
 * FIFO
 */
class TaskQueueH {
public:
    TaskQueueH(int maxQueueSize = DEFAULT_MAXQUEUESIZE);
    virtual ~TaskQueueH();

    /*
     * Return: 0 OK
     *         1 Queue is full
     */
    int addTask(TaskH *task);

    /*
     * If the queue is empty, wait another thread to add task
     */
    TaskH *getTask();

    int getQueueCount();

    void singalQueue() {
        MutexGuardH guard(&_queueMutex);
        _cond.Singal();
    }

    void setQueueName(std::string queue_name) { _queueName = queue_name; }

    std::string getQueueName() { return _queueName; }
    
private:
    unsigned int _maxQueueSize;
    std::string _queueName;
    std::queue<TaskH*> _queue;
    MutexH _queueMutex;
    CondH _cond;    
};




#endif /* PMGW_TASKQUEUE_H_ */
