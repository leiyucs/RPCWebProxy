CXX = g++
FLAGS = -std=c++11 -DHAVE_INTTYPES_H -DHAVE_NETINET_IN_H -Wall -g
INCLUDE_DIR = -I/usr/local/include/thrift
LD_DIR = -L/usr/local/lib

GEN_SRC = ./gen-cpp/RpcProxy.cpp  ./gen-cpp/proxy_constants.cpp ./gen-cpp/proxy_types.cpp

all: Server Client

#add cache policy source code here
Server: Server.cpp $(GEN_SRC) CacheRandom.cpp CacheLRU.cpp CacheFIFO.cpp CacheMAXS.cpp
	$(CXX) $^ -o $@ $(LD_DIR) $(FLAGS) $(INCLUDE_DIR) -lcurl -lthrift

Client: Client.cpp $(GEN_SRC)
	$(CXX) $^ -o $@ $(LD_DIR) $(FLAGS) $(INCLUDE_DIR) -lthrift

clean:
	rm *.o ./gen-cpp/*.o Server Client
