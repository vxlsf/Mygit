#include "configData.h"
#include <iostream>
#include <stdlib.h>
#include "rd_info.h"
#include <algorithm>

extern global_conf public_global_conf;
std::string getvalue(std::string &str_tmp)
{
    str_tmp.erase(std::remove(str_tmp.begin(), str_tmp.end(), ' '), str_tmp.end());

    int pos = str_tmp.find("=");

    return str_tmp.substr(pos+1);
}

int ConfigData :: parse_file()
{
    std::ifstream m_stream;

    try {
        m_stream.open(filename.c_str());

        std::string str_tmp;

        while(std::getline(m_stream,str_tmp))
        {
            if (str_tmp.find("threadPoolNumber") != std::string :: npos)
            {
                if ( (public_global_conf.thread_pool_number = atoi(getvalue(str_tmp).c_str())) <= 0)
                {
                    public_global_conf.thread_pool_number = 5;
                }
                std::cout << "threadPoolNumber: " << public_global_conf.thread_pool_number << std::endl; 
                
                continue;
            }

            if (str_tmp.find("keyTypeFile") != std::string :: npos)
            {
                public_global_conf.key_type_file = getvalue(str_tmp);
                
                std::cout << "keytypefile : " << public_global_conf.key_type_file << std::endl;
                continue;
            }

            if (str_tmp.find("keyLogFile") != std::string::npos)
            {
                public_global_conf.key_log_file = getvalue(str_tmp);

                std::cout << "keylogfile : " << public_global_conf.key_log_file << std::endl;
                continue;
            }
			
			if (str_tmp.find("server_ip") != std::string::npos)
            {
                public_global_conf.server_ip = getvalue(str_tmp);

                std::cout << "server_ip : " << public_global_conf.server_ip << std::endl;
                continue;
            }


        }

        m_stream.close();
    }
   catch(...)
   {
       return -1;
       //log some wrong log
   }

   return 0;
}



