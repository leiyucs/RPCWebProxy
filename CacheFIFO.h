#ifndef CACHEFIFO_H
#define CACHEFIFO_H
#include "cache.h"
#include <vector>
#include <algorithm>

class CacheFIFO : public Cache {

private:
    std::vector<std::string> urlkey;
public:
    CacheFIFO(size_t cachesize);
    std::string* get(std::string url);
    void put(std::string url, std::string webpage);
};

#endif