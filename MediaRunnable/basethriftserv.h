#ifndef __BASETHRIFTSERV_H_
#define __BASETHRIFTSERV_H_

#include <string>
#include <vector>
#include <map>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include "RecordRedisThrift/gen-cpp/recordService.h"
#include <errno.h>
//#include <config.h>

using namespace apache::thrift;  
using namespace apache::thrift::protocol;  
using namespace apache::thrift::transport; 


class BaseThriftServ
{
public:
    BaseThriftServ(std::string &ip_port,bool isBinary);
    virtual ~BaseThriftServ();
protected:
    boost::shared_ptr<TTransport> m_Trans; 
    boost::shared_ptr<TProtocol> m_Prot;
    boost::shared_ptr<TSocket> m_Sock;
    recordServiceClient *m_Client; 

    std::string m_ip_port;
    int retry_num;
    bool m_isBinary;   
public:
    int CreatProtocol_Bin();
    int CloseConn();
    int update_retry_num();
    std::string Get_m_ip_port();
    recordServiceClient *getClient()
    {
        return m_Client;
    }
};

#endif
