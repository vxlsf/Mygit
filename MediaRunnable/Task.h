
#ifndef PMGW_TASK_H_
#define PMGW_TASK_H_

    
class TaskH {
public:
//    Task(bool deleteFlag = true);
    TaskH(bool deleteFlag);
    virtual ~TaskH();
    //virtual int wait();
    //virtual void notify();
    virtual int process() = 0;
    //virtual bool isDone();
    //virtual Task * getParent();

    void *getThreadSpecificData() { return _threadSpecificData; }
    void setThreadSpecificData(void * data) { _threadSpecificData = data; }

    void setDeleteFlag(bool deleteFlag) { _deleteFlag = deleteFlag; }
    bool shouldDelete() { return _deleteFlag; }

private:
    bool _deleteFlag;
    void *_threadSpecificData;
};



#endif /* PMGW_TASK_H_ */
