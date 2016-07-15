#ifndef __MYLOG_H_
#define __MYLOG_H_

#include <log4cxx/logger.h>
#include <log4cxx/logstring.h>
#include <log4cxx/propertyconfigurator.h>


using namespace log4cxx;

class MyLog{
public:
    MyLog();
    ~MyLog();

public:
    static void DebugLog(std::string debuginfo);
    static void KeyLog(std::string debuginfo);
    static void KeyTypeLog(std::string debuginfo);
};

#endif
