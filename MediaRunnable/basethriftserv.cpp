#include "basethriftserv.h"
#include "RecordRedisThrift/gen-cpp/recordService.h"
#include "mylog.h"
#include "rd_info.h"

extern global_conf public_global_conf; 

bool GetIPandPort(std::string ip_port,std::string &ip,int &port)
{
    bool res=false;
    std::string::size_type pos=ip_port.find(":");
    if(pos!=std::string::npos)
    {   
        ip=ip_port.substr(0,pos);
        port=atoi(ip_port.substr(pos+1,ip_port.size()-pos-1).c_str());
        res=true;
    }   
    else
    {   
        res=false;
    }   
    return res;
}
BaseThriftServ::BaseThriftServ(std::string &ip_port,bool isBinary)
{
    retry_num = 0;
    m_isBinary=isBinary;
    m_ip_port=ip_port;
}

BaseThriftServ::~BaseThriftServ()
{
    CloseConn();
}
int BaseThriftServ::update_retry_num()
{
    retry_num++;

    if (retry_num >=3)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int BaseThriftServ::CreatProtocol_Bin()
{
    std::string strip;
    int iport;
    if(GetIPandPort(m_ip_port,strip,iport))
    {
        
        boost::shared_ptr<TSocket> socket(new TSocket(strip.c_str(), iport));
		    boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
		    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
		    socket->setRecvTimeout(100);
		    socket->setConnTimeout(100);
		    try {
				  transport->open();
		    } catch (TException &tx) {
		        MyLog::DebugLog("CreatProtocol_Bin err! ("+m_ip_port+"), ["+std::string(tx.what())+"]");
			return -1;
		    }

            recordServiceClient * client = new recordServiceClient(protocol);
            m_Client = client;
		    m_Sock=socket;
		    m_Trans=transport; 
		    m_Prot=protocol;
	  }
    
    
    return 0;
}

    
int BaseThriftServ::CloseConn()
{
		try {
    m_Trans->close();
    } catch (TException &tx) {
		        MyLog::DebugLog("CloseConn err! ("+m_ip_port+"), ["+std::string(tx.what())+"]");
		}
    return 0;
}

std::string BaseThriftServ::Get_m_ip_port()
{
		return m_ip_port;
}
