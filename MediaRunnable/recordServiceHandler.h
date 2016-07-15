// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.
#ifndef __RECORD_REDIS_SEVICE_HANDLER_
#define __RECORD_REDIS_SEVICE_HANDLER_

#include "basethriftserv.h"
#include "RecordRedisThrift/gen-cpp/recordService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/concurrency/PosixThreadFactory.h>
#include "mylog.h"
#include "rd_info.h"


using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;

using boost::shared_ptr;

extern global_conf public_global_conf;


class recordServiceHandler : virtual public recordServiceIf {
 public:
  recordServiceHandler() {
	  relay_Client = new BaseThriftServ(public_global_conf.server_ip, true);
	  m_bIsConn = relay_Client->CreatProtocol_Bin() == 0 ? true : false;
  }
  
  void Update(std::string& _return, const reqinfo& request) ;

  void GetAllKeyValue(resinfo& _return, const std::string& type) ;

  void CheckOneRecord(std::string& _return, const reqinfo& request) ;

  void PushRunnableType(std::string& _return, const std::string& type) ;

  void DelRunnableType(std::string& _return, const std::string& type) ;

  void DelOneRecord(std::string& _return, const reqinfo& request);

 protected:
  BaseThriftServ *relay_Client;
  bool m_bIsConn;
  
};

#endif
