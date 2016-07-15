OBJECT= basethriftserv.o recordRedis.o rd_info.o ThreadSync.o  Runnable.o Task.o TaskQueue.o TaskRunnable.o TaskThreadPool.o Thread.o recordServiceHandler.o mylog.o  configData.o WorkTask.o ./RecordRedisThrift/gen-cpp/recordService.o ./RecordRedisThrift/gen-cpp/record_redis_constants.o ./RecordRedisThrift/gen-cpp/record_redis_types.o 
CPP=g++ -g
#FLAG=-D DEBUG_MOD
FLAG=

ITHRIFT=-I/usr/local/thrift/include/ -I/usr/local/thrift/include/thrift/
LTHRIFT=/usr/local/thrift/lib/libthrift.a
IBOOST=-I/usr/include/boost/

IADXSERVTHRIFT=-I./RecordRedisThrift/gen-cpp

IHIREDIS=-I /usr/local/hiredis/include/hiredis
LHIREDIS=-L /usr/local/hiredis/lib -lhiredis -Wl,-rpath=/usr/local/hiredis/lib
ILOG4CXX=-I /usr/local/log4cxx/include
LLOG4CXX=-L /usr/local/log4cxx/lib/ -llog4cxx -Wl,-rpath=/usr/local/log4cxx/lib/

all: localRecordServer
$(OBJECT):%.o:%.cpp
	$(CPP) -I. ${ITHRIFT} ${IBOOST} ${IADXSERVTHRIFT} ${ILOG4CXX} ${IHIREDIS} ${IPROTO} -DHAVE_NETINET_IN_H -c $< -o $@
localRecordServer:$(OBJECT)
	$(CPP) -o localRecordServer ${OBJECT} ${LTHRIFT} ${LLOG4CXX} ${LHIREDIS} -lpthread -ldl  
clean:
	$(RM) *.o localRecordServer ./RecordRedisThrift/gen-cpp/*.o ./RecordRedisThrift/gen-cpp/*.h ./RecordRedisThrift/gen-cpp/*.cpp 
	/usr/local/thrift/bin/thrift -o ./RecordRedisThrift --gen cpp ./RecordRedisThrift/record_redis.thrift
 
