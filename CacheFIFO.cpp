#include "CacheFIFO.h"
#include <string>

CacheFIFO::CacheFIFO(size_t cachesize):Cache(cachesize) {
}

std::string* CacheFIFO::get(std::string url) {
    auto it = this->store.find(url);
    if (it == store.end())
        return NULL;
    else {
        return &(it->second);
    }
}


void CacheFIFO::put(std::string url, std::string page) {
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
    else
        size_used = size_used + page.size() - store[url].size();                
    store[url] = page;
}