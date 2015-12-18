#ifndef CACHEMAXS_H
#define CACHEMAXS_H
#include "cache.h"
#include <set>
#include <algorithm>

class CacheMAXS : public Cache {
private:
    std::set<std::pair<int, std::string>, std::greater<std::pair<int, std::string> > > urlkey;
public:
    CacheMAXS(size_t cachesize);
    std::string* get(std::string url);
    void put(std::string url, std::string webpage);
};

#endif