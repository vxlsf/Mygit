
#ifndef PMGW_THREAD_H_
#define PMGW_THREAD_H_

#include <pthread.h>
#include <string>
#include "Runnable.h"


typedef enum {
        NONE,       // Before call pthread_create() or after call pthread_join().
        CREATED,    // After call pthread_create() but the runnable is not run.
        RUNNING,    // Runnable is running.
        STOPPED,    // Runnable was stoped, but not call pthread_join().
} THREAD_STATUS;


class ThreadH {
public:
    ThreadH();
    ~ThreadH();

    /*
     * Return: 0  OK
               -1 ERROR
               1  Runnable is NULL
               2  Another runnable is working
     */
    int start(RunnableH *runnable);
    void stop();

    THREAD_STATUS getStatus() { return _status; }

    void setRunnable(RunnableH *runnable) { _runnable = runnable; }
    RunnableH *getRunnable() { return _runnable; }

    void setThreadName(std::string threadName) { _threadName = threadName; }
    std::string getThreadName() { return _threadName; }

    static void *threadFunction(void *argv);

private:
    void setStatus(THREAD_STATUS status) { _status = status; }
    void delRunnable();
    void join() {
        if (getStatus() != NONE) {
            pthread_join(_pthread, NULL);
            setStatus(NONE);
        }
    }

private:
    std::string _threadName;
    THREAD_STATUS _status;
    RunnableH *_runnable;
    pthread_t _pthread;
    pthread_attr_t _attr;
};


#endif /* PMGW_THREAD_H_ */
