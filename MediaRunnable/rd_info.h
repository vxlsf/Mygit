#ifndef __RD_INFO_
#define __RD_INFO_

#include <pthread.h>
#include <map>
#include <string>
#include <set>
#include "ThreadSync.h"
#include "TaskThreadPool.h"

#define TYPEERROR "{\"status\":0,\"message\":\"Not a registered runnable type.\"}"
#define MESSAGE_OK "{\"status\":1,\"message\":\"ok.\"}"
#define TYPEEMPTY "{\"status\":0,\"message\":\"Type is empty.\"}"
#define VALUEEMPTY "{\"status\":0,\"message\":\"Value is empty.\"}"
#define KEYEMPTY   "{\"status\":0,\"message\":\"Key is empty.\"}"
#define CHECKRECORDOK "{\"status\":1,\"message\":\"This is a recorded record.\"}"
#define CHECKRECORDERR "{\"status\":0,\"message\":\"This isn't a recorded record.\"}"

typedef struct global_conf_t
{
    std::map< std::string, std::map<std::string, std::string> >   record_map;
    RwLockH                                                       record_map_mutex;

    std::set<std::string>                                       record_set;
    RwLockH                                                      record_set_mutex;

    std::string                                                 key_type_file;

    std::string                                                 key_log_file;
    int                                                         thread_pool_number;                                                    
    TaskThreadPoolH *                                            thread_pool;   

	std::string                                             server_ip;
    

}global_conf;

#endif
