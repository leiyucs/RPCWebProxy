#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <boost/random.hpp>
#include <cmath>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "./gen-cpp/RpcProxy.h"

#define NUMBER_OF_REQUEST 500
#define MAX_LIST_NUM 200
using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace proxy;

double lambda=0;
int workload_size = 0;
int workload_count[MAX_LIST_NUM]={};

double timeduration(struct timeval t1, struct timeval t2)
{
    return (t2.tv_sec-t1.tv_sec)*1000000+(t2.tv_usec-t1.tv_usec);
}

void genWorkload(vector<string>& workload, string urllistfile, int mode) {
  ifstream input(urllistfile.c_str());
  vector<string> list;
  string line;
  while (getline(input,line)) {
    list.push_back(line);
  }
  int listsize = (int) list.size();
  workload_size = listsize;
  switch(mode) {
    case 1: {
      for (int i=0; i<NUMBER_OF_REQUEST; i++) {
        int index = rand()%listsize;
        workload.push_back(list[index]);
        workload_count[index]++;
      }
      break;
    }
    case 2: {
      boost::mt19937 rnd_gen(1);
      boost::exponential_distribution<> exprng(lambda);
      int i =0;
      while (i<NUMBER_OF_REQUEST) {
        int index = floor(exprng(rnd_gen));
        if(index<listsize) {
          workload.push_back(list[index]);
          workload_count[index]++;
          i++;
        }
      }
      break;
    }
  }
}

int main(int argc, char** argv) {
  int mode;
  vector<string> workload;
  if (argc<2) {
    cout<<"usage: Client workload_type" <<endl;
    cout<<"workload_type: 1--uniform random 2-- exponential distribution"<<endl;
    return 0;
  }
  else {
    mode=atoi(argv[1]);
    if (mode == 2) {
      if(argc!=3)  {
      cout<<"if workload_type=2, usage: Client workload_type lambda "<<endl;
      return 0;
    }
      lambda=atof(argv[2]);
      cout<<"exponential workload with lambda value"<<lambda<<endl;
    }
  }

  genWorkload(workload, "url_list", mode);

  boost::shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  RpcProxyClient client(protocol);
  
  string webpage;
  struct timeval t1, t2;
  double duration=0.0;
  try {
    transport->open();
    for (vector<string>::iterator it = workload.begin(); it != workload.end(); it++) {
      cout << "Request Page " + *it << endl;
      gettimeofday(&t1,NULL);
      client.getWebpage(webpage,*it);
      gettimeofday(&t2,NULL);
      duration += timeduration(t1,t2);
      //cout<<webpage<<endl;
    }
    transport->close();
  } catch (TException& tx) {
    cout << "ERROR: " << tx.what() << endl;
  }
  for (int i=0; i<workload_size; i++)
      cout<<"url no."<<i<<" "<<workload_count[i]<<endl;
  cout<<"average access latency of workload = " << duration/NUMBER_OF_REQUEST << endl;
}
