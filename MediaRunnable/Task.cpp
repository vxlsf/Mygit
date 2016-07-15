
#include "Task.h"
#include <cstdio>


TaskH::TaskH(bool deleteFlag) :
_deleteFlag(deleteFlag),
_threadSpecificData(NULL) {
}

TaskH::~TaskH() {
}

