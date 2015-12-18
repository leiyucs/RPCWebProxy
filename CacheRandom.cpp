#include "CacheRandom.h"
#include <string>
CacheRandom::CacheRandom(size_t cachesize):Cache(cachesize) {
	srand(time(NULL));
}

void CacheRandom::evict() {
	int index = rand()%urlkey.size();
	int i = 0;
	std::list<std::string>::iterator it = urlkey.begin();
	//std::cout <<size_used<<" " <<index<<" " << urlkey.size() <<"here" << std::endl;

	while (i<index) {
		it++;
		i++;
	}
	auto search = store.find(*it);
	size_used = size_used - search->second.size();
	store.erase(*it);
	urlkey.erase(it);
}

std::string* CacheRandom::get(std::string url) {
	std::string *result;
	auto search = store.find(url);
	if (search != store.end()) {
		result = &search->second;
	}
	else
		result = NULL;
	return result;
}


void CacheRandom::put(std::string url, std::string page) {

	if (page.size() > cachesize)
		return;
	while (size_used + page.size() > cachesize) {
		evict();
	}
	auto search = store.find(url);
	if (search != store.end())
	{
		size_used = size_used - search->second.size();
		search->second = page;
		size_used = size_used + page.size();
	}
	else
	{
		store.insert({url, page});
		size_used += page.size();
		urlkey.push_back(url);
	}
}