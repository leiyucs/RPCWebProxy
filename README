# A Simple RPC based Web Proxy

Requirement:  
1. require g++ version that support option "-std=c++11".  We use g++ 4.8.4.
2. Before "make", in the Makefile, the locations of libraries and header files below should be set to their locations on your machine if they are different.
 "INCLUDE_DIR = -I/usr/local/include/thrift
   LD_DIR = -L/usr/local/lib"
3. In the client program, the "localhost" should be changed to the server's IP address in your experiment platform, before "make".

Build: 
    run "make" to compile source code that generates two executable file : "Client" and "Server"

Usage:
    commandline for Server:   ./Server cachesize cachepolicy
	cachesize is the number with unit KB and MB, for example: 100KB, 200KB, 1MB, 2MB, which specifies the size of the cache.
	cachepolicy is a number from 1 to 4 which specifies the cache replacement policy.
			 1---random  2----LRU   3----MAXS 4----FIFO
	Example:  ./Server 2MB 1  


    commandline for Client:   ./Client typeofworkload [parameter for exponential distribution]
	typeofworkload is a number from 1 to 2
		1---- uniform random distribution
		2---- exponential distribution
	when typeofworkload=2,  the parameter lambda for the exponential distribution should be specified. 
	Example:  ./Client 1
		  ./Client 2 0.5
		  ./Client 2 1
	 
	 
     Server should be started before running Client. In our program, running the client makes 1000 requests to the server and the server measures the cache hit ratio within 1000 requests.　The server program should be restarted for another run of the client program, in order to measure the cache hit ratio at the server side for a batch of 1000 requests each time.  
