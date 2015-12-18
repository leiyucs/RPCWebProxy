#include "CacheLRU.h"
#include <string>

CacheLRU::CacheLRU(size_t cachesize):Cache(cachesize) {
}

std::string* CacheLRU::get(std::string url) {
    auto it = this->store.find(url);
    if (it == store.end())
        return NULL;
    else {
	auto urlkeyIt = find(urlkey.begin(), urlkey.end(), url);
        urlkey.erase(urlkeyIt);
        urlkey.push_back(url);
        return &(it->second);
    }
}


void CacheLRU::put(std::string url, std::string page) {
    if (page.size() > cachesize)
        return;
    while (size_used + page.size() > cachesize) {
        std::string oldurl = urlkey.front();
        urlkey.erase(urlkey.begin());
        size_used = size_used - (store[oldurl]).size();
        store.erase(oldurl);
    }

    auto it = find(urlkey.begin(), urlkey.end(), url);
    if (it == urlkey.end()) {
        size_used = size_used + page.size();
        urlkey.push_back(url);
    }
    else {
        size_used = size_used + page.size() - store[url].size();
        urlkey.erase(it);
        urlkey.push_back(url);
    }
    
    store[url] = page;
}

