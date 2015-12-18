#ifndef CACHERANDOM_H
#define CACHERANDOM_H
#include "cache.h"
#include <list>
class CacheRandom : public Cache {

private:
	std::list<std::string> urlkey;
	void evict();
public:
	CacheRandom(size_t cachesize);
	std::string* get(std::string url);
	void put(std::string url, std::string webpage);
};

#endif