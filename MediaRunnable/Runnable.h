
#ifndef PMGW_RUNNABLE_H_
#define PMGW_RUNNABLE_H_

#include <string>

class RunnableH {
public:
    RunnableH();
    virtual ~RunnableH();

    virtual bool initialize() { return true; }
    virtual int run() = 0;
    virtual void clean() {}
    virtual void stop() { _stopFlag = true; }

    std::string &getThreadName() { return _threadName; }
    void setThreadName( std::string name ) { _threadName = name; }

    bool shouldStop() { return _stopFlag;}

    void setDelFlag(bool delFlag) { _delFlag = delFlag; }
    bool shouldDel() { return _delFlag; }

private:
    std::string _threadName;
    bool _stopFlag;
    bool _delFlag;

};

#endif /* PMGW_RUNNABLE_H_ */
