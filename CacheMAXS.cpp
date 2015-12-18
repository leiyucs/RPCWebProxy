#include "CacheMAXS.h"
#include <string>

CacheMAXS::CacheMAXS(size_t cachesize):Cache(cachesize) {
}

std::string* CacheMAXS::get(std::string url) {
    auto it = this->store.find(url);
    if (it == store.end())
        return NULL;
    else {
        return &(it->second);
    }
}


void CacheMAXS::put(std::string url, std::string page) {
    if (page.size() > cachesize)
        return;
    while (size_used + page.size() > cachesize) {
        auto oldurlpair = *(urlkey.begin());
        urlkey.erase(urlkey.begin());
        size_used = size_used - (store[oldurlpair.second]).size();
        store.erase(oldurlpair.second);
    }

    auto it = urlkey.begin();
    for (; it != urlkey.end(); it++) {
        if (it->second == url)
            break;
    }
    if (it == urlkey.end()) {
        size_used = size_used + page.size();
        urlkey.insert(make_pair(page.size(), url));
    }
    else {
        size_used = size_used + page.size() - store[url].size();
    }
    store[url] = page;
}