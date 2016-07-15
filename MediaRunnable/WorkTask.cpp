#include "WorkTask.h"
#include "ThreadSync.h"
#include "mylog.h"
#include "rd_info.h"
#include <iostream>


extern global_conf public_global_conf; 

WorkTask :: WorkTask(std::string str_oper, const reqinfo &request):TaskH(true) 
{
    setDeleteFlag(true);
    key = request.key;
    value = request.value;
    type = request.type;
    oper = str_oper; 
}


int WorkTask :: process()
{
    RwLockGuardH lock_map_guard(&public_global_conf.record_map_mutex, 1);

    bool flag = false;

    if (oper.find("ADD") != std::string::npos)
    {
     //   public_global_conf.record_map[type].insert(make_pair(key, value));
        public_global_conf.record_map[type][key] = value;
        flag = true;
    }
    else if (oper.find("DEL") != std::string::npos)
    {
        std::map<std::string, std::string > :: iterator iter = public_global_conf.record_map[type].find(key);
        if (iter != public_global_conf.record_map[type].end())
        {
            public_global_conf.record_map[type].erase(iter);
            flag = true;
        }
    }

    if (flag == true)
        MyLog :: KeyLog(oper +"KEY:" + key + "VALUE:" + value + "TYPE:" + type);
}
