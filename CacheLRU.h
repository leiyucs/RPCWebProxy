#ifndef CACHELRU_H
#define CACHELRU_H
#include "cache.h"
#include <vector>
#include <algorithm>

class CacheLRU : public Cache {

private:
    std::vector<std::string> urlkey;
public:
    CacheLRU(size_t cachesize);
    std::string* get(std::string url);
    void put(std::string url, std::string webpage);
};

#endif
