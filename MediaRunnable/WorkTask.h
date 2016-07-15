#ifndef __WRITE_TASK_H_
#define __WRITE_TASK_H_

#include <string>
#include "Task.h"
#include "recordServiceHandler.h"

class WorkTask : public TaskH
{
    public :

        WorkTask(std::string str_oper,const reqinfo &request);
        int     process();

    private :

        std::string key;
        std::string value;
        std::string   type;
        std::string  oper;
};


#endif
