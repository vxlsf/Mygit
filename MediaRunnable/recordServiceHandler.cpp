#include "basethriftserv.h"
#include "recordServiceHandler.h"
#include "rd_info.h"
#include "WorkTask.h"
#include <string>
#include <map>
#include <set>
#include "mylog.h"

extern global_conf public_global_conf;

int check_valid_request(std::string &_return , const reqinfo& request)
{
    if (request.type.empty())
    {   
        _return = TYPEEMPTY; 
        return -1;
    }   

    if (request.value.empty())
    {   
        _return = VALUEEMPTY;
        return -1;
    }   

    if (request.key.empty())
    {   
        _return = KEYEMPTY;
        return -1;
    }   

    return 0;
}

void recordServiceHandler ::Update(std::string& _return, const reqinfo& request)
{

	if (check_valid_request(_return , request) == -1)
        return ;

    RwLockGuardH lock_set_guard(&public_global_conf.record_set_mutex, 0);

    if (public_global_conf.record_set.count(request.type) == 0)
    { 
        _return = TYPEERROR;
        return ;
    }

    WorkTask * task = new WorkTask(std::string("ADD:"), request);
        
    public_global_conf.thread_pool->getTaskQueue()->addTask(task);

    _return = MESSAGE_OK;

	if (!m_bIsConn) 
	{
		printf("reconnect start\n");
		m_bIsConn = relay_Client->CreatProtocol_Bin() == 0 ? true : false;
		printf("reconnect end! m_bIsConn = %d\n", m_bIsConn? 1: 0);
	}
	
	if (m_bIsConn)
	{
		try
		{
			printf("start client.update!\n");
			relay_Client->getClient()->Update(_return, request);
			 printf("end client.update!\n");
		}
		catch (TException &tx)
		{
			printf("catch TException\n");
			m_bIsConn = false;
		}
	}
}

void recordServiceHandler :: GetAllKeyValue(resinfo& _return, const std::string& type)
{
    if (type.empty())
    {
        return;
    }

    RwLockGuardH lock_set_guard(&public_global_conf.record_set_mutex, 0);
    RwLockGuardH lock_map_guard(&public_global_conf.record_map_mutex, 0);

    if (public_global_conf.record_set.count(type))
    {
        _return.__set_keyValueMap(public_global_conf.record_map[type]);
        return ;
    }
}

void recordServiceHandler :: CheckOneRecord(std::string& _return, const reqinfo& request)
{
    if (check_valid_request(_return , request) == -1)
        return ;

    RwLockGuardH lock_set_guard(&public_global_conf.record_set_mutex, 0);

    if (public_global_conf.record_set.count(request.type) == 0)
    {
        _return = TYPEERROR;
        return ;
    }

    RwLockGuardH lock_map_guard(&public_global_conf.record_map_mutex, 0);

    std::map<std::string, std::string> :: iterator iter = public_global_conf.record_map[request.type].find(request.key);
    if (iter != public_global_conf.record_map[request.type].end())
    {
        _return  = CHECKRECORDOK;
        return ;
    }
    else
    {
        _return = CHECKRECORDERR;
        return ;
    }
}

void recordServiceHandler :: PushRunnableType(std::string& _return, const std::string& type)
{
    if (type.empty())
    {
        _return = TYPEEMPTY;
        return ;
    }

    RwLockGuardH lock_set_guard(&public_global_conf.record_set_mutex, 1);
   
    public_global_conf.record_set.insert(type);
        
    MyLog::KeyTypeLog("ADD:" +type);
    _return = MESSAGE_OK;
}

void recordServiceHandler :: DelOneRecord(std::string& _return, const reqinfo& request)
{
    if (check_valid_request(_return , request) == -1)
    {
        return ;
    }

    RwLockGuardH lock_set_guard(&public_global_conf.record_set_mutex, 0);

    if (public_global_conf.record_set.count(request.type) == 0)
    {
        _return = TYPEERROR;
        return ;
    }

    WorkTask * task = new WorkTask(std::string("DEL:"), request);
    public_global_conf.thread_pool->getTaskQueue()->addTask(task);

    _return = MESSAGE_OK;


}

void recordServiceHandler :: DelRunnableType(std::string& _return, const std::string& type)
{
    if (type.empty())
    {
        _return = TYPEERROR;
        return ;
    }

    RwLockGuardH lock_set_guard(&public_global_conf.record_set_mutex, 1);

    std::set<std::string>::iterator iter = public_global_conf.record_set.find(type);

    if (iter == public_global_conf.record_set.end())
    {
        _return = MESSAGE_OK;
        return ;
    }
    else
    {
        public_global_conf.record_set.erase(iter);

        MyLog::KeyTypeLog("DEL:" + type);
    }
}
