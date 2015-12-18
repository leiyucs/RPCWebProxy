#ifndef CACHE_H
#define CACHE_H

#include <string>
#include <iostream>
#include <unordered_map>

class Cache {
protected:
	size_t cachesize;  //capacity
	size_t size_used;  //occupied  size
	std::unordered_map<std::string, std::string> store;
public:
	Cache(size_t size):cachesize(size),size_used(0){};
	size_t size() {return size_used;}
	virtual ~Cache(){};
	virtual std::string* get(std::string url) = 0 ;
	virtual void put(std::string url, std::string page) = 0;
};

#endif