#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "cache.h"
#include <string>
#include <sstream>
#include <curl/curl.h>
#include "./gen-cpp/RpcProxy.h"
#include "CacheRandom.h"
#include "CacheLRU.h"
#include "CacheFIFO.h"
#include "CacheMAXS.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace  ::proxy;

size_t cache_size;
int cache_policy;

int totalhits=0;
int getquestsnum =0;

size_t convert(size_t size, std::string unit) {
  size_t result = 0;
  if (unit == "KB") 
    result = size*1024;
  else if (unit == "MB")
    result = size*1024*1024;
  else
    fprintf(stderr, "Error: unit not supported.\n");
  return result;
}

/*This function is registered as a callback with CURL.  As the data
   from the requested webpage is returned, write_data is called.*/

static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
  std::string *s =(std::string *) userp;
  size_t realsize = size*nmemb;
  s->append((const char*)buffer, realsize);
  return realsize;
}


class RpcProxyHandler : virtual public RpcProxyIf {
 private:
  Cache *cache;
 public:
  RpcProxyHandler() {
    // Your initialization goes here
    switch (cache_policy) {
      case 1: //random
        cache = new CacheRandom(cache_size);
        std::cout<<"cache size "<<cache_size<<" random policy" <<std::endl;
        break;
      case 2: //LRU
        cache = new CacheLRU(cache_size);
        std::cout<<"cache size "<<cache_size<< " LRU policy" <<std::endl;
        break;
      case 3: //MAXS
        cache = new CacheMAXS(cache_size);
        std::cout<<"cache size "<<cache_size<<" MAXS policy" <<std::endl;
        break;
      case 4: //FIFO
        cache = new CacheFIFO(cache_size);
        std::cout<<"cache size "<<cache_size<<" FIFO policy" <<std::endl;
        break;
    }
  }
  ~RpcProxyHandler() {
    delete cache;
  }

  void getWebpage(std::string& _return, const std::string& url) {
    // Your implementation goes here
    std::cout<<"getWebpage "+url <<std::endl;
    std::string *page;
    getquestsnum++;
    if ((page=cache->get(url))) {  //cache hit
      _return=*page;
      totalhits++;
    }
    else {   //cache miss
      std::string webpage;
      CURL *curl = curl_easy_init();
      CURLcode res;
      if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &webpage);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
          fprintf(stderr, "curl_easy_perform() failed:%s.\n", curl_easy_strerror(res));
        }
        else {
          _return = webpage;
          curl_easy_cleanup(curl);

          cache->put(url, webpage);
        }
      }
      else {
        fprintf(stderr, "Error: could not get CURL handle.\n");
        exit(EXIT_FAILURE);
      }
    }
    std::cout<<"hit ratio so far =" << totalhits/(float)getquestsnum <<std::endl;
  }
};

int main(int argc, char **argv) {
  int port = 9090;
  if (argc<3) {
    std::cout<<"usage: Server cache-size cache-policy"<<std::endl;
    std::cout<<"cache-policy: 1---random  2----LRU   3----MAXS 4----FIFO"<<std::endl;
    return 0;
  }
  else {
    std::string unit;
    std::istringstream ss(argv[1]);
    ss>>cache_size>>unit;
    cache_size = convert(cache_size,unit);
    cache_policy = atoi(argv[2]);
  }

  boost::shared_ptr<RpcProxyHandler> handler(new RpcProxyHandler());
  boost::shared_ptr<TProcessor> processor(new RpcProxyProcessor(handler));
  boost::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  boost::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  std::cout << "Starting the server..." << std::endl;
  server.serve();
  return 0;
}