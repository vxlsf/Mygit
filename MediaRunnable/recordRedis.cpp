#include "recordServiceHandler.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <signal.h>
#include <unistd.h>
#include <set>
#include <pthread.h>
#include <map>
#include "mylog.h"
#include "configData.h"
#include "ThreadSync.h"
#include "rd_info.h"
#include "TaskThreadPool.h"
#include "recordRedis.h"
#include "TaskQueue.h"

#define _CFG_FILE "conf/recordRedis.cfg"

global_conf public_global_conf;



void load_key_type()
{
    std::ifstream m_stream;

    try
    {
        m_stream.open(public_global_conf.key_type_file.c_str());
        
        std::string str_tmp;
        while(std::getline(m_stream, str_tmp))
        {
            int pos ;
            if ((pos = str_tmp.find("ADD:")) != std::string::npos)
            {
                std::string str_key_type= str_tmp.substr(pos + strlen("ADD:"));

                public_global_conf.record_set.insert(str_key_type);
                
                MyLog::DebugLog("Add one key type :" + str_key_type );
            }

            if ((pos = str_tmp.find("DEL:")) != std::string::npos)
            {
                std::string str_key_type = str_tmp.substr(pos + strlen("DEL:"));

                std::set<std::string> :: iterator iter = public_global_conf.record_set.find(str_key_type);
                
                if (iter != public_global_conf.record_set.end())
                    public_global_conf.record_set.erase(iter);
                    
                MyLog::DebugLog("Delete one key type:" + str_key_type);
            }
        }
    }
    catch(...)
    {
        MyLog::DebugLog("load_key_type failed.");
        exit(1);
    }

    m_stream.close();
}

void load_key_log()
{
    std::ifstream m_stream;

    try
    {
        m_stream.open(public_global_conf.key_log_file.c_str());

        std::string str_tmp;
        
        while(std::getline(m_stream, str_tmp))
        {
            int key_pos, value_pos, type_pos ;
            std::string key, value, type;

            if ((key_pos = str_tmp.find("KEY:")) == std::string::npos) continue;

            if ((value_pos = str_tmp.find("VALUE:")) == std::string::npos) continue;
            
            if ((type_pos = str_tmp.find("TYPE:")) == std::string::npos) continue;

            type = str_tmp.substr(type_pos + strlen("TYPE:"));

            if (public_global_conf.record_set.count(type) == 0)
                continue;
            key = str_tmp.substr(key_pos + strlen("KEY:"), value_pos - key_pos - strlen("KEY:"));

            value = str_tmp.substr(value_pos+strlen("VALUE:"), type_pos - value_pos-strlen("VALUE:"));

            if (str_tmp.find("ADD:") != std::string::npos)
            {
               public_global_conf.record_map[type][key] = value;
                   // public_global_conf.record_map[type].insert(std::make_pair(key,value));
            }
            if (str_tmp.find("DEL:") != std::string::npos)
            {
                std::map<std::string, std::string >::iterator iter = public_global_conf.record_map[type].find(key);
                
                if (iter != public_global_conf.record_map[type].end())
                    public_global_conf.record_map[type].erase(iter);
            }
        }

    }
    catch(...)
    {
        std::cout << "haha" << std::endl;
    }
}


void init()
{
    load_key_type();
    load_key_log();

    for (std::map<std::string, std::map<std::string, std::string> >::iterator iter = public_global_conf.record_map.begin(); iter != public_global_conf.record_map.end(); iter++)
    {
        for (std::map<std::string, std::string>::iterator it = (iter->second).begin(); it!= (iter->second).end(); it++)
        {
            std::cout << "key: " << it->first << ",value: " << it->second << std::endl;
        }
    }

    for (std::set<std::string> ::iterator  iter = public_global_conf.record_set.begin(); iter != public_global_conf.record_set.end(); iter++)
    {
        std::cout <<"KeyType :" << *iter << std::endl;
    }
}

int load_cfg()
{
    ConfigData m_config(_CFG_FILE);
    if (m_config.parse_file() !=0 )
    {
        std::cout << "init file failed." << std::endl;

        exit(1);
    }
}

int main(int argc, char **argv) {
    int port, thread_num;
    if(argc != 3) {
        fprintf(stdout, "Usage: <thread_num> <port>\n", argv[0], argv[1]);
        exit(-1);
    }
    for(int i = 1; i != argc; ++i) {
        int len = strlen(argv[i]);
        for(int j = 0; j != len; ++j) {
            if(!isdigit(argv[i][j])) {
                fprintf(stdout, "argv[%d][%d] is not digital\n", i, j);
                exit(-1);
            }
        }
    }

    thread_num = atoi(argv[1]);
    port = atoi(argv[2]);

    srand(time(NULL));
    sigset_t         mask;	
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sa.sa_mask = mask;    
    if (sigaction(SIGPIPE, &sa, 0) == -1) {
        exit(-1);
    }
    if(pthread_sigmask(SIG_BLOCK, &mask, NULL) != 0) {
        return -1;
    }


    PropertyConfigurator::configure("log4cxx.properties");

    load_cfg();

    init();

    TaskQueueH * task_queue = new TaskQueueH(10000);
    std::string str_prefix("record_redis");
    
    public_global_conf.thread_pool = new TaskThreadPoolH(str_prefix, public_global_conf.thread_pool_number);
    public_global_conf.thread_pool->setTaskQueue(task_queue);
    public_global_conf.thread_pool->start();

    MyLog::DebugLog("Thread create successed.");
    //这里生成用户实现的CalculatorHandler服务，再把其帮定到一个Processor上去，它主要用于处理协议的输入与输出流
    shared_ptr<recordServiceHandler> handler(new recordServiceHandler());
    shared_ptr<TProcessor> processor(new recordServiceProcessor(handler));
    //生成一个传输通道，这里使用了Socket方式
    shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    //生成一个传输工厂，主要用于把上面的transport转换成一个新的应用层传输通道
    shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    //定义了RPC的协议工厂，这里使用了二进制协议，你不可以使用别的协议，如JSON,Compact等
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
    //生成一个多线程的服务端，就是对其加入线程池
    shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(thread_num);
    //shared_ptr<ThreadManager> threadManager = ThreadManager::newThreadManager();
    shared_ptr<PosixThreadFactory> threadFactory = shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
    threadManager->threadFactory(threadFactory);
    threadManager->start();
    TThreadPoolServer server(processor, serverTransport, transportFactory, protocolFactory, threadManager);
    server.serve();

    //Thread Server
#if 0
    shared_ptr<adxServiceHandler> handler(new adxServiceHandler());
    shared_ptr<TProcessor> processor(new adxServiceProcessor(handler));
    shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
    TThreadedServer server(processor, serverTransport, transportFactory, protocolFactory);
    server.serve();
#endif

    return 0;
}