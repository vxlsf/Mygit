#include "mylog.h"

LoggerPtr DebugInfoLog(Logger::getLogger("debuginfo"));

LoggerPtr KeyInfoLog(Logger::getLogger("keyloginfo"));

LoggerPtr KeyTypeInfoLog(Logger::getLogger("keytypeinfo"));
MyLog::MyLog()
{
}

MyLog::~MyLog()
{
}


void MyLog::KeyLog(std::string debuginfo)
{
    LOG4CXX_DEBUG(KeyInfoLog,debuginfo );
}

void MyLog::KeyTypeLog(std::string debuginfo)
{
    LOG4CXX_DEBUG(KeyTypeInfoLog, debuginfo);
}


void MyLog::DebugLog(std::string debuginfo)
{
    LOG4CXX_DEBUG(DebugInfoLog,debuginfo);
}

