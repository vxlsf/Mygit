/*
 * ThreadSync.h
 *
 *  Created on: 2011-10-8
 *      Author: qianggw
 */

#ifndef PMGW_THREADSYNC_H_
#define PMGW_THREADSYNC_H_

#include <pthread.h>

class CondH;

// Mutex
class MutexH {
public:
    MutexH() {
        pthread_mutexattr_t mutex_attr;
        pthread_mutexattr_init(&mutex_attr);
        pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&_mutex, &mutex_attr);
    }
    virtual ~MutexH() { pthread_mutex_destroy(&_mutex); }

    void Lock() { pthread_mutex_lock(&_mutex); }
    bool TryLock() {
        if (pthread_mutex_trylock(&_mutex) == 0) {
            return true;
        }
        return false;
    }
    void UnLock() { pthread_mutex_unlock(&_mutex); }

private:
    friend class CondH;
    pthread_mutex_t _mutex;
};

class MutexGuardH {
public:
    MutexGuardH(MutexH *mutex) : _mutex(mutex) {
        if (_mutex) {
            _mutex->Lock();
        }
    }
    ~MutexGuardH() {
        if (_mutex) {
            _mutex->UnLock();
            _mutex = NULL;
        }
    }

private:
    MutexGuardH(const MutexGuardH&);
    MutexGuardH& operator=(const MutexGuardH&);

private:
    MutexH *_mutex;
};

// Cond
class CondH {
public:
    CondH() { pthread_cond_init(&_cond, NULL); }
    virtual ~CondH() { pthread_cond_destroy(&_cond); }

    void Singal() { pthread_cond_signal(&_cond); }
    void Wait(MutexH &mutex) { pthread_cond_wait(&_cond, &mutex._mutex); }

private:
    pthread_cond_t _cond;
};

// RwLock
class RwLockH {
public:
    RwLockH() { pthread_rwlock_init(&_rwlock, NULL); }
    ~RwLockH() { pthread_rwlock_destroy(&_rwlock); }

    int RdLock() { return pthread_rwlock_rdlock(&_rwlock); }
    int WrLock() { return pthread_rwlock_wrlock(&_rwlock); }
    int UnLock() { return pthread_rwlock_unlock(&_rwlock); }
    int TryRdLock() { return pthread_rwlock_tryrdlock(&_rwlock); }
    int TryWrLock() { return pthread_rwlock_trywrlock(&_rwlock); }

private:
    pthread_rwlock_t _rwlock;
};

class RwLockGuardH {
public:
    enum {RwLockType_Read = 0, RwLockType_Write = 1, };

    RwLockGuardH(RwLockH *rwlock, int type) : _rwlock(rwlock) {
        if (_rwlock) {
            if (RwLockType_Read == type) {
                _rwlock->RdLock();
            }
            else {
                _rwlock->WrLock();
            }
        }
    }
    ~RwLockGuardH() {
        if (_rwlock) {
            _rwlock->UnLock();
        }
    }

private:
    RwLockGuardH(const RwLockGuardH&);
    RwLockGuardH& operator=(const RwLockGuardH&);

private:
    RwLockH *_rwlock;
};



#endif /* PMGW_THREADSYNC_H_ */
